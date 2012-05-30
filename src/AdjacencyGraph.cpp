#include "AdjacencyGraph.h"
#include "FigureFactory.h"
#include "Figure.h"

#include <queue>

AdjacencyGraph::AdjacencyGraph(const PointVector& pointVec, const PointPairVector& pairVec, unsigned int d, FigureType figure)
{
    PointVertexMap pointVertex;
    /* add points to graph */
    PointVector::const_iterator iter = pointVec.begin();
    PointVector::const_iterator endIter = pointVec.end();
    for(;iter!=endIter;++iter)
    {
        VertexID v = boost::add_vertex(graph);
        graph[v] = *iter;
        pointVertex[*iter] = v;
    }
    /* add edges to these vertexes which represent intersecting figures */
    PointPairVector::const_iterator pairIter = pairVec.begin();
    PointPairVector::const_iterator pairEndIter = pairVec.end();
    for(;pairIter!=pairEndIter;++pairIter)
    {
//        FigureFactory& factory = getFigureFactory();
        FigurePtr a;
        FigurePtr b;
        if(figure == CIRCLE)
        {
            a = CircleFactory::getInstance().getFigure(pairIter->first,d);
            b = CircleFactory::getInstance().getFigure(pairIter->second,d);
        }
        else
        {
            a = SquareFactory::getInstance().getFigure(pairIter->first,d);
            b = SquareFactory::getInstance().getFigure(pairIter->second,d);
        }
        if(a->intersects(*b))
        {
            EdgeID edge;
            bool ok;
            boost::tie(edge,ok) = boost::add_edge(pointVertex[pairIter->first],pointVertex[pairIter->second],graph);
            if(!ok)
            {
                // something went wrong. There should not be two the same edges in graph.
            }
        }
    }
}

/*void(std::set& c,std::vector<VertexID>& inv)
{
    std::vector<VertexID>::iterator inv = inv.begin();
    std::vector<VertexID>::iterator invEnd = inv.end();
    for(;inv != invEnd; ++inv)
    {
        AdjGraph::adjacency_iterator neighbourIt, neighbourEnd;
        boost::tie(neighbourIt, neighbourEnd) = boost::adjacent_vertices(*inv,graph);
        for(;neighbourIt != neighbourEnd; ++neighbourIt)
        {
            
        }
    }
}*/

void AdjacencyGraph::solve(bool state)
{
    VertexComparator vc(*this);
    std::priority_queue<VertexID, std::vector<VertexID>, VertexComparator> Q(vc);
    //std::vector<VertexID> Q;
    boost::graph_traits<AdjGraph>::vertex_iterator vi, vi_end;
    boost::tie(vi,vi_end) = boost::vertices(graph);
    for(;vi!=vi_end;++vi)
    {
//        Q.push_back(*vi);
        Q.push(*vi);
    }
//    std::sort(Q.begin(),Q.end(),vc);
    // now pop'ing from priority_queue will give vertexes by amount of adjacencies (from the highest to the lowest)
    VertexID current = Q.top();
    while(!Q.empty() && adjacencesCount(current)>0)
    {
        boost::clear_vertex(current,graph);
        boost::remove_vertex(current,graph);
        Q.pop();
        current = Q.top();
    }
    if(state)
    {
        boost::graph_traits<AdjGraph>::edge_iterator ei, ei_end;
        boost::tie(ei,ei_end) = boost::edges(graph);
        do
        {
            std::priority_queue<VertexID, std::vector<VertexID>, VertexComparator> q(vc);
            while(!Q.empty())
            {
                q.push(Q.top());
                Q.pop();
            }

            VertexID curr = q.top();
            while(!q.empty() && adjacencesCount(curr)>0)
            {
                boost::clear_vertex(curr,graph);
                boost::remove_vertex(curr,graph);
                q.pop();
                curr = q.top();
            }
/*            for(;ei != ei_end;++ei)
            {
                current = boost::source(*ei,graph);
                std::cout << graph[current] << std::endl;
                boost::clear_vertex(current,graph);
                boost::remove_vertex(current,graph);
                break;
            }//break;*/
            boost::tie(ei,ei_end) = boost::edges(graph);
            if(ei != ei_end)
            {
                while(!q.empty())
                {
                    Q.push(q.top());
                    q.pop();
                }
            }
        } while(ei != ei_end); // if there are still some edges
    }
}

PointVector AdjacencyGraph::getPointVector() const
{
    PointVector result;
    boost::graph_traits<AdjGraph>::vertex_iterator vi, vi_end;
    boost::tie(vi,vi_end) = boost::vertices(graph);
    for(;vi!=vi_end;++vi)
    {
        result.push_back(graph[*vi]);
    }
    return result;
}

size_t AdjacencyGraph::adjacencesCount(const VertexID& vertex) const
{
    AdjGraph::adjacency_iterator neighbourIt, neighbourEnd;
    boost::tie(neighbourIt, neighbourEnd) = boost::adjacent_vertices(vertex,graph);
    return distance(neighbourIt,neighbourEnd);
}

AdjacencyGraph::VertexComparator::VertexComparator(AdjacencyGraph& adj) : parent(adj)
{}

bool AdjacencyGraph::VertexComparator::operator()(const VertexID& lhs, const VertexID& rhs) const
{
    bool result = parent.adjacencesCount(lhs)<=parent.adjacencesCount(rhs); // unfortunatelly linear complexity
    return result;
}
