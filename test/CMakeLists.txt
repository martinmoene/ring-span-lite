# Copyright (c) 2017 Martin Moene
#
# https://github.com/martinmoene/ring-span-lite
#
# This code is licensed under the MIT License (MIT).

cmake_minimum_required( VERSION 2.8 )

project( test )

option( RING_SPAN_LITE_COLOURISE_TEST "Colourise test output" OFF )

include_directories  (  "${PROJECT_SOURCE_DIR}/../include/nonstd" )

set( SOURCES ring-span-lite.t.cpp ring-span.t.cpp )

add_definitions( -Dgsl_CONFIG_CONTRACT_VIOLATION_THROWS )

if( RING_SPAN_LITE_COLOURISE_TEST )
    add_definitions( -Dlest_FEATURE_COLOURISE=1 )
endif()

set( HAS_STD_FLAGS  FALSE )
set( HAS_CPP11_FLAG FALSE )
set( HAS_CPP14_FLAG FALSE )

if( MSVC )
    add_compile_options( -W3 -EHsc -D_SCL_SECURE_NO_WARNINGS )

elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang" )

    set( HAS_STD_FLAGS TRUE )

    # GNU: available -std flags depends on version
    if( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
        execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)

        if( GCC_VERSION )
            if( NOT GCC_VERSION VERSION_LESS 4.8.0 )
                set( HAS_CPP11_FLAG TRUE )
            endif()
            if( NOT GCC_VERSION VERSION_LESS 4.9.2 )
                set( HAS_CPP14_FLAG TRUE )
            endif()
        endif()
    else()  # Clang:
        set( HAS_CPP11_FLAG TRUE )
#       set( HAS_CPP14_FLAG TRUE )
    endif()

    add_compile_options( -Wall -Wno-missing-braces -fno-elide-constructors -Wconversion -Wsign-conversion -Wno-string-conversion )

    add_executable( ring-span-lite-cpp98.t ${SOURCES} )
    target_compile_options( ring-span-lite-cpp98.t  PUBLIC -std=c++98 )

    add_executable( ring-span-lite-cpp03.t ${SOURCES} )
    target_compile_options( ring-span-lite-cpp03.t  PUBLIC -std=c++03 )

    if( HAS_CPP11_FLAG )
        add_executable( ring-span-lite-cpp11.t ${SOURCES} )
        target_compile_options( ring-span-lite-cpp11.t  PUBLIC -std=c++11 )
    endif()

    if( HAS_CPP14_FLAG )
        add_executable( ring-span-lite-cpp14.t ${SOURCES} )
        target_compile_options( ring-span-lite-cpp14.t  PUBLIC -std=c++14 )
    endif()

elseif( CMAKE_CXX_COMPILER_ID MATCHES "Intel" )
# as is
else()
# as is
endif()

add_executable( ring-span-lite.t ${SOURCES} )

# configure unit tests via CTest:

enable_testing()

if( NOT HAS_STD_FLAGS )
    add_test(     NAME test          COMMAND ring-span-lite.t --pass )
else()
    add_test(     NAME test-cpp98    COMMAND ring-span-lite-cpp98.t )
    add_test(     NAME test-cpp03    COMMAND ring-span-lite-cpp03.t )
    if( HAS_CPP11_FLAG )
        add_test( NAME test-cpp11    COMMAND ring-span-lite-cpp11.t )
    endif()
    if( HAS_CPP14_FLAG )
        add_test( NAME test-cpp14    COMMAND ring-span-lite-cpp14.t --pass )
    endif()
endif()
add_test(         NAME list_version  COMMAND ring-span-lite.t --version )
add_test(         NAME list_tags     COMMAND ring-span-lite.t --list-tags )
add_test(         NAME list_tests    COMMAND ring-span-lite.t --list-tests )

# end of file
