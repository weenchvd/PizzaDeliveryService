
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

enum nodes
{
    A, B, C, D, E, F, G, H, NUMBER_OF
};
char name[] = "ABCDEFGH";

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
}

} // namespace ds
