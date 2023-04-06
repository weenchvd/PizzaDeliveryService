
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

int calcDistance(const Map& map, int edgeSrcVertex, int edgeTgtVertex)
{
    const int xDiff{ map.graph().m_vertices[edgeSrcVertex].m_property.x_ -
                     map.graph().m_vertices[edgeTgtVertex].m_property.x_ };
    const int yDiff{ map.graph().m_vertices[edgeSrcVertex].m_property.y_ -
                     map.graph().m_vertices[edgeTgtVertex].m_property.y_ };
    return std::sqrt(xDiff * xDiff + yDiff * yDiff) * Map::scale_;
}

Map::Map()
    : g_{}
{
    using namespace boost;
    int numVPM{ 0 };
    add_vertex(GraphVertexPropertyMap(numVPM++, Map::maxDelTime_, 0, 0), g_);
    add_vertex(GraphVertexPropertyMap(numVPM++, Map::maxDelTime_, 200, 0), g_);
    add_vertex(GraphVertexPropertyMap(numVPM++, Map::maxDelTime_, 400, 100), g_);
    add_vertex(GraphVertexPropertyMap(numVPM++, Map::maxDelTime_, 200, 200), g_);
    add_vertex(GraphVertexPropertyMap(numVPM++, Map::maxDelTime_, 0, 200), g_);
    add_vertex(GraphVertexPropertyMap(numVPM++, Map::maxDelTime_, 100, 100), g_);
    add_vertex(GraphVertexPropertyMap(numVPM++, Map::maxDelTime_, 150, 50), g_);
    add_vertex(GraphVertexPropertyMap(numVPM++, Map::maxDelTime_, 250, 50), g_);

    int numEPM{ 0 };
    int distance{ 0 };
    distance = calcDistance(*this, 0, 1);
    add_edge(0, 1, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 1, 2);
    add_edge(1, 2, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 2, 3);
    add_edge(2, 3, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 3, 4);
    add_edge(3, 4, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 4, 0);
    add_edge(4, 0, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 0, 5);
    add_edge(0, 5, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 5, 2);
    add_edge(5, 2, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 5, 3);
    add_edge(5, 3, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 1, 3);
    add_edge(1, 3, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 0, 6);
    add_edge(0, 6, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    add_edge(6, 0, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 1, 6);
    add_edge(1, 6, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 6, 5);
    add_edge(6, 5, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 6, 7);
    add_edge(6, 7, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
    distance = calcDistance(*this, 7, 1);
    add_edge(7, 1, GraphEdgePropertyMap(numEPM++, distance, distance / Map::avgSpeed_), g_);
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

} // namespace ds
