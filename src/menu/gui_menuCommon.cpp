
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"gui_menuCommon.hpp"
#include"options.hpp"
#include<algorithm>
#include<assert.h>

namespace ds {

using namespace std;

ImGuiTileMesh::ImGuiTileMesh(ImVec2 pos,
                             ImVec2 regionSize,
                             Tile dimension,
                             float itemSpacing) noexcept
    :
    pos_        { pos },
    regionSize_ { regionSize },
    tileSize_   {},
    itemSpacing_{ itemSpacing },
    dimension_  { dimension }
{
    assert(dimension_.x_ > 0);
    assert(dimension_.y_ > 0);
    tileSize_ = ImVec2{
        floor((regionSize_.x - itemSpacing_ * (dimension_.x_ - 1)) / dimension_.x_),
        floor((regionSize_.y - itemSpacing_ * (dimension_.y_ - 1)) / dimension_.y_),
    };
}

ImVec2 ImGuiTileMesh::getPos(Tile coordinate) const noexcept
{
    return ImVec2{
        pos_.x + (tileSize_.x + itemSpacing_) * (coordinate.x_ - 1),
        pos_.y + (tileSize_.y + itemSpacing_) * (coordinate.y_ - 1)
    };
}

ImVec2 ImGuiTileMesh::getSize(Tile dimension) const noexcept
{
    return ImVec2{
        tileSize_.x * dimension.x_ + itemSpacing_ * (dimension.x_ - 1),
        tileSize_.y * dimension.y_ + itemSpacing_ * (dimension.y_ - 1)
    };
}

///************************************************************************************************

void guiCommonInitialization(ImGuiWindowFlags& window_flags)
{
    const ImGuiViewport* viewport{ ImGui::GetMainViewport() };
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    window_flags |= ImGuiWindowFlags_NoDocking;
}

///************************************************************************************************

void guiMenuBarOptions(ManagmentSystem& ms)
{
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
            ImGui::TextUnformatted("Map");
            ImGui::Checkbox("Show grid", &Options::instance().optMap_.showGrid_);
            ImGui::SliderInt("Scale##Map",
                &Options::instance().optMap_.scale_,
                OptionsMap::minScale_,
                OptionsMap::maxScale_,
                "%d", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderInt("Vertex radius (pixels)##Map",
                &Options::instance().optMap_.vertexRadius_,
                OptionsMap::minVertexRadius_,
                OptionsMap::maxVertexRadius_,
                "%d", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderInt("Edge width (pixels)##Map",
                &Options::instance().optMap_.edgeWidth_,
                OptionsMap::minEdgeWidth_,
                OptionsMap::maxEdgeWidth_,
                "%d", ImGuiSliderFlags_AlwaysClamp);

            ImGui::Separator();
            ImGui::TextUnformatted("Delivery");
            ImGui::SliderInt("Delivery time (sec)##Delivery",
                &Options::instance().optDelivery_.deliveryTime_,
                OptionsDelivery::minDeliveryTime_,
                OptionsDelivery::maxDeliveryTime_,
                "%d", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderInt("Time to check free couriers (sec)##Delivery",
                &Options::instance().optDelivery_.checkTimeFreeCour_,
                OptionsDelivery::minCheckTimeFreeCour_,
                OptionsDelivery::maxCheckTimeFreeCour_,
                "%d", ImGuiSliderFlags_AlwaysClamp);

            ImGui::Separator();
            ImGui::TextUnformatted("Courier");
            ImGui::SliderInt("Courier pause time (sec)##Courier",
                &Options::instance().optCourier_.pauseTime_,
                OptionsCourier::minPauseTime_,
                OptionsCourier::maxPauseTime_,
                "%d", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderInt("Courier pause chance (%)##Courier",
                &Options::instance().optCourier_.pauseChance_,
                OptionsCourier::minPauseChance_,
                OptionsCourier::maxPauseChance_,
                "%d", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderInt("Order acceptance time (sec)##Courier",
                &Options::instance().optCourier_.acceptanceTime_,
                OptionsCourier::minAcceptanceTime_,
                OptionsCourier::maxAcceptanceTime_,
                "%d", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderInt("Delivery time (sec)##Courier",
                &Options::instance().optCourier_.deliveryTime_,
                OptionsCourier::minDeliveryTime_,
                OptionsCourier::maxDeliveryTime_,
                "%d", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderInt("Payment time (sec)##Courier",
                &Options::instance().optCourier_.paymentTime_,
                OptionsCourier::minPaymentTime_,
                OptionsCourier::maxPaymentTime_,
                "%d", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderInt("Average courier speed (meters per second)##Courier",
                &Options::instance().optCourier_.averageSpeed_,
                OptionsCourier::minAverageSpeed_,
                OptionsCourier::maxAverageSpeed_,
                "%d", ImGuiSliderFlags_AlwaysClamp);

            ImGui::EndMenu();
        }

        ImGui::SameLine(0.0f, 50.0f);
        ImGui::TextUnformatted(cmn::getTime(ms.getCurrentTime()).c_str());

        ImGui::EndMenuBar();
    }
}

} // namespace ds
