
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"gui_menuCommon.hpp"
#include"gui_menuMain.hpp"
#include"imgui.h"
#include<cstdlib>
#include<filesystem>
#include<limits>
#include<regex>
#include<sstream>

namespace ds {

using namespace std;

void guiMenuMain(bool* open, Map& map)
{
    ImGuiWindowFlags window_flags{ 0 };
    guiCommonInitialization(window_flags);
    string s;

    s = string{ u8"Main menu" } + u8"###MenuMain";
    if (ImGui::Begin(s.c_str(), nullptr, window_flags)) {
        const ImVec2 defaultScrolling   { 0.0f, 0.0f };
        //const float  defaultScaling     { 1.0f };
        //const float  defaultScalingStep { 0.1f };

        static bool optEnableGrid       { true };
        ImGui::Checkbox("Enable grid", &optEnableGrid);
        {
            ImGuiIO& io{ ImGui::GetIO() };
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }

        ImVec2 canvasP0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
        ImVec2 canvasSize = ImGui::GetContentRegionAvail(); // Resize canvas to what's available
        if (canvasSize.x < 50.0f) canvasSize.x = 50.0f;
        if (canvasSize.y < 50.0f) canvasSize.y = 50.0f;
        ImVec2 canvasP1 = ImVec2(canvasP0.x + canvasSize.x, canvasP0.y + canvasSize.y);

        // draw border and background color of the canvas
        ImGuiIO& io{ ImGui::GetIO() };
        ImDrawList* drawList{ ImGui::GetWindowDrawList() };
        drawList->AddRectFilled(canvasP0, canvasP1, ImGui::GetColorU32(color::grey20));
        drawList->AddRect(canvasP0, canvasP1, ImGui::GetColorU32(color::white));

        // canvas == big invisible button
        ImGui::InvisibleButton("Canvas", canvasSize,
            ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        const bool isHoveredCanvas{ ImGui::IsItemHovered() };
        static ImVec2 scrolling{ defaultScrolling };
        const ImVec2 origin{ canvasP0.x + scrolling.x, canvasP0.y + scrolling.y }; // Lock scrolled origin
        const ImVec2 mousePosInCanvas{ io.MousePos.x - origin.x, io.MousePos.y - origin.y };

        // actions on the canvas
        const auto& vertices{ map.graph().m_vertices };
        static bool addingEdge{ false };
        static int vertex       { numeric_limits<int>::max() };
        static int vertexPrev   { 0 };
        int        vertexCur    { numeric_limits<int>::max() };
        static bool isHoveredVertexPrev { false };
        bool        isHoveredVertexCur  { false };
        static ImVec2 vertexPos { 0.0f, 0.0f };
        static ImVec2 edgeP1    { 0.0f, 0.0f };
        static ImVec2 edgeP2    { 0.0f, 0.0f };
        static int edgeSrcVertex{ 0 };
        static int edgeTgtVertex{ numeric_limits<int>::max() };
        if (isHoveredCanvas) {
            ImGui::PushID(u8"VertextNumber");
            for (int i = 0; i < vertices.size(); ++i) {
                ImGui::PushID(i);
                // vertex == invisible button in same place
                const ImVec2 invButtonSize{ float(Map::vertexRadius_ * 2), float(Map::vertexRadius_ * 2) };
                const ImVec2 screenPos{
                    origin.x + vertices[i].m_property.x_ - Map::vertexRadius_,
                    origin.y + vertices[i].m_property.y_ - Map::vertexRadius_
                };
                ImGui::SetCursorScreenPos(screenPos);
                ImGui::InvisibleButton(u8"v", invButtonSize,
                    ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
                // vertex description
                ostringstream oss;
                oss << u8"Vertex " << i << endl
                    << u8"Coordinates X,Y: " << vertices[i].m_property.x_ << ','
                    << vertices[i].m_property.y_ << endl;
                for (int j = 0; j < vertices[i].m_out_edges.size(); ++j) {
                    oss << u8"Edge " << j << u8" (distance,time): " << i << u8" -> "
                        << vertices[i].m_out_edges[j].m_target
                        << u8" (" << vertices[i].m_out_edges[j].get_property().distance_
                        << ',' << vertices[i].m_out_edges[j].get_property().time_ << ')' << endl;
                }
                // if vertex is hovered, show description
                if (ImGui::IsItemHovered()) {
                    vertexCur = i;
                    isHoveredVertexCur = true;

                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::TextUnformatted(oss.str().c_str());
                    ImGui::PopTextWrapPos();
                    ImGui::EndTooltip();
                }
                // start drawing an edge
                if (isHoveredVertexPrev && !addingEdge && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    edgeSrcVertex = vertexPrev;
                    edgeTgtVertex = numeric_limits<int>::max();
                    edgeP1 = ImVec2{
                        float(vertices[edgeSrcVertex].m_property.x_),
                        float(vertices[edgeSrcVertex].m_property.y_)
                    };
                    edgeP2 = mousePosInCanvas;
                    addingEdge = true;
                }
                ImGui::PopID();
            }
            ImGui::PopID();
            // add edge
            if (addingEdge) {
                edgeP2 = mousePosInCanvas;
                edgeTgtVertex = vertexCur;
                if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    if (edgeTgtVertex != edgeSrcVertex &&
                        edgeTgtVertex != numeric_limits<int>::max())
                    {
                        // check that the edge does not exist
                        bool isExist{ false };
                        for (int i = 0; i < vertices[edgeSrcVertex].m_out_edges.size(); ++i) {
                            if (edgeTgtVertex == vertices[edgeSrcVertex].m_out_edges[i].m_target) {
                                isExist = true;
                            }
                        }
                        if (!isExist) {
                            // find distance between points and add the edge
                            const int xDiff{ vertices[edgeSrcVertex].m_property.x_ -
                                             vertices[edgeTgtVertex].m_property.x_ };
                            const int yDiff{ vertices[edgeSrcVertex].m_property.y_ -
                                             vertices[edgeTgtVertex].m_property.y_ };
                            const int distance = std::sqrt(xDiff * xDiff + yDiff * yDiff) * Map::scale_;
                            map.addEdge(edgeSrcVertex, edgeTgtVertex, distance);
                        }
                    }
                    addingEdge = false;
                }
            }
            // scrolling canvas
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
                scrolling.x += io.MouseDelta.x;
                scrolling.y += io.MouseDelta.y;
            }
            // open context menu
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                if (isHoveredVertexPrev) {
                    vertex = vertexPrev;
                    ImGui::OpenPopup(u8"ContextVertexPopup");
                }
                else {
                    vertexPos = mousePosInCanvas;
                    ImGui::OpenPopup(u8"ContextCanvasPopup");
                }
            }
        }

        // context menu
        if (ImGui::BeginPopup(u8"ContextCanvasPopup")) {
            ostringstream oss;
            oss << u8"Coordinates X,Y: " << vertexPos.x << ',' << vertexPos.y;
            ImGui::TextUnformatted(oss.str().c_str());
            ImGui::Separator();
            if (ImGui::MenuItem(u8"Add vertex here")) {
                map.addVertex(vertexPos.x, vertexPos.y);
            }
            if (ImGui::MenuItem(u8"Reset scrolling")) {
                scrolling = defaultScrolling;
            }
            ImGui::EndPopup();
        }
        if (ImGui::BeginPopup(u8"ContextVertexPopup")) {
            if (ImGui::MenuItem(u8"Remove vertex")) {
                if (vertex != numeric_limits<int>::max()) {
                    map.removeVertex(vertex);
                    vertex = numeric_limits<int>::max();
                }
            }
            if (ImGui::BeginMenu(u8"Remove edge")){
                for (int i = 0; i < vertices[vertex].m_out_edges.size(); ++i) {
                    ostringstream oss;
                    oss << u8"Remove edge " << i << u8": " << vertex << u8" -> "
                        << vertices[vertex].m_out_edges[i].m_target;
                    if (ImGui::MenuItem(oss.str().c_str())) {
                        map.removeEdge(vertex, vertices[vertex].m_out_edges[i].m_target);
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }

        vertexPrev = vertexCur;
        isHoveredVertexPrev = isHoveredVertexCur;


        drawList->PushClipRect(canvasP0, canvasP1, true);
        // draw grid of the canvas
        if (optEnableGrid) {
            const float gridStep{ 100.0f };
            for (float x{ fmodf(scrolling.x, gridStep) }; x < canvasSize.x; x += gridStep) {
                drawList->AddLine(ImVec2{ canvasP0.x + x, canvasP0.y },
                                  ImVec2{ canvasP0.x + x, canvasP1.y },
                                  ImGui::GetColorU32(color::grey30));
            }
            for (float y{ fmodf(scrolling.y, gridStep) }; y < canvasSize.y; y += gridStep) {
                drawList->AddLine(ImVec2{ canvasP0.x, canvasP0.y + y },
                                  ImVec2{ canvasP1.x, canvasP0.y + y },
                                  ImGui::GetColorU32(color::grey30));
            }
        }
        // draw vertices
        ImU32 graphColor{ ImGui::GetColorU32(color::yellow) };
        for (int i = 0; i < vertices.size(); ++i) {
            drawList->AddCircleFilled(
                ImVec2{ origin.x + vertices[i].m_property.x_,
                        origin.y + vertices[i].m_property.y_ },
                Map::vertexRadius_, graphColor, 4);
        }
        // draw edges
        for (int i = 0; i < vertices.size(); ++i) {
            for (int j = 0; j < vertices[i].m_out_edges.size(); ++j) {
                const ImVec2 srcPoint{
                    origin.x + vertices[i].m_property.x_,
                    origin.y + vertices[i].m_property.y_
                };
                const ImVec2 tgtPoint{
                    origin.x + vertices[vertices[i].m_out_edges[j].m_target].m_property.x_,
                    origin.y + vertices[vertices[i].m_out_edges[j].m_target].m_property.y_
                };
                drawList->AddLine(srcPoint, tgtPoint, graphColor, float(Map::edgeWidth_));
                // draw the direction of the edge as a point close to the target point
                const float t{ 0.9f };
                const ImVec2 directionPoint{
                    (1 - t) * srcPoint.x + t * tgtPoint.x,
                    (1 - t) * srcPoint.y + t * tgtPoint.y
                };
                drawList->AddCircleFilled(directionPoint, float(Map::vertexRadius_ * 0.6f), graphColor);
            }
        }
        // draw the edge being drawn at the moment
        if (addingEdge) {
            drawList->AddLine(
                ImVec2{ origin.x + edgeP1.x, origin.y + edgeP1.y },
                ImVec2{ origin.x + edgeP2.x, origin.y + edgeP2.y },
                graphColor, float(Map::edgeWidth_));
        }
        drawList->PopClipRect();
    }
    ImGui::End();
}

} // namespace ds
