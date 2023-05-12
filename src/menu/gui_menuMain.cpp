
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
#include<iterator>
#include<limits>
#include<regex>
#include<sstream>

namespace ds {

using namespace std;

void guiMenuMain_Submenu(SubmenuType& submenu)
{
    const auto& style{ ImGui::GetStyle() };
    ImGuiTableFlags_ tableFlags{ static_cast<ImGuiTableFlags_>(
        ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchSame
    ) };
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 0.0f, 0.0f });
    if (ImGui::BeginTable(u8"Submenus", cmn::numberOf<SubmenuType>(), tableFlags)) {
        const ImVec2 buttonSize{
            ImGui::GetWindowContentRegionMax().x / cmn::numberOf<SubmenuType>(),
            ImGui::GetFrameHeight()
        };

        bool isActiveSubmenu{ false };
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        isActiveSubmenu = (submenu == SubmenuType::COMMON) ? true : false;
        if (isActiveSubmenu) {
            ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonActive]);
        }
        if (ImGui::Button(u8"Common", buttonSize)) {
            submenu = SubmenuType::COMMON;
        }
        if (isActiveSubmenu) {
            ImGui::PopStyleColor();
        }

        ImGui::TableNextColumn();
        isActiveSubmenu = (submenu == SubmenuType::KITCHEN) ? true : false;
        if (isActiveSubmenu) {
            ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonActive]);
        }
        if (ImGui::Button(u8"Kitchen", buttonSize)) {
            submenu = SubmenuType::KITCHEN;
        }
        if (isActiveSubmenu) {
            ImGui::PopStyleColor();
        }

        ImGui::TableNextColumn();
        isActiveSubmenu = (submenu == SubmenuType::DELIVERY) ? true : false;
        if (isActiveSubmenu) {
            ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonActive]);
        }
        if (ImGui::Button(u8"Delivery", buttonSize)) {
            submenu = SubmenuType::DELIVERY;
        }
        if (isActiveSubmenu) {
            ImGui::PopStyleColor();
        }

        static_assert(cmn::numberOf<SubmenuType>() == 3);
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
}

void guiMenuMain(bool* open, ManagmentSystem& ms)
{
    static SubmenuType submenu{ SubmenuType::COMMON };

    bool showGuiMenuCommon  { false };
    bool showGuiMenuKitchen { false };
    bool showGuiMenuDelivery{ false };

    switch (submenu) {
    case SubmenuType::COMMON:
        showGuiMenuCommon   = true;
        guiMenuCommon(&showGuiMenuCommon, ms, submenu);
        break;
    case SubmenuType::KITCHEN:
        showGuiMenuKitchen  = true;
        guiMenuKitchen(&showGuiMenuKitchen, ms, submenu);
        break;
    case SubmenuType::DELIVERY:
        showGuiMenuDelivery = true;
        guiMenuDelivery(&showGuiMenuDelivery, ms, submenu);
        break;
    }

    if (!(showGuiMenuCommon || showGuiMenuKitchen || showGuiMenuDelivery)) {
        submenu = SubmenuType::COMMON;
        *open = false;
    }
    static_assert(cmn::numberOf<SubmenuType>() == 3);
}

