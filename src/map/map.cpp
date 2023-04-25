
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"map.hpp"

namespace ds {

using namespace std;

bool operator==(const GraphRC& rc1, const GraphRC& rc2)
{
    return rc1.distance_ == rc2.distance_
        && rc1.time_     == rc2.time_;
}

bool operator<(const GraphRC& rc1, const GraphRC& rc2)
{
    if (rc1.time_ >  rc2.time_) return false;
    if (rc1.time_ == rc2.time_) return rc1.distance_ < rc2.distance_;
    return true;
}

bool operator==(const GraphRC2& rc1, const GraphRC2& rc2)
{
    return rc1.tgt0InPath_     == rc2.tgt0InPath_
        && rc1.visited_.size() == rc2.visited_.size()
        && rc1.distance_       == rc2.distance_
        && rc1.time_           == rc2.time_;
}

bool operator<(const GraphRC2& rc1, const GraphRC2& rc2)
{
    if (rc1.tgt0InPath_ == false && rc2.tgt0InPath_ == true) return false;
    if (rc1.tgt0InPath_ == true && rc2.tgt0InPath_ == false) return true;
    if (rc1.visited_.size() < rc2.visited_.size()) return false;
    if (rc1.visited_.size() > rc2.visited_.size()) return true;
    if (rc1.time_ > rc2.time_) return false;
    if (rc1.time_ < rc2.time_) return true;
    if (rc1.distance_ > rc2.distance_) return false;
    if (rc1.distance_ < rc2.distance_) return true;
    return false;
}

int calcDistance(const Map& map, int edgeSrcVertex, int edgeTgtVertex)
{
    const int xDiff{ map.graph().m_vertices[edgeSrcVertex].m_property.x_ -
                     map.graph().m_vertices[edgeTgtVertex].m_property.x_ };
    const int yDiff{ map.graph().m_vertices[edgeSrcVertex].m_property.y_ -
                     map.graph().m_vertices[edgeTgtVertex].m_property.y_ };
    return std::sqrt(xDiff * xDiff + yDiff * yDiff) * Options::instance().optMap_.scale_;
}

Map::Map()
    : g_{}
{
    addVertex(0, 0);
    addVertex(200, 0);
    addVertex(400, 100);
    addVertex(200, 200);
    addVertex(0, 200);
    addVertex(100, 100);
    addVertex(150, 50);
    addVertex(250, 50);

    int distance{ 0 };
    distance = calcDistance(*this, 0, 1);
    addEdge(0, 1, distance);
    distance = calcDistance(*this, 1, 2);
    addEdge(1, 2, distance);
    distance = calcDistance(*this, 2, 3);
    addEdge(2, 3, distance);
    distance = calcDistance(*this, 3, 4);
    addEdge(3, 4, distance);
    distance = calcDistance(*this, 4, 0);
    addEdge(4, 0, distance);
    distance = calcDistance(*this, 0, 5);
    addEdge(0, 5, distance);
    distance = calcDistance(*this, 5, 2);
    addEdge(5, 2, distance);
    distance = calcDistance(*this, 5, 3);
    addEdge(5, 3, distance);
    distance = calcDistance(*this, 1, 3);
    addEdge(1, 3, distance);
    distance = calcDistance(*this, 0, 6);
    addEdge(0, 6, distance);
    addEdge(6, 0, distance);
    distance = calcDistance(*this, 1, 6);
    addEdge(1, 6, distance);
    distance = calcDistance(*this, 6, 5);
    addEdge(6, 5, distance);
    distance = calcDistance(*this, 6, 7);
    addEdge(6, 7, distance);
    distance = calcDistance(*this, 7, 1);
    addEdge(7, 1, distance);
}

std::vector<Graph::edge_descriptor> Map::getPath(size_t srcVertex, size_t tgtVertex)
{
    vector<vector<Graph::edge_descriptor>> optSolutions;
    vector<GraphRC> paretoOptRCS;
    r_c_shortest_paths(g_, get(&GraphVertexPropertyMap::num_, g_),
        get(&GraphEdgePropertyMap::num_, g_), srcVertex, tgtVertex,
        optSolutions, paretoOptRCS, GraphRC{ 0, 0 }, GraphREF{}, GraphDF{},
        std::allocator<boost::r_c_shortest_paths_label<Graph, GraphRC>>(),
        boost::default_r_c_shortest_paths_visitor());

    if (optSolutions.size() < 1) throw;
    vector<Graph::edge_descriptor> path{};
    path.reserve(optSolutions[0].size());
    for (int i = optSolutions[0].size() - 1; i >= 0; --i) {
        path.push_back(optSolutions[0][i]);
    }
    return path;
}

MapPath Map::getPath(const Graph::vertex_descriptor srcVertex,
                     const vector<Graph::vertex_descriptor>& tgtVertices,
                     const vector<chrono::seconds>& remainingTime)
{
    assert(tgtVertices.size() == remainingTime.size());
    assert(tgtVertices.empty() == false);
    MapPath mp;
    vector<vector<Graph::edge_descriptor>> optSolutions;
    vector<GraphRC2> paretoOptRCs;
    r_c_shortest_paths(g_, get(&GraphVertexPropertyMap::num_, g_),
        get(&GraphEdgePropertyMap::num_, g_),
        srcVertex, srcVertex,
        optSolutions, paretoOptRCs,
        GraphRC2{ tgtVertices, remainingTime, 0, 0 }, GraphREF2{}, GraphDF2{},
        std::allocator<boost::r_c_shortest_paths_label<Graph, GraphRC2>>(),
        boost::default_r_c_shortest_paths_visitor());

    if (optSolutions.empty() == true || optSolutions[0].empty() == true) {
        mp.path_ = getPath(srcVertex, tgtVertices[0]);
        mp.visited_.push_back(0);
        return mp;
    }
    const Graph::vertex_descriptor lastVisited{
        tgtVertices[paretoOptRCs[0].visited_[paretoOptRCs[0].visited_.size() - 1]]
    };
    int i{ 0 };
    for (; i < optSolutions[0].size(); ++i) {
        if (lastVisited == optSolutions[0][i].m_source) {
            mp.path_.reserve(optSolutions[0].size() - (i + 1));
            break;
        }
    }
    for (int j = optSolutions[0].size() - 1; j > i; --j) {
        mp.path_.push_back(optSolutions[0][j]);
    }
    mp.visited_ = std::move(paretoOptRCs[0].visited_);
    return mp;
}

} // namespace ds
