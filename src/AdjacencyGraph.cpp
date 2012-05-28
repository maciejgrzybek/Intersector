#include "AdjacencyGraph.h"
#include "FigureFactory.h"
#include "Figure.h"

#include <queue>

AdjacencyGraph::AdjacencyGraph(const PointVector& pointVec, const PointPairVector& pairVec, unsigned int d)
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
    std::cout << "compares: " << pairIter->first << " with " << pairIter->second << std::endl;
//        FigureFactory& factory = getFigureFactory();
        FigureFactory& factory = CircleFactory::getInstance();
        FigurePtr a = factory.getFigure(pairIter->first,d);
        FigurePtr b = factory.getFigure(pairIter->second,d);
        if(a->intersects(*b))
        {
            std::cout << a->getCenter() << " vs " << b->getCenter() << std::endl;
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

void AdjacencyGraph::solve()
{
    VertexComparator vc(*this);
    std::priority_queue<VertexID, std::vector<VertexID>, VertexComparator> Q(vc);
    boost::graph_traits<AdjGraph>::vertex_iterator vi, vi_end;
    boost::tie(vi,vi_end) = boost::vertices(graph);
    for(;vi!=vi_end;++vi)
    {
        std::cout << graph[*vi] << std::endl;
        Q.push(*vi);
    }
    // now pop'ing from priority_queue will give vertexes by amount of adjacencies (from the highest to the lowest)
    VertexID current = Q.top();
    while(!Q.empty() && adjacencesCount(current)>0)
    {
    std::cout << "current: " << graph[current] << "(" << adjacencesCount(current) << ")" << std::endl;
    boost::graph_traits<AdjGraph>::edge_iterator ei, ei_end;
    boost::tie(ei,ei_end) = boost::edges(graph);
    std::cout << "edges:" << std::endl;
    for(;ei != ei_end;++ei)
    {
        std::cout << graph[boost::source(*ei,graph)] << "->" << graph[boost::target(*ei,graph)] << std::endl;
    }
        boost::clear_vertex(current,graph);
        boost::remove_vertex(current,graph);
    boost::tie(ei,ei_end) = boost::edges(graph);
    std::cout << "AFTER:edges:" << std::endl;
    for(;ei != ei_end;++ei)
    {
        std::cout << graph[boost::source(*ei,graph)] << "->" << graph[boost::target(*ei,graph)] << std::endl;
    }
        Q.pop();
        current = Q.top();
    }
    std::cout << "is empty: " << Q.empty() << " current: " << graph[current] << "("<<adjacencesCount(current)<<")" << std::endl;
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
    std::cout << parent.graph[lhs] << "("<< parent.adjacencesCount(lhs) <<")" << "vs" << parent.graph[rhs] << "(" << parent.adjacencesCount(rhs) << ") ";
    bool result = parent.adjacencesCount(lhs)<=parent.adjacencesCount(rhs); // unfortunatelly linear complexity
    std::cout << "= " << result << std::endl; 
    return result;
}
