// Digraph.hpp
//
// Heath Sias
// ICS 46 Spring 2015
//
// Declaration of a Digraph template class, a generic directed graph, using the
// adjacency lists technique (each vertex stores a linked list of its outgoing edges).
//
// Constraints: EdgeInfo and VertexInfo types must be well-behaved.
//              Specifically, their destructors must clean up any
//              dynamically allocated members.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <stdexcept>
#include <queue>
#include <iostream>



// A generic exception class, useable by any source file which includes this header.
//
class DigraphException
{
public:
    DigraphException(const std::string& reason): reason_{reason} { }

    std::string reason() const { return reason_; }

private:
    std::string reason_;
};



// As different kinds of Digraphs store different kinds of edge information,
// DigraphEdge is a template.
//
template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes a VertexInfo object and a list of
// its outgoing edges.  As different kinds of Digraphs store different kinds of
// edge information, DigraphVertex is a template.
//
template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};



// An SPVertex includes a vertex number, a 'known' flag, a distance,
// and a previous vertex. It is used when running Dijkstra's
// Short Path Algorithm to keep track of current shortest path info
//
struct SPVertex
{
    int number;
    bool known;
    double distance;
    int previous;
};



// SPDistanceComparator is a comparison class for use in a priority queue.
// It is used exclusively in findShortestPath()
//
struct SPDistanceComparator
{
    bool operator()(const SPVertex& lhs, const SPVertex& rhs) const
    {
        return lhs.distance < rhs.distance;
    }
};



// Digraph template class, a generic directed graph, using the adjacency lists
// technique (each vertex stores a linked list of its outgoing edges).
//
template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // A new, empty Digraph with no vertices and no edges.
    Digraph();

    // A deep copy of another Digraph object
    Digraph(const Digraph& d);

    // Deallocates any memory associated with the Digraph.
    ~Digraph();

    // Assigns the contents of the given Digraph into this Digraph
    Digraph& operator=(const Digraph& d);

    // Returns all vertex numbers contained in this Digraph.
    std::vector<int> vertices() const;

    // Returns all edges contained in this Digraph.
    std::vector<std::pair<int, int>> edges() const;

    // Returns all edges outgoing from the given vertex. Throws a DigraphException
    // if the given vertex does not exist.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // Returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  Throws a DigraphException if the given
    // vertex does not exist.
    VertexInfo vertexInfo(int vertex) const;

    // Returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  Throws a DigraphException
    // if either vertex does not exist or if the corresponding edge does not exist.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // Adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If the corresponding vertex already exists,
    // throws a DigraphException.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // Adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  Throws a DigraphException
    // if one of the vertices does not exist or if the edge already
    // exists.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // Removes the vertex (and all of its incoming and outgoing edges) with the
    // given vertex number from the Digraph.  Throws a DigraphException if the
    // vertex does not exist.
    void removeVertex(int vertex);

    // Removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // Throws a DigraphException if one of the vertices does not exist or if the ]
    // edge does not exist.
    void removeEdge(int fromVertex, int toVertex);

    // Returns the number of vertices in the graph.
    int vertexCount() const;

    // Returns the total number of edges in the graph.
    int edgeCount() const;

    // Returns the number of edges in the graph that are outgoing from the given
    // vertex number. Throws a DigraphException if the given vertex does not exist.
    int edgeCount(int vertex) const;

    // Returns true if the Digraph is strongly connected, false if not.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the precedessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;

    // clears the graph
    void clearGraph();



private: // member variables
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> graph;



private: // member functions
    void enumerateConnections(int vertex, std::map<int, bool>& visited) const;
};



template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
    : graph{}
{}



template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
    : graph{d.graph}
{}



template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph()
{}



template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    graph = d.graph;
    return *this;
}



template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
    std::vector<int> allVertices{};
    for (auto v : graph)
    {
        allVertices.push_back(v.first);
    }
    return allVertices;
}



template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int, int>> allEdges{};
    for (auto v : graph)
    {
        for (auto e : v.second.edges)
        {
            allEdges.push_back(std::pair<int,int>{e.fromVertex, e.toVertex});
        }
    }
    return allEdges;
}



template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    std::vector<std::pair<int, int>> theseEdges{};
    try
    {
        for (auto e : graph.at(vertex).edges)
        {
            theseEdges.push_back(std::pair<int,int>{e.fromVertex, e.toVertex});
        }
    }
    catch (const std::out_of_range& oor)
    {
        throw DigraphException("Vertex does not exist!");
    }
    return theseEdges;
}



