
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef MAP_HPP
#define MAP_HPP

#include"options.hpp"
#include<algorithm>
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/graph_traits.hpp>
#include<boost/graph/r_c_shortest_paths.hpp>
#include<chrono>

namespace ds {

struct GraphVertexPropertyMap {
    GraphVertexPropertyMap(int number = 0, int x = 0, int y = 0)
        : num_{ number }, x_{ x }, y_{ y } {}

    int num_;                               // property number
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

struct GraphRC2 {
    GraphRC2(
        const std::vector<Graph::vertex_descriptor>& tgtVertices,
        const std::vector<std::chrono::seconds>& remainingTime,
        int distance,
        int time)
        :
        tgtVertices_    { tgtVertices },
        remainingTime_  { remainingTime },
        visited_        {},
        distance_       { distance },
        time_           { time },
        tgt0InPath_     { false }
    {
        assert(tgtVertices_.size() == remainingTime_.size());
        assert(tgtVertices_.size() > 0);
    }

    const std::vector<Graph::vertex_descriptor>&    tgtVertices_;
    const std::vector<std::chrono::seconds>&        remainingTime_; // remaining time to deliver
    std::vector<size_t>                             visited_;       // indexes of visited vertices in 'tgtVertices_'
    int                                             distance_;      // meters
    int                                             time_;          // seconds
    bool                                            tgt0InPath_;    // the 1st target vertex ('tgtVertices_[0]') is on the path
};

bool operator==(const GraphRC2& rc1, const GraphRC2& rc2);

bool operator<(const GraphRC2& rc1, const GraphRC2& rc2);

// ResourceExtensionFunction model
class GraphREF {
public:
    inline bool operator()(const Graph& g, GraphRC& newRC, const GraphRC& oldRC,
        boost::graph_traits<Graph>::edge_descriptor ed) const
    {
        newRC.distance_ = oldRC.distance_ + g[ed].distance_;
        newRC.time_     = oldRC.time_     + g[ed].time_;
        return newRC.time_ <= Options::instance().optDelivery_.deliveryTime_;
    }
};

class GraphREF2 {
public:
    inline bool operator()(const Graph& g, GraphRC2& newRC, const GraphRC2& oldRC,
        boost::graph_traits<Graph>::edge_descriptor ed) const
    {
        newRC.distance_ = oldRC.distance_ + g[ed].distance_;
        newRC.time_     = oldRC.time_     + g[ed].time_;
        for (int i = 0; i < newRC.tgtVertices_.size(); ++i) {
            if (newRC.tgtVertices_[i] == g[ed.m_target].num_ &&
                find(newRC.visited_.cbegin(), newRC.visited_.cend(), i) == newRC.visited_.cend())
            {
                if (i == 0) {
                    newRC.tgt0InPath_ = true;
                }
                if (i > 0 && newRC.time_ > newRC.remainingTime_[i].count()) {
                    continue;
                }
                newRC.visited_.push_back(i);
                newRC.time_ += Options::instance().optCourier_.deliveryTime_;
                newRC.time_ += Options::instance().optCourier_.paymentTime_;
            }
        }
        return newRC.time_ <= Options::instance().optDelivery_.deliveryTime_ * 2;
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

class GraphDF2 {
public:
    inline bool operator()(const GraphRC2& rc1, const GraphRC2& rc2) const
    {
        return rc1 < rc2 || rc1 == rc2;
    }
};


struct MapPath {
    std::vector<Graph::edge_descriptor>     path_;
    std::vector<size_t>                     visited_;           // indexes of visited vertices in 'tgtVertices_'
};

class Map {
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

    std::vector<Graph::edge_descriptor> getPath(size_t srcVertex, size_t tgtVertex);

    MapPath getPath(const Graph::vertex_descriptor srcVertex,
                    const std::vector<Graph::vertex_descriptor>& tgtVertices,
                    const std::vector<std::chrono::seconds>& remainingTime);

private:
    Graph g_;
};

inline size_t Map::addVertex(int x, int y)
{
    return boost::add_vertex(GraphVertexPropertyMap(g_.m_vertices.size(), x, y), g_);
}

inline void Map::removeVertex(size_t vertex)
{
    boost::clear_vertex(vertex, g_);
    boost::remove_vertex(vertex, g_);
}

inline Graph::edge_descriptor Map::addEdge(size_t srcVertex, size_t tgtVertex, int distance)
{
    return boost::add_edge(srcVertex, tgtVertex, GraphEdgePropertyMap(
        g_.m_edges.size(), distance, distance / OptionsCourier::defAverageSpeed_), g_).first;
}

inline void Map::removeEdge(size_t srcVertex, size_t tgtVertex)
{
    boost::remove_edge(srcVertex, tgtVertex, g_);
}

} // namespace ds

#endif // !MAP_HPP
