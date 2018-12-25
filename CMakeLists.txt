# Copyright (c) 2017-2018 Martin Moene
#
# https://github.com/martinmoene/ring-span-lite
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required( VERSION 3.5 )

# ring_span-lite project and version, updated by script/update-version.py:

project(
    ring_span_lite
    VERSION 0.2.0
    DESCRIPTION "A C++yy-like ring_span type for C++98, C++11 and later in a single-file header-only library"
#   HOMEPAGE_URL "https://github.com/martinmoene/ring-span-lite"
    LANGUAGES CXX )

set( ${PROJECT_NAME}_version "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" )

# Toplevel or subproject:

if ( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
    set( ring_span_IS_TOPLEVEL_PROJECT TRUE )
else()
    set( ring_span_IS_TOPLEVEL_PROJECT FALSE )
endif()

# If toplevel project, enable building and performing of tests, disable building of examples:

option( RING_SPAN_LITE_OPT_BUILD_TESTS    "Build and perform ring-span-lite tests" ${ring_span_IS_TOPLEVEL_PROJECT} )
option( RING_SPAN_LITE_OPT_BUILD_EXAMPLES "Build ring-span-lite examples" OFF )

option( RING_SPAN_LITE_OPT_SELECT_STD     "Select std::ring_span"    OFF )
option( RING_SPAN_LITE_OPT_SELECT_NONSTD  "Select nonstd::ring_span" OFF )

include( GNUInstallDirs )

set( package_name "ring-span-lite" )
set( include_source_dir "${PROJECT_SOURCE_DIR}/include" )

# Interface library:

add_library(
    ${package_name} INTERFACE )

target_include_directories(
    ${package_name} INTERFACE "$<BUILD_INTERFACE:${include_source_dir}>" )

# Installation:

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

# If requested, build and perform tests, build examples:

enable_testing()

if ( RING_SPAN_LITE_OPT_BUILD_TESTS )
    add_subdirectory( test )
endif()

if ( RING_SPAN_LITE_OPT_BUILD_EXAMPLES )
    add_subdirectory( example )
endif()

# end of file
