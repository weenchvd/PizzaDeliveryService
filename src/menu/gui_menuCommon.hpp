
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef GUI_MENU_COMMON_HPP
#define GUI_MENU_COMMON_HPP

#include"imgui.h"
#include"msystem.hpp"

namespace ds {

namespace color {

const ImVec4 green              { 43.0f / 255.0f, 146.0f / 255.0f, 22.0f / 255.0f, 1.0f };
const ImVec4 turquoise          { 17.0f / 255.0f, 118.0f / 255.0f, 113.0f / 255.0f, 1.0f };
const ImVec4 blue               { 66.0f / 255.0f, 173.0f / 255.0f, 250.0f / 255.0f, 1.0f };
const ImVec4 orange             { 255.0f / 255.0f, 153.0f / 255.0f, 0.0f / 255.0f, 1.0f };
const ImVec4 yellow             { 195.0f / 255.0f, 151.0f / 255.0f, 0.0f / 255.0f, 1.0f };
const ImVec4 yellowDark         { 125.0f / 255.0f, 97.0f / 255.0f, 0.0f / 255.0f, 1.0f };
const ImVec4 grey               { 150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f, 1.0f };
const ImVec4 greyDark           { 80.0f / 255.0f, 80.0f / 255.0f, 80.0f / 255.0f, 1.0f };
const ImVec4 white              { 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f };
const ImVec4 grey90             { 225.0f / 255.0f, 225.0f / 255.0f, 225.0f / 255.0f, 1.0f };
const ImVec4 grey80             { 200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.0f };
const ImVec4 grey70             { 175.0f / 255.0f, 175.0f / 255.0f, 175.0f / 255.0f, 1.0f };
const ImVec4 grey60             { 150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f, 1.0f };
const ImVec4 grey50             { 125.0f / 255.0f, 125.0f / 255.0f, 125.0f / 255.0f, 1.0f };
const ImVec4 grey40             { 100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f };
const ImVec4 grey30             { 75.0f / 255.0f, 75.0f / 255.0f, 75.0f / 255.0f, 1.0f };
const ImVec4 grey20             { 50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f };

} //namespace color


struct Tile {
    static constexpr unsigned short int minSize_{ 1 };
    static constexpr unsigned short int maxSize_{ 64 };

    constexpr Tile(unsigned short int x, unsigned short int y) noexcept
        : x_{ x }, y_{ y }
    {
        static_assert(minSize_ >= 1);
        if      (x_ < minSize_) x_ = minSize_;
        else if (x_ > maxSize_) x_ = maxSize_;
        if      (y_ < minSize_) y_ = minSize_;
        else if (y_ > maxSize_) y_ = maxSize_;
    }

    unsigned short int x_;
    unsigned short int y_;
};


class ImGuiTileMesh {
public:
    explicit ImGuiTileMesh(ImVec2 pos,
                           ImVec2 regionSize,
                           Tile dimension,
                           float itemSpacing) noexcept;

    ImVec2 getPos(Tile coordinate) const noexcept;

    ImVec2 getSize(Tile dimension) const noexcept;

private:
    ImVec2          pos_;
    ImVec2          regionSize_;
    ImVec2          tileSize_;
    float           itemSpacing_;
    Tile            dimension_;
};

///************************************************************************************************

void guiCommonInitialization(ImGuiWindowFlags& window_flags);

///************************************************************************************************

void guiMenuBarOptions(ManagmentSystem& ms);

} // namespace ds

#endif // !GUI_MENU_COMMON_HPP
