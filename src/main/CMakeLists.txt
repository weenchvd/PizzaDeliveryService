
# Copyright (c) 2023 Vitaly Dikov
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.14)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


set(EXEC_NAME "pizza-ds")
set(SOURCE_CXX_LIST "graphicUI.cpp"
                    "main.cpp"
)

# Add source to this project's executable.
add_executable(${EXEC_NAME} ${SOURCE_CXX_LIST})

target_link_libraries(${EXEC_NAME}
                      PRIVATE imgui
                      PRIVATE map
                      PRIVATE menu
)

# Add tests and install targets if needed.
install(TARGETS ${EXEC_NAME} DESTINATION "${EXEC__INSTALL_DIR}")
