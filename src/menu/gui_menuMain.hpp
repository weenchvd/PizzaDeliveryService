
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef GUI_MENU_MAIN_HPP
#define GUI_MENU_MAIN_HPP

#include"msystem.hpp"
#include<memory>

namespace ds {

enum class SubmenuType : char {
    __INVALID = -1,                 /// invalid, must be the first
    // vvv TYPES vvv
    COMMON,
    KITCHEN,
    DELIVERY,
    // ^^^ TYPES ^^^
    __NUMBER_OF,
    __END                           /// must be the last
};

constexpr ImVec2 defaultMapScrolling{ 50, 50 };

struct GuiMapData {
    ImVec2 scrolling_{ defaultMapScrolling };
    ImVec2 tilePos_;
    ImVec2 tileSize_;
};

///************************************************************************************************

void guiMenuMain_Submenu(SubmenuType& submenu);

void guiMenuMain(bool* open, ManagmentSystem& ms);

void guiMenuCommon(bool* open, ManagmentSystem& ms, SubmenuType& submenu);

void guiMenuKitchen(bool* open, ManagmentSystem& ms, SubmenuType& submenu);

void guiMenuDelivery(bool* open, ManagmentSystem& ms, SubmenuType& submenu);

void guiMap(ManagmentSystem& ms, GuiMapData& mapData);

} // namespace ds

#endif // !GUI_MENU_MAIN_HPP
