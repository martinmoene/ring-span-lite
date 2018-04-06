// Copyright (c) 2017 Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
//
// This code is licensed under the MIT License (MIT).

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

CASE( "ring-span-lite version" "[.version]" )
{
    nsrs_PRESENT( ring_span_lite_VERSION );
}

CASE( "C++ compiler: compiler version" "[.compiler]" )
{
#ifdef nsrs_COMPILER_CLANG_VERSION
    nsrs_PRESENT( nsrs_COMPILER_CLANG_VERSION );
#else
    nsrs_ABSENT(  nsrs_COMPILER_CLANG_VERSION );
#endif

#ifdef nsrs_COMPILER_GNUC_VERSION
    nsrs_PRESENT( nsrs_COMPILER_GNUC_VERSION );
#else
    nsrs_ABSENT(  nsrs_COMPILER_GNUC_VERSION );
#endif

#ifdef nsrs_COMPILER_MSVC_VERSION
    nsrs_PRESENT( nsrs_COMPILER_MSVC_VERSION );
#else
    nsrs_ABSENT(  nsrs_COMPILER_MSVC_VERSION );
#endif
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

CASE( "C++ language: nsrs_CPP11_OR_GREATER" "[.stdc++]" )
{
#if nsrs_CPP11_OR_GREATER
    nsrs_PRESENT( nsrs_CPP11_OR_GREATER );
#else
    nsrs_ABSENT(  nsrs_CPP11_OR_GREATER );
#endif
}

CASE( "C++ language: nsrs_CPP14_OR_GREATER" "[.stdc++]" )
{
#if nsrs_CPP14_OR_GREATER
    nsrs_PRESENT( nsrs_CPP14_OR_GREATER );
#else
    nsrs_ABSENT(  nsrs_CPP14_OR_GREATER );
#endif
}

CASE( "C++ language: nsrs_CPP17_OR_GREATER" "[.stdc++]" )
{
#if nsrs_CPP17_OR_GREATER
    nsrs_PRESENT( nsrs_CPP17_OR_GREATER );
#else
    nsrs_ABSENT(  nsrs_CPP17_OR_GREATER );
#endif
}

CASE( "C++ language: presence of C++ language features" "[.stdlanguage]" )
{
#if nsrs_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG
    nsrs_PRESENT( nsrs_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG );
#else
    nsrs_ABSENT(  nsrs_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG );
#endif

#if nsrs_HAVE_CONSTEXPR_11
    nsrs_PRESENT( nsrs_HAVE_CONSTEXPR_11 );
#else
    nsrs_ABSENT(  nsrs_HAVE_CONSTEXPR_11 );
#endif

#if nsrs_HAVE_CONSTEXPR_14
    nsrs_PRESENT( nsrs_HAVE_CONSTEXPR_14 );
#else
    nsrs_ABSENT(  nsrs_HAVE_CONSTEXPR_14 );
#endif

#if nsrs_HAVE_ENUM_CLASS
    nsrs_PRESENT( nsrs_HAVE_ENUM_CLASS );
#else
    nsrs_ABSENT(  nsrs_HAVE_ENUM_CLASS );
#endif

#if nsrs_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    nsrs_PRESENT( nsrs_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE );
#else
    nsrs_ABSENT(  nsrs_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE );
#endif

#if nsrs_HAVE_EXPLICIT_CONVERSION
    nsrs_PRESENT( nsrs_HAVE_EXPLICIT_CONVERSION );
#else
    nsrs_ABSENT(  nsrs_HAVE_EXPLICIT_CONVERSION );
#endif

#if nsrs_HAVE_NOEXCEPT
    nsrs_PRESENT( nsrs_HAVE_NOEXCEPT );
#else
    nsrs_ABSENT(  nsrs_HAVE_NOEXCEPT );
#endif
}

CASE( "C++ library: presence of C++ library features" "[.stdlibrary]" )
{
#if nsrs_HAVE_TYPE_TRAITS
    nsrs_PRESENT( nsrs_HAVE_TYPE_TRAITS );
#else
    nsrs_ABSENT(  nsrs_HAVE_TYPE_TRAITS );
#endif
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv );
}

#if 0
g++            -I../include/nonstd -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++98 -I../include/nonstd -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++03 -I../include/nonstd -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++0x -I../include/nonstd -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++11 -I../include/nonstd -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++14 -I../include/nonstd -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
g++ -std=c++17 -I../include/nonstd -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass

cl -EHsc -I../include/nonstd ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe --pass
#endif

// end of file