template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    try
    {
        return graph.at(vertex).vinfo;
    }
    catch (const std::out_of_range& oor)
    {
        throw DigraphException("Vertex does not exist!");
    }
}



template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    try
    {
        for (auto e : graph.at(fromVertex).edges)
        {
            if (e.toVertex == toVertex)
            {
                return e.einfo;
            }
        }
        throw DigraphException("Edge does not exist!");
    }
    catch (const std::out_of_range& oor)
    {
        throw DigraphException("Vertex does not exist!");
    }
}



template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    if (graph.count(vertex) > 0)
    {
        throw DigraphException("Vertex already exists!");
    }
    DigraphVertex<VertexInfo, EdgeInfo> thisVertex;
    thisVertex.vinfo = vinfo;
    thisVertex.edges = {};
    graph.insert(std::pair<int,DigraphVertex<VertexInfo, EdgeInfo>>(vertex, thisVertex));

}



template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
    try
    {
        graph.at(toVertex); // see if it exists
        for (auto e : graph.at(fromVertex).edges)
        {
            if (e.toVertex == toVertex)
            {
                throw DigraphException("Edge already exists!");
            }
        }
        DigraphEdge<EdgeInfo> thisEdge;
        thisEdge.fromVertex = fromVertex;
        thisEdge.toVertex = toVertex;
        thisEdge.einfo = einfo;
        graph.at(fromVertex).edges.push_back(thisEdge);
    }
    catch (const std::out_of_range& oor)
    {
        throw DigraphException("Vertex does not exist!");
    }
}



template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    if (graph.erase(vertex) == 0)
    {
        throw DigraphException("Vertex does not exist!");
    }

    for (auto v : graph)
    {
        for (auto it = v.second.edges.begin(); it != v.second.edges.end();)
        {
            if (it->toVertex == vertex)
            {
                it = v.second.edges.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}



template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    if (graph.count(toVertex) == 0)
    {
        throw DigraphException("Vertex does not exist!");
    }
    try
    {
        for (auto it = graph.at(fromVertex).edges.begin(); it != graph.at(fromVertex).edges.end();)
        {
            if (it->toVertex == toVertex)
            {
                it = graph.at(fromVertex).edges.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    catch (const std::out_of_range& oor)
    {
        throw DigraphException("Vertex does not exist!");
    }
}



template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const
{
    return graph.size();
}



template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const
{
    int numEdges = 0;
    for (auto v : graph)
    {
        numEdges += v.second.edges.size();
    }
    return numEdges;
}



template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    try
    {
        return graph.at(vertex).edges.size();
    }
    catch (const std::out_of_range& oor)
    {
        throw DigraphException("Vertex does not exist!");
    }
}



template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    for (auto v : graph)
    {
        std::map<int, bool> visited;
        for (auto it : graph)
        {
            visited[it.first] = false;
        }

        enumerateConnections(v.first, visited);
        for (auto i : visited)
        {
            if (i.second == false)
            {
                return false;
            }
        }
    }
    return true;
}



template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::enumerateConnections(int vertex, std::map<int, bool>& visited) const
{
    if (visited.at(vertex) == true)
    {
        return;
    }

    visited[vertex] = true;
    for (auto e : graph.at(vertex).edges)
    {
        enumerateConnections(e.toVertex, visited);
    }
}



template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    // vertex : current info map -> for algorithm
    std::map<int, SPVertex> infoMap;

    for (auto v : graph)
    {
        infoMap[v.first] = {v.first, false, std::numeric_limits<double>::infinity(), v.first};
    }
    infoMap[startVertex] = {startVertex, false, 0.0, startVertex};

    std::priority_queue<SPVertex, std::vector<SPVertex>, SPDistanceComparator> pq;
    pq.push(infoMap[startVertex]);

    while (!pq.empty())
    {
        SPVertex spV = pq.top();
        pq.pop();

        if (spV.known == false)
        {
            spV.known = true;

            for (auto e : graph.at(spV.number).edges)
            {
                if (infoMap.at(e.toVertex).distance > spV.distance + edgeWeightFunc(e.einfo))
                {
                    infoMap.at(e.toVertex).distance = spV.distance + edgeWeightFunc(e.einfo);
                    infoMap.at(e.toVertex).previous = spV.number;
                    pq.push(infoMap[e.toVertex]);
                }
            }
        }
    }

    // vertex : previous vertex map -> to return
    std::map<int, int> traceMap;

    // add all vertexNum, Pvs to traceMap and return;
    for (auto v : infoMap)
    {
        traceMap[v.second.number] = v.second.previous;
    }

    return traceMap;
}



template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::clearGraph()
{
    graph.clear();
}



#endif // DIGRAPH_HPP
