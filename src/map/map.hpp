
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef MAP_HPP
#define MAP_HPP

#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/graph_traits.hpp>
#include<boost/graph/r_c_shortest_paths.hpp>

namespace ds {

struct GraphVertexPropertyMap {
    GraphVertexPropertyMap(int number = 0, int time = 0, int x = 0, int y = 0)
        : num_{ number }, time_{ time }, x_{ x }, y_{ y } {}

    int num_;                               // property number
    int time_;                              // seconds
    int x_;                                 // x coordinate
    int y_;                                 // y coordinate
};

struct GraphEdgePropertyMap {
    GraphEdgePropertyMap(int number = 0, int distance = 0, int time = 0)
        : num_{ number }, distance_{ distance }, time_{ time } {}

    int num_;                               // property number
    int distance_;                          // meters
    int time_;                              // seconds
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    GraphVertexPropertyMap, GraphEdgePropertyMap, boost::no_property,
    boost::listS>;

// ResourceContainer model
struct GraphRC {
    GraphRC(int distance, int time)
        : distance_{ distance }, time_{ time } {}

    int distance_;                          // meters
    int time_;                              // seconds
};

bool operator==(const GraphRC& rc1, const GraphRC& rc2);

bool operator<(const GraphRC& rc1, const GraphRC& rc2);

// ResourceExtensionFunction model
class GraphREF {
public:
    inline bool operator()(const Graph& g, GraphRC& newRC, const GraphRC& oldRC,
        boost::graph_traits<Graph>::edge_descriptor ed) const
    {
        newRC.distance_ = oldRC.distance_ + g[ed].distance_;
        newRC.time_     = oldRC.time_     + g[ed].time_;
        return newRC.time_ <= g[target(ed, g)].time_;
    }
};

// DominanceFunction model
class GraphDF {
public:
    inline bool operator()(const GraphRC& rc1, const GraphRC& rc2) const
    {
        return rc1.distance_ <= rc2.distance_
            && rc1.time_     <= rc2.time_;
    }
};

class Map {
public:
    static constexpr auto maxDelTime_   { 60* 60 };             // maximum delivery time, seconds
    static constexpr auto noPathTime_   { maxDelTime_ * 5 };    // time for "no path", seconds
    static constexpr auto waitingTime_  { 60 * 4 };             // waiting time, seconds

    static constexpr auto scale_        { 10 };
    static constexpr auto avgSpeed_     { 10 };                 // average courier speed, meters per second

    static constexpr auto vertexRadius_ { 6 };                  // in pixels
    static constexpr auto edgeWidth_    { 2 };                  // in pixels

public:
    Map();

    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;

    virtual ~Map() noexcept {}

    const Graph& graph() const { return g_; }

public:
    size_t addVertex(int x, int y);

    void removeVertex(size_t vertex);

    Graph::edge_descriptor addEdge(size_t srcVertex, size_t tgtVertex, int distance);

    void removeEdge(size_t srcVertex, size_t tgtVertex);

private:
    Graph g_;
};

inline size_t Map::addVertex(int x, int y)
{
    return boost::add_vertex(GraphVertexPropertyMap(g_.m_vertices.size(), Map::maxDelTime_, x, y), g_);
}

inline void Map::removeVertex(size_t vertex)
{
    boost::clear_vertex(vertex, g_);
    boost::remove_vertex(vertex, g_);
}

inline Graph::edge_descriptor Map::addEdge(size_t srcVertex, size_t tgtVertex, int distance)
{
    return boost::add_edge(srcVertex, tgtVertex,
        GraphEdgePropertyMap(0, distance, distance / Map::avgSpeed_), g_).first;
}

inline void Map::removeEdge(size_t srcVertex, size_t tgtVertex)
{
    boost::remove_edge(srcVertex, tgtVertex, g_);
}

} // namespace ds

#endif // !MAP_HPP
