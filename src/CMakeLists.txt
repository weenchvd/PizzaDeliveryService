
# Copyright (c) 2023 Vitaly Dikov
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_compile_definitions(_UNICODE UNICODE)

add_subdirectory("imgui")
add_subdirectory("main")
add_subdirectory("map")
add_subdirectory("menu")
