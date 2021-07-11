// Copyright 2017-2018 by Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef  nsrs_RING_SPAN_LITE_T_HEADER
#define  nsrs_RING_SPAN_LITE_T_HEADER  "ring-span-main.t.hpp"
#endif

#include nsrs_RING_SPAN_LITE_T_HEADER

#define nsrs_PRESENT( x ) \
    std::cout << #x << ": " << x << "\n"

#define nsrs_ABSENT( x ) \
    std::cout << #x << ": (undefined)\n"

lest::tests & specification()
{
    static lest::tests tests;
    return tests;
}

CASE( "ring-span-lite version" "[.ring-span]][.version]" )
{
    nsrs_PRESENT( ring_span_lite_MAJOR );
    nsrs_PRESENT( ring_span_lite_MINOR );
    nsrs_PRESENT( ring_span_lite_PATCH );
    nsrs_PRESENT( ring_span_lite_VERSION );
}

CASE( "ring-span-lite configuration" "[.ring-span][.config]" )
{
    nsrs_PRESENT( nsrs_HAVE_STD_RING_SPAN );
    nsrs_PRESENT( nsrs_USES_STD_RING_SPAN );
    nsrs_PRESENT( nsrs_RING_SPAN_DEFAULT );
    nsrs_PRESENT( nsrs_RING_SPAN_NONSTD );
    nsrs_PRESENT( nsrs_RING_SPAN_STD );
    nsrs_PRESENT( nsrs_CONFIG_SELECT_RING_SPAN );
//  nsrs_PRESENT( nsrs_CONFIG_NO_EXCEPTIONS );
    nsrs_PRESENT( nsrs_CPLUSPLUS );
}

CASE( "C++ language: __cplusplus" "[.stdc++]" )
{
    nsrs_PRESENT( __cplusplus );

#ifdef _MSVC_LANG
    nsrs_PRESENT( _MSVC_LANG );
#else
    nsrs_ABSENT(  _MSVC_LANG );
#endif
}

CASE( "C++ compiler: compiler version" "[.compiler]" )
{
#if nsrs_USES_STD_RING_SPAN
    std::cout << "(Compiler version not available: using std::ring_span)\n";
#else
    nsrs_PRESENT( nsrs_COMPILER_CLANG_VERSION );
    nsrs_PRESENT( nsrs_COMPILER_GNUC_VERSION );
    nsrs_PRESENT( nsrs_COMPILER_MSVC_VERSION );
#endif
}

CASE( "Presence of C++ language features" "[.stdlanguage]" )
{
#if nsrs_USES_STD_RING_SPAN
    std::cout << "(Presence of C++ language features not available: using std::ring_span)\n";
#else
    nsrs_PRESENT( nsrs_HAVE_CONSTEXPR_11 );
    nsrs_PRESENT( nsrs_HAVE_CONSTEXPR_14 );
    nsrs_PRESENT( nsrs_HAVE_IS_DEFAULT );
    nsrs_PRESENT( nsrs_HAVE_IS_DELETE );
    nsrs_PRESENT( nsrs_HAVE_NOEXCEPT );
    nsrs_PRESENT( nsrs_HAVE_NULLPTR );
    nsrs_PRESENT( nsrs_HAVE_NO_UNIQUE_ADDRESS );
#endif
}

CASE( "Presence of C++ library features" "[.stdlibrary]" )
{
#if nsrs_USES_STD_RING_SPAN
    std::cout << "(Presence of C++ library features not available: using std::ring_span)\n";
#else
    // none
#endif

#if defined _HAS_CPP0X
    nsrs_PRESENT( _HAS_CPP0X );
#else
    nsrs_ABSENT(  _HAS_CPP0X );
#endif
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv );
}

#if 0
g++            -I../include -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++98 -I../include -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++03 -I../include -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++0x -I../include -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++11 -I../include -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++14 -I../include -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++17 -I../include -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass

cl -EHsc -I../include ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
#endif

// end of file
