
# Copyright (c) 2023 Vitaly Dikov
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.14)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


set(LIBRARY_NAME "menu")
set(SOURCE_CXX_LIST "gui_menuCommon.cpp"
                    "gui_menuMain.cpp"
)

add_library(${LIBRARY_NAME} STATIC ${SOURCE_CXX_LIST})

target_include_directories(${LIBRARY_NAME}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${LIBRARY_NAME}
                      PUBLIC imgui
                      PUBLIC map
)
