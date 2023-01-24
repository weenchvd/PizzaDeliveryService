
# Copyright (c) 2023 Vitaly Dikov
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.20)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)



set(LIBRARY_NAME "imgui")
set(SOURCE_CXX_LIST "${IMGUI_LIB_REPOS_DIR}/imgui.cpp"
                    "${IMGUI_LIB_REPOS_DIR}/imgui_demo.cpp"
                    "${IMGUI_LIB_REPOS_DIR}/imgui_draw.cpp"
                    "${IMGUI_LIB_REPOS_DIR}/imgui_tables.cpp"
                    "${IMGUI_LIB_REPOS_DIR}/imgui_widgets.cpp"
                    "${IMGUI_LIB_BACKENDS_DIR}/imgui_impl_dx11.cpp"
                    "${IMGUI_LIB_BACKENDS_DIR}/imgui_impl_win32.cpp"
)

add_library(${LIBRARY_NAME} STATIC ${SOURCE_CXX_LIST})

target_include_directories(${LIBRARY_NAME}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                           PUBLIC "${IMGUI_LIB_REPOS_DIR}"
                           PUBLIC "${IMGUI_LIB_BACKENDS_DIR}"
)

target_link_libraries(${LIBRARY_NAME}
                      PUBLIC "d3d11.lib"
                      PUBLIC "d3dcompiler.lib"
                      PUBLIC "dxgi.lib"
)

configure_file("${SRC_FONTS_DIR}/UBUNTU FONT LICENCE Version 1.0.txt"
               "${EXEC__BIN_DIR}/${FONTS__DIR__REL_PATH_FROM_EXEC}/UBUNTU FONT LICENCE Version 1.0.txt"
               COPYONLY
)
configure_file("${SRC_FONTS_DIR}/Ubuntu-R.ttf"
               "${EXEC__BIN_DIR}/${FONTS__DIR__REL_PATH_FROM_EXEC}/Ubuntu-R.ttf"
               COPYONLY
)
install(FILES "${SRC_FONTS_DIR}/UBUNTU FONT LICENCE Version 1.0.txt"
        DESTINATION "${EXEC__INSTALL_DIR}/${FONTS__DIR__REL_PATH_FROM_EXEC}"
)
install(FILES "${SRC_FONTS_DIR}/Ubuntu-R.ttf"
        DESTINATION "${EXEC__INSTALL_DIR}/${FONTS__DIR__REL_PATH_FROM_EXEC}"
)
install(FILES "${IMGUI_LIB_REPOS_DIR}/LICENSE.txt"
        DESTINATION "${EXEC__INSTALL_DIR}/${LICENSES__DIR__REL_PATH_FROM_EXEC}"
        RENAME "imgui_license.txt"
)

#**********************************************************************
# create header file with paths
set(FONT_PATH_FILE_NAME "fontPath.hpp")
cmake_language(CALL create_file "${FONT_PATH_FILE_NAME}.in")
file(TO_NATIVE_PATH
  "${FONTS__DIR__REL_PATH_FROM_EXEC}/Ubuntu-R.ttf"
  FONT_UBUNTU_R_FILE__NATIVE_REL_PATH
)
configure_file("${FONT_PATH_FILE_NAME}.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/${FONT_PATH_FILE_NAME}"
)