void guiMenuCommon(bool* open, ManagmentSystem& ms, SubmenuType& submenu)
{
    ImGuiWindowFlags window_flags{ 0 };
    guiCommonInitialization(window_flags);
    string s;

    s = string{ u8"Common" } + u8"###MenuCommon";
    if (ImGui::Begin(s.c_str(), nullptr, window_flags)) {
        guiMenuBarOptions(ms);

        guiMenuMain_Submenu(submenu);

        const unsigned int qtyColumn{ 4 };
        const ImVec2 defaultScrolling{ 0.0f, 0.0f };
        const ImVec2 contentRegionSize{ ImGui::GetContentRegionAvail() };
        const ImGuiStyle& style{ ImGui::GetStyle() };
        static_assert(qtyColumn > 1);
        const ImVec2 columnSize{
            (contentRegionSize.x - style.ItemSpacing.x * (qtyColumn - 1)) / qtyColumn,
            contentRegionSize.y
        };
        ImVec2 canvasSize{
            columnSize.x * (qtyColumn - 1) + style.ItemSpacing.x * (qtyColumn - 2),
            columnSize.y
        };

        // draw INFO panel
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        ImGuiWindowFlags flags{ ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse };
        if (ImGui::BeginChild("Info", columnSize, false, flags)) {
            ImGui::PopStyleVar();
            const unsigned int qtyListBox{ 3 };
            const ImVec2 sizeListBox{
                -FLT_MIN,
                columnSize.y / qtyListBox - ImGui::GetTextLineHeightWithSpacing()
            };
            const unsigned int width{ 8 };
            const char filler{ '0' };

            ImGui::TextUnformatted("Orders in the \"Cooking\" status (top - first)");
            if (ImGui::BeginListBox("Cooking orders", sizeListBox)) {
                auto cookOrders{ ms.kitchen().getOrders() };
                for (auto iter{ cookOrders.first }; iter != cookOrders.second; ++iter) {
                    ostringstream oss;
                    oss << setw(width) << setfill(filler) << cmn::toUnderlying((*iter)->getID())
                        << " - " << toString((*iter)->getStatus());
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }

            ImGui::TextUnformatted("Orders with the \"Delivery\" status (top - first)");
            if (ImGui::BeginListBox("Delivery orders", sizeListBox)) {
                auto delivOrders{ ms.delivery().getOrders() };
                for (auto iter{ delivOrders.first }; iter != delivOrders.second; ++iter) {
                    ostringstream oss;
                    oss << setw(width) << setfill(filler) << cmn::toUnderlying((*iter)->getID())
                        << " - " << toString((*iter)->getStatus());
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }

            ImGui::TextUnformatted("Orders with the \"Completed\" status (top - last)");
            if (ImGui::BeginListBox("Completed orders", sizeListBox)) {
                auto complOrders{ ms.scheduler().getOrdersCompleted() };
                boost::circular_buffer<Order*>::const_iterator iter{ complOrders.second };
                while (iter != complOrders.first) {
                    --iter;
                    ostringstream oss;
                    oss << setw(width) << setfill(filler) << cmn::toUnderlying((*iter)->getID())
                        << " - " << toString((*iter)->getStatus());
                    if ((*iter)->getStatus() == OrderStatus::COMPLETED) {
                        oss << " in "
                            << cmn::getDuration((*iter)->getTimeEnd() - (*iter)->getTimeStart());
                    }
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
                    float(Options::instance().optMap_.vertexRadius_ * 2),
                    float(Options::instance().optMap_.vertexRadius_ * 2)
                };
                const ImVec2 screenPos{
                    origin.x + vertices[i].m_property.x_ - Options::instance().optMap_.vertexRadius_,
                    origin.y + vertices[i].m_property.y_ - Options::instance().optMap_.vertexRadius_
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
                            const int distance = std::sqrt(xDiff * xDiff + yDiff * yDiff)
                                * Options::instance().optMap_.scale_;
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
        if (Options::instance().optMap_.showGrid_) {
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
                Options::instance().optMap_.vertexRadius_, graphColor, 4);
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
                    float(Options::instance().optMap_.edgeWidth_));
                // draw the direction of the edge as a point close to the target point
                const float t{ 0.75f };
                const ImVec2 directionPoint{
                    (1 - t) * srcPoint.x + t * tgtPoint.x,
                    (1 - t) * srcPoint.y + t * tgtPoint.y
                };
                drawList->AddCircleFilled(directionPoint,
                    int(Options::instance().optMap_.vertexRadius_ * 0.6f), graphColor);
            }
        }
        // draw the edge being drawn at the moment
        if (addingEdge) {
            drawList->AddLine(
                ImVec2{ origin.x + edgeP1.x, origin.y + edgeP1.y },
                ImVec2{ origin.x + edgeP2.x, origin.y + edgeP2.y },
                graphColor, float(Options::instance().optMap_.edgeWidth_));
        }
        // draw courier location
        auto couriers{ ms.getCouriers() };
        for (auto iter{ couriers.first }; iter != couriers.second; ++iter) {
            ImVec2 courierLocation{ iter->get()->getCurrentLocation() };
            courierLocation.x += origin.x;
            courierLocation.y += origin.y;
            drawList->AddCircleFilled(courierLocation, 8, ImGui::GetColorU32(color::turquoise), 6);
        }

        drawList->PopClipRect();
    }
    ImGui::End();
}

void guiMenuKitchen(bool* open, ManagmentSystem& ms, SubmenuType& submenu)
{
    ImGuiWindowFlags window_flags{ 0 };
    guiCommonInitialization(window_flags);
    string s;

    s = string{ u8"Kitchen" } + u8"###MenuKitchen";
    if (ImGui::Begin(s.c_str(), nullptr, window_flags)) {
        guiMenuBarOptions(ms);

        guiMenuMain_Submenu(submenu);

        const unsigned int qtyColumn{ 4 };
        const ImVec2 contentRegionSize{ ImGui::GetContentRegionAvail() };
        const ImGuiStyle& style{ ImGui::GetStyle() };
        static_assert(qtyColumn > 0);
        const ImVec2 columnSize{
            (contentRegionSize.x - style.ItemSpacing.x * (qtyColumn - 1)) / qtyColumn,
            contentRegionSize.y
        };

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        ImGuiWindowFlags flags{ ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse };
        if (ImGui::BeginChild("Column1", columnSize, false, flags)) {
            ImGui::PopStyleVar();
            const unsigned int width{ 8 };
            const char filler{ '0' };

            ImGui::TextUnformatted("Orders in the \"Cooking\" status (top - first)");
            if (ImGui::BeginListBox("Cooking orders", ImVec2(-FLT_MIN, -FLT_MIN))) {
                auto cookOrders{ ms.kitchen().getOrders() };
                for (auto iter{ cookOrders.first }; iter != cookOrders.second; ++iter) {
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
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        if (ImGui::BeginChild("Column2", columnSize, false, flags)) {
            ImGui::PopStyleVar();
            const unsigned int qtyListBox{ 3 };
            const ImVec2 sizeListBox{
                -FLT_MIN,
                columnSize.y / qtyListBox - ImGui::GetTextLineHeightWithSpacing()
            };

            ImGui::TextUnformatted("Dough queue (top - first)");
            if (ImGui::BeginListBox("Dough queue", sizeListBox)) {
                auto foodQueueDough{ ms.kitchen().getQueueDough() };
                for (auto iter{ foodQueueDough.first }; iter != foodQueueDough.second; ++iter) {
                    ostringstream oss;
                    oss << toString((*iter)->getName()) << " - " << toString((*iter)->getStatus());
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }

            ImGui::TextUnformatted("Filling queue (top - first)");
            if (ImGui::BeginListBox("Filling queue", sizeListBox)) {
                auto foodQueueFilling{ ms.kitchen().getQueueFilling() };
                for (auto iter{ foodQueueFilling.first }; iter != foodQueueFilling.second; ++iter) {
                    ostringstream oss;
                    oss << toString((*iter)->getName()) << " - " << toString((*iter)->getStatus());
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }

            ImGui::TextUnformatted("Picker queue (top - first)");
            if (ImGui::BeginListBox("Picker queue", sizeListBox)) {
                auto foodQueuePicker{ ms.kitchen().getQueuePicker() };
                for (auto iter{ foodQueuePicker.first }; iter != foodQueuePicker.second; ++iter) {
                    ostringstream oss;
                    oss << toString((*iter)->getName()) << " - " << toString((*iter)->getStatus());
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }
        }
        else {
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();


        const ImVec2 column3Size{
            columnSize.x * 2 + style.ItemSpacing.x,
            columnSize.y
        };
        ImGui::SameLine();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        if (ImGui::BeginChild("Column3", column3Size, false, flags)) {
            ImGui::PopStyleVar();
            const unsigned int width{ 4 };
            const char filler{ '0' };
            auto kitcheners{ ms.getKitcheners() };
            ImGui::TextUnformatted("Kitcheners");
            if (ImGui::BeginListBox("Kitcheners", ImVec2(-FLT_MIN, -FLT_MIN))) {
                for (auto iter{ kitcheners.first }; iter != kitcheners.second; ++iter) {
                    ostringstream oss;
                    const auto kitchener{ iter->get() };
                    oss << "ID: " << setw(width) << setfill(filler)
                        << cmn::toUnderlying(kitchener->getID())
                        << " - " << toString(kitchener->getType())
                        << " - " << toString(kitchener->getStatus());
                    const auto food{ kitchener->getFood() };
                    if (food != nullptr) {
                        oss << " (" << toString(food->getName())
                            << " - OrderID: "
                            << cmn::toUnderlying(ms.kitchen().getOrder(food).getID()) << ")";
                    }
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }
        }
        else {
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void guiMenuDelivery(bool* open, ManagmentSystem& ms, SubmenuType& submenu)
{
    ImGuiWindowFlags window_flags{ 0 };
    guiCommonInitialization(window_flags);
    string s;

    s = string{ u8"Delivery" } + u8"###MenuDelivery";
    if (ImGui::Begin(s.c_str(), nullptr, window_flags)) {
        guiMenuBarOptions(ms);

        guiMenuMain_Submenu(submenu);

#if 1
        const ImGuiStyle& style{ ImGui::GetStyle() };
        const ImVec2 tlScrPos{ ImGui::GetCursorScreenPos() };
        const ImVec2 regionSize{ ImGui::GetContentRegionAvail() };
        const ImGuiTileMesh tileMesh{ tlScrPos, regionSize, Tile{ 4, 4 }, style.ItemSpacing.y };
        ImVec2 tilePos;
        ImVec2 tileSize;
        ImGuiWindowFlags flags;

        tilePos = tileMesh.getPos(Tile{ 1,1 });
        tileSize = tileMesh.getSize(Tile{ 1,2 });
        ImGui::SetCursorScreenPos(tilePos);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        if (ImGui::BeginChild("Tile1", tileSize, false, flags)) {
            ImGui::PopStyleVar();
            const unsigned int width{ 8 };
            const char filler{ '0' };

            ImGui::TextUnformatted("Orders with the \"Delivery\" status (top - first)");
            if (ImGui::BeginListBox("Delivery orders", ImVec2(-FLT_MIN, -FLT_MIN))) {
                auto delivOrders{ ms.delivery().getOrders() };
                for (auto iter{ delivOrders.first }; iter != delivOrders.second; ++iter) {
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


        tilePos = tileMesh.getPos(Tile{ 2,1 });
        tileSize = tileMesh.getSize(Tile{ 1,2 });
        ImGui::SetCursorScreenPos(tilePos);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        if (ImGui::BeginChild("Tile2", tileSize, false, flags)) {
            ImGui::PopStyleVar();
            const unsigned int width{ 8 };
            const char filler{ '0' };

            ImGui::TextUnformatted("Orders with the \"Completed\" status (top - last)");
            if (ImGui::BeginListBox("Completed orders", ImVec2(-FLT_MIN, -FLT_MIN))) {
                auto complOrders{ ms.scheduler().getOrdersCompleted() };
                boost::circular_buffer<Order*>::const_iterator iter{ complOrders.second };
                while (iter != complOrders.first) {
                    --iter;
                    ostringstream oss;
                    oss << setw(width) << setfill(filler) << cmn::toUnderlying((*iter)->getID())
                        << " - " << toString((*iter)->getStatus());
                    if ((*iter)->getStatus() == OrderStatus::COMPLETED) {
                        oss << " in "
                            << cmn::getDuration((*iter)->getTimeEnd() - (*iter)->getTimeStart());
                    }
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }
        }
        else {
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();


        tilePos = tileMesh.getPos(Tile{ 1,3 });
        tileSize = tileMesh.getSize(Tile{ 2,2 });
        ImGui::SetCursorScreenPos(tilePos);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
        flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        if (ImGui::BeginChild("Tile3", tileSize, false, flags)) {
            ImGui::PopStyleVar();
            const unsigned int width{ 4 };
            const char filler{ '0' };
            auto couriers{ ms.getCouriers() };

            ImGui::TextUnformatted("Couriers");
            if (ImGui::BeginListBox("Couriers", ImVec2(-FLT_MIN, -FLT_MIN))) {
                for (auto iter{ couriers.first }; iter != couriers.second; ++iter) {
                    ostringstream oss;
                    const auto courier{ iter->get() };
                    const auto status{ courier->getStatus() };
                    oss << "ID: " << setw(width) << setfill(filler)
                        << cmn::toUnderlying(courier->getID())
                        << " - " << toString(status);
                    if (status == CourierStatus::MOVEMENT_TO_CUSTOMER ||
                        status == CourierStatus::DELIVERY_AND_PAYMENT)
                    {
                        const auto curOrder{ courier->getCurrentOrder() };
                        oss << " (OrderID: "
                            << cmn::toUnderlying((*curOrder)->getID()) << ")";
                    }
                    ImGui::TextUnformatted(oss.str().c_str());
                }
                ImGui::EndListBox();
            }
        }
        else {
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();


#else
        {
            const ImGuiStyle& style{ ImGui::GetStyle() };
            const ImVec2 tlScrPos{ ImGui::GetCursorScreenPos() };
            const ImVec2 regionSize{ ImGui::GetContentRegionAvail() };
            const ImGuiTileMesh tileMesh{ tlScrPos, regionSize, Tile{ 4, 3 }, style.ItemSpacing.y };

            ImDrawList* drawList{ ImGui::GetWindowDrawList() };
            ImVec2 pos;
            ImVec2 size;

            drawList->AddRect(tlScrPos,
                ImVec2{ tlScrPos.x + regionSize.x, tlScrPos.y + regionSize.y },
                ImGui::GetColorU32(color::white));


#if 0
            pos = tileMesh.getPos(Tile{ 1,1 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::blue));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 2,1 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::grey));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 3,1 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::turquoise));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 4,1 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::yellow));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);



            pos = tileMesh.getPos(Tile{ 1,2 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::grey30));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 2,2 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::green));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 3,2 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::grey));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 4,2 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::orange));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);



            pos = tileMesh.getPos(Tile{ 1,3 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::blue));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 2,3 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::grey30));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 3,3 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::turquoise));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 4,3 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::grey));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

#else

            pos = tileMesh.getPos(Tile{ 1,1 });
            size = tileMesh.getSize(Tile{ 1,2 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::blue));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 2,1 });
            size = tileMesh.getSize(Tile{ 2,2 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::turquoise));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 4,1 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::grey));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 4,2 });
            size = tileMesh.getSize(Tile{ 1,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::green));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);

            pos = tileMesh.getPos(Tile{ 1,3 });
            size = tileMesh.getSize(Tile{ 4,1 });
            drawList->AddRect(pos, ImVec2{ pos.x + size.x, pos.y + size.y },
                ImGui::GetColorU32(color::yellow));
            ImGui::SetCursorScreenPos(pos);
            ImGui::Text("Size %f x %f", size.x, size.y);
#endif
        }
#endif
    }
    ImGui::End();
}

} // namespace ds
