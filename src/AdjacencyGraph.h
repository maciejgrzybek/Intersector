#ifndef ADJACENCY_GRAPH_H
#define ADJACENCY_GRAPH_H

#include "Space.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/unordered_map.hpp>

class AdjacencyGraph
{
public:
    /**
     * Constructor generates AdjacencyGraph from given PointPairVector
     * @param const PointVector& Reference to PointVector containing points to build graph from.
     * @param const PointPairVector& Reference to PointPairVector to graph be build from.
     * @param unsigned int Size of figure.
     */
    AdjacencyGraph(const PointVector&,const PointPairVector&, unsigned int, FigureType);

    void solve(bool = true);

    PointVector getPointVector() const;

protected:
    typedef boost::adjacency_list<  // adjacency_list is a template depending on:
                boost::listS,       // container for edges [std::list]
                boost::listS,        // container for vertices [std::list]
                boost::undirectedS, // undirected edges
                Point               // type of vertex
              > AdjGraph;
    typedef AdjGraph::vertex_descriptor VertexID;
    typedef AdjGraph::edge_descriptor EdgeID;
    typedef boost::unordered_map<Point,VertexID,PointHash> PointVertexMap;

    class VertexComparator
    {
        AdjacencyGraph& parent;
    public:
        VertexComparator(AdjacencyGraph&);
        bool operator()(const VertexID&, const VertexID&) const;
    };

    /**
     * Returns number of adjacences of given vertex.
     * @param const VertexID& Reference to vertex.
     * @return size_t Number of adjacency vertexes.
     */
    size_t adjacencesCount(const VertexID&) const;

    AdjGraph graph;

};

#endif
