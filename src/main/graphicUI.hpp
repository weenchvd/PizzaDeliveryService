
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef GRAPHIC_UI_HPP
#define GRAPHIC_UI_HPP

#include"msystem.hpp"
#include<d3d11.h>

bool initializeGUI(WNDCLASSEXW& wc, HWND& hWnd);

void shutdownGUI(WNDCLASSEXW& wc, HWND& hWnd);

void renderGUI(bool* open, ds::ManagmentSystem& ms);

#endif // !GRAPHIC_UI_HPP
