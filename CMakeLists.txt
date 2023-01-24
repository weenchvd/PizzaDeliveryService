
# Copyright (c) 2023 Vitaly Dikov
# 
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.20)

project("PizzaDeliveryService"  VERSION 0.1.0)

option(ENABLE_TESTS "Enable tests" OFF)



set(EXTERNAL_PROJECTS_DIR "${CMAKE_SOURCE_DIR}/external")
set(SRC_FILES_DIR "${CMAKE_SOURCE_DIR}/files")
set(SRC_FONTS_DIR "${SRC_FILES_DIR}/fonts")

#**********************************************************************
set(EXEC__BIN_DIR "${CMAKE_BINARY_DIR}/src/main")
set(EXEC__INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/EXEC")
set(LICENSES__DIR__REL_PATH_FROM_EXEC "licenses")
set(DATA__DIR__REL_PATH_FROM_EXEC "data")
set(FONTS__DIR__REL_PATH_FROM_EXEC "${DATA__DIR__REL_PATH_FROM_EXEC}/fonts")

#**********************************************************************
function(create_file _file_name)
  file(REAL_PATH "${_file_name}" file_)
  if (NOT EXISTS ${file_})
    file(WRITE ${file_} "")
  endif()
endfunction()

#**********************************************************************
if(ENABLE_TESTS)
  # set paths
  set(GOOGLETEST_LIB_ROOT_DIR      "${EXTERNAL_PROJECTS_DIR}/googletest")
  set(GOOGLETEST_LIB_REPOS_DIR     "${GOOGLETEST_LIB_ROOT_DIR}/repos")
  set(GOOGLETEST_LIB_FCSTUFF_DIR   "${GOOGLETEST_LIB_ROOT_DIR}/cmake-fetchcontent-stuff")

  # download and unpack GOOGLETEST library
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         "release-1.12.1"
    PREFIX          ${GOOGLETEST_LIB_FCSTUFF_DIR}
    SOURCE_DIR      ${GOOGLETEST_LIB_REPOS_DIR}
  )

  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

  FetchContent_MakeAvailable(googletest)

  enable_testing()

  add_subdirectory("test")
endif()

#[=[
if(WIN32)
  cmake_path(GET CMAKE_BINARY_DIR FILENAME CONFIG_NAME)

  # set paths
  set(BOOST_LIB_ROOT_DIR    "${EXTERNAL_PROJECTS_DIR}/boost")
  set(BOOST_LIB_REPOS_DIR   "${BOOST_LIB_ROOT_DIR}/repos")
  set(BOOST_LIB_BUILD_DIR   "${BOOST_LIB_ROOT_DIR}/build")
  set(BOOST_LIB_FCSTUFF_DIR "${BOOST_LIB_ROOT_DIR}/cmake-fetchcontent-stuff")
  if (CONFIG_NAME)
    set(BOOST_LIB_INSTALL_DIR "${BOOST_LIB_ROOT_DIR}-install/${CONFIG_NAME}")
  else()
    set(BOOST_LIB_INSTALL_DIR "${BOOST_LIB_ROOT_DIR}-install")
  endif()

  set(BOOST_LIB_INCLUDE_DIR "${BOOST_LIB_INSTALL_DIR}/include")
  set(BOOST_LIB_LIB_DIR     "${BOOST_LIB_INSTALL_DIR}/lib")

  # set script options and commands for the build
  set(BOOST_LIB_BUILD_OPTIONS         "--includedir=${BOOST_LIB_INCLUDE_DIR}")
  list(APPEND BOOST_LIB_BUILD_OPTIONS "--libdir=${BOOST_LIB_LIB_DIR}")
  list(APPEND BOOST_LIB_BUILD_OPTIONS "--build-dir=${BOOST_LIB_BUILD_DIR}")
  list(APPEND BOOST_LIB_BUILD_OPTIONS "--build-type=minimal")
  list(APPEND BOOST_LIB_BUILD_OPTIONS "--no-cmake-config")

  set(BOOST_LIB_BUILD_PROPERTIES            "address-model=64")
  list(APPEND BOOST_LIB_BUILD_PROPERTIES    "link=static")
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    list(APPEND BOOST_LIB_BUILD_PROPERTIES  "variant=debug")
  else()
    list(APPEND BOOST_LIB_BUILD_PROPERTIES  "variant=release")
  endif()
  
  file(GLOB BOOST_LIB_LIST "${BOOST_LIB_LIB_DIR}/*boost*.lib")
  list(LENGTH BOOST_LIB_LIST BOOST_LIB_LIST_LENGTH)

  # download and unpack BOOST library
  include(FetchContent)
  FetchContent_Declare(
    boost
    GIT_REPOSITORY  https://github.com/boostorg/boost.git
    GIT_TAG         "boost-1.81.0"
    PREFIX          ${BOOST_LIB_FCSTUFF_DIR}
    SOURCE_DIR      ${BOOST_LIB_REPOS_DIR}
  )
  set(FETCHCONTENT_QUIET OFF CACHE BOOL "" FORCE)

  FetchContent_GetProperties(boost)
  if(NOT boost_POPULATED)
    FetchContent_Populate(boost)
  endif()

  # run build and installation
  if(${boost_POPULATED} AND NOT ${BOOST_LIB_LIST_LENGTH})
    execute_process(
      COMMAND           "bootstrap.bat"
      WORKING_DIRECTORY ${BOOST_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
    execute_process(
      COMMAND           "./b2" ${BOOST_LIB_BUILD_OPTIONS} ${BOOST_LIB_BUILD_PROPERTIES} "install"
      WORKING_DIRECTORY ${BOOST_LIB_REPOS_DIR}
      COMMAND_ECHO      STDOUT
    )
  endif()
endif()
]=]

if(WIN32)
  # set paths
  set(IMGUI_LIB_ROOT_DIR      "${EXTERNAL_PROJECTS_DIR}/imgui")
  set(IMGUI_LIB_REPOS_DIR     "${IMGUI_LIB_ROOT_DIR}/repos")
  set(IMGUI_LIB_FCSTUFF_DIR   "${IMGUI_LIB_ROOT_DIR}/cmake-fetchcontent-stuff")
  set(IMGUI_LIB_BACKENDS_DIR  "${IMGUI_LIB_REPOS_DIR}/backends")

  # download and unpack IMGUI library
  include(FetchContent)
  FetchContent_Declare(
    imgui
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         "d822c65317ba881798bed8fce9ffba267d27dada"
    PREFIX          ${IMGUI_LIB_FCSTUFF_DIR}
    SOURCE_DIR      ${IMGUI_LIB_REPOS_DIR}
  )
  set(FETCHCONTENT_QUIET OFF CACHE BOOL "" FORCE)

  FetchContent_GetProperties(imgui)
  if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
  endif()
endif()

#**********************************************************************
add_subdirectory("src")
