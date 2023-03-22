
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"gui_menuCommon.hpp"
#include"gui_menuMain.hpp"
#include"imgui.h"
#include"options.hpp"
#include<cstdlib>
#include<filesystem>
#include<limits>
#include<regex>
#include<sstream>

namespace ds {

using namespace std;

void guiMenuMain(bool* open, ManagmentSystem& ms)
{
    ImGuiWindowFlags window_flags{ 0 };
    guiCommonInitialization(window_flags);
    string s;

    s = string{ u8"Main menu" } + u8"###MenuMain";
    if (ImGui::Begin(s.c_str(), nullptr, window_flags)) {
        // menu bar
        static bool optEnableGrid{ true };
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                ImGuiIO& io{ ImGui::GetIO() };
                ImGui::Text("%.1f FPS (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);
                ImGui::SliderInt("Frames per second (FPS)",
                    &Options::instance().fps_, Options::minFPS_,
                    Options::maxFPS_, "%d", ImGuiSliderFlags_AlwaysClamp);
                ImGui::SliderInt("Time speed (multiplier)",
                    &Options::instance().timeSpeed_, Options::minTimeSpeed_,
                    Options::maxTimeSpeed_, "%d", ImGuiSliderFlags_AlwaysClamp);

                ImGui::Separator();
                ImGui::SliderInt("Delivery and payment time (sec)",
                    &Options::instance().paymentTime_, Options::minPaymentTime_,
                    Options::maxPaymentTime_, "%d", ImGuiSliderFlags_AlwaysClamp);
                ImGui::SliderInt("Time to check free couriers (sec)",
                    &Options::instance().checkTimeFreeCour_, Options::minCheckTimeFreeCour_,
                    Options::maxCheckTimeFreeCour_, "%d", ImGuiSliderFlags_AlwaysClamp);

                ImGui::Separator();
                ImGui::Checkbox("Enable grid", &optEnableGrid);
                ImGui::SliderInt("Vertex radius (pixels)",
                    &Options::instance().vertexRadius_, Options::minVertexRadius_,
                    Options::maxVertexRadius_, "%d", ImGuiSliderFlags_AlwaysClamp);
                ImGui::SliderInt("Edge width (pixels)",
                    &Options::instance().edgeWidth_, Options::minEdgeWidth_,
                    Options::maxEdgeWidth_, "%d", ImGuiSliderFlags_AlwaysClamp);

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        const ImVec2 defaultScrolling   { 0.0f, 0.0f };
        ImVec2 contentRegionSize{ ImGui::GetContentRegionAvail() };
        const ImGuiStyle& style{ ImGui::GetStyle() };
        ImVec2 infoColumnSize{
            contentRegionSize.x / 4.0f - style.ItemSpacing.x,
            contentRegionSize.y - style.ItemSpacing.y
        };
        ImVec2 canvasSize{
            contentRegionSize.x / 4.0f * 3.0f - style.ItemSpacing.x,
            contentRegionSize.y - style.ItemSpacing.y
        };

        // draw INFO panel
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        ImGuiWindowFlags flags{ ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse };
        if (ImGui::BeginChild("Info", infoColumnSize, false, flags)) {
            ImGui::PopStyleVar();
            const unsigned int width{ 8 };
            const char filler{ '0' };

            ImGui::TextUnformatted("Orders in the \"Cooking\" status (top - last)");
            if (ImGui::BeginListBox("Cooking orders",
                ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing() + 2)))
            {
                auto cookOrders{ ms.kitchen().getOrders() };
                vector<Order*>::const_iterator iter{ cookOrders.second };
                while (iter != cookOrders.first) {
                    --iter;
                    ostringstream oss;
                    oss << setw(width) << setfill(filler) << cmn::toUnderlying((*iter)->getID())
                        << " - " << toString((*iter)->getStatus());
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }

            ImGui::TextUnformatted("Orders with the \"Delivery\" status (top - last)");
            if (ImGui::BeginListBox("Delivery orders",
                ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing() + 2)))
            {
                auto delivOrders{ ms.delivery().getOrders() };
                vector<Order*>::const_iterator iter{ delivOrders.second };
                while (iter != delivOrders.first) {
                    --iter;
                    ostringstream oss;
                    oss << setw(width) << setfill(filler) << cmn::toUnderlying((*iter)->getID())
                        << " - " << toString((*iter)->getStatus());
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }

            ImGui::TextUnformatted("Orders with the \"Completed\" status (top - last)");
            if (ImGui::BeginListBox("Completed orders",
                ImVec2(-FLT_MIN, Options::numComplOrders_ * ImGui::GetTextLineHeightWithSpacing() + 2)))
            {
                auto complOrders{ ms.scheduler().getOrdersCompleted() };
                boost::circular_buffer<Order*>::const_iterator iter{ complOrders.second };
                while (iter != complOrders.first) {
                    --iter;
                    ostringstream oss;
                    oss << setw(width) << setfill(filler) << cmn::toUnderlying((*iter)->getID())
                        << " - " << toString((*iter)->getStatus());
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }
        }
        else {
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImVec2 canvasP0{ ImGui::GetCursorScreenPos() };         // ImDrawList API uses screen coordinates!
        if (canvasSize.x < 50.0f) canvasSize.x = 50.0f;
        if (canvasSize.y < 50.0f) canvasSize.y = 50.0f;
        ImVec2 canvasP1{ ImVec2(canvasP0.x + canvasSize.x, canvasP0.y + canvasSize.y) };

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
        const auto& vertices{ ms.map().graph().m_vertices };
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
                const ImVec2 invButtonSize{
                    float(Options::instance().vertexRadius_ * 2),
                    float(Options::instance().vertexRadius_ * 2)
                };
                const ImVec2 screenPos{
                    origin.x + vertices[i].m_property.x_ - Options::instance().vertexRadius_,
                    origin.y + vertices[i].m_property.y_ - Options::instance().vertexRadius_
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
                            ms.map().addEdge(edgeSrcVertex, edgeTgtVertex, distance);
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
                ms.map().addVertex(vertexPos.x, vertexPos.y);
            }
            if (ImGui::MenuItem(u8"Reset scrolling")) {
                scrolling = defaultScrolling;
            }
            ImGui::EndPopup();
        }
        if (ImGui::BeginPopup(u8"ContextVertexPopup")) {
            if (ImGui::MenuItem(u8"Remove vertex")) {
                if (vertex != numeric_limits<int>::max()) {
                    ms.map().removeVertex(vertex);
                    vertex = numeric_limits<int>::max();
                }
            }
            if (ImGui::BeginMenu(u8"Remove edge")){
                for (int i = 0; i < vertices[vertex].m_out_edges.size(); ++i) {
                    ostringstream oss;
                    oss << u8"Remove edge " << i << u8": " << vertex << u8" -> "
                        << vertices[vertex].m_out_edges[i].m_target;
                    if (ImGui::MenuItem(oss.str().c_str())) {
                        ms.map().removeEdge(vertex, vertices[vertex].m_out_edges[i].m_target);
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
                Options::instance().vertexRadius_, graphColor, 4);
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
                drawList->AddLine(srcPoint, tgtPoint, graphColor,
                    float(Options::instance().edgeWidth_));
                // draw the direction of the edge as a point close to the target point
                const float t{ 0.9f };
                const ImVec2 directionPoint{
                    (1 - t) * srcPoint.x + t * tgtPoint.x,
                    (1 - t) * srcPoint.y + t * tgtPoint.y
                };
                drawList->AddCircleFilled(directionPoint,
                    int(Options::instance().vertexRadius_ * 0.6f), graphColor);
            }
        }
        // draw the edge being drawn at the moment
        if (addingEdge) {
            drawList->AddLine(
                ImVec2{ origin.x + edgeP1.x, origin.y + edgeP1.y },
                ImVec2{ origin.x + edgeP2.x, origin.y + edgeP2.y },
                graphColor, float(Options::instance().edgeWidth_));
        }
        // draw courier location
        auto couriers{ ms.getCouriers() };
        for (auto iter{ couriers.first }; iter != couriers.second; ++iter) {
            ImVec2 courierLocation{ iter->get()->getLocation() };
            courierLocation.x += origin.x;
            courierLocation.y += origin.y;
            drawList->AddCircleFilled(courierLocation, 8, ImGui::GetColorU32(color::turquoise), 6);
        }

        drawList->PopClipRect();
    }
    ImGui::End();
}

} // namespace ds
