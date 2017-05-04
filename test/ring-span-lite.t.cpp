// Copyright (c) 2017 Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
//
// This code is licensed under the MIT License (MIT).

#include "ring-span-lite.t.hpp"

#define ring_span_PRESENT( x ) \
    std::cout << #x << ": " << x << "\n"

#define ring_span_ABSENT( x ) \
    std::cout << #x << ": (undefined)\n"

lest::tests & specification()
{
    static lest::tests tests;
    return tests;
}

CASE( "__cplusplus" "[.stdc++]" )
{
    ring_span_PRESENT( __cplusplus );
}

CASE( "ring-span-lite version" "[.version]" )
{
    ring_span_PRESENT( ring_span_lite_VERSION );
}

CASE( "compiler version" "[.compiler]" )
{
#if ring_span_COMPILER_GNUC_VERSION
    ring_span_PRESENT( ring_span_COMPILER_GNUC_VERSION );
#else
    ring_span_ABSENT(  ring_span_COMPILER_GNUC_VERSION );
#endif

#if ring_span_COMPILER_MSVC_VERSION
    ring_span_PRESENT( ring_span_COMPILER_MSVC_VERSION );
#else
    ring_span_ABSENT(  ring_span_COMPILER_MSVC_VERSION );
#endif
}

CASE( "Presence of C++ language features" "[.stdlanguage]" )
{
#if ring_span_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG
    ring_span_PRESENT( ring_span_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG );
#else
    ring_span_ABSENT(  ring_span_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG );
#endif

#if ring_span_HAVE_CONSTEXPR_11
    ring_span_PRESENT( ring_span_HAVE_CONSTEXPR_11 );
#else
    ring_span_ABSENT(  ring_span_HAVE_CONSTEXPR_11 );
#endif

#if ring_span_HAVE_CONSTEXPR_14
    ring_span_PRESENT( ring_span_HAVE_CONSTEXPR_14 );
#else
    ring_span_ABSENT(  ring_span_HAVE_CONSTEXPR_14 );
#endif

#if ring_span_HAVE_ENUM_CLASS
    ring_span_PRESENT( ring_span_HAVE_ENUM_CLASS );
#else
    ring_span_ABSENT(  ring_span_HAVE_ENUM_CLASS );
#endif

#if ring_span_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE
    ring_span_PRESENT( ring_span_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE );
#else
    ring_span_ABSENT(  ring_span_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE );
#endif

#if ring_span_HAVE_EXPLICIT_CONVERSION
    ring_span_PRESENT( ring_span_HAVE_EXPLICIT_CONVERSION );
#else
    ring_span_ABSENT(  ring_span_HAVE_EXPLICIT_CONVERSION );
#endif

#if ring_span_HAVE_NOEXCEPT
    ring_span_PRESENT( ring_span_HAVE_NOEXCEPT );
#else
    ring_span_ABSENT(  ring_span_HAVE_NOEXCEPT );
#endif
}

CASE( "Presence of C++ library features" "[.stdlibrary]" )
{
#if ring_span_HAVE_TYPE_TRAITS
    ring_span_PRESENT( ring_span_HAVE_TYPE_TRAITS );
#else
    ring_span_ABSENT(  ring_span_HAVE_TYPE_TRAITS );
#endif
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv );
}

#if 0
g++            -I../include/nonstd -o ring-span-lite.t.exe ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe --pass
g++ -std=c++98 -I../include/nonstd -o ring-span-lite.t.exe ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe --pass
g++ -std=c++03 -I../include/nonstd -o ring-span-lite.t.exe ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe --pass
g++ -std=c++0x -I../include/nonstd -o ring-span-lite.t.exe ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe --pass
g++ -std=c++11 -I../include/nonstd -o ring-span-lite.t.exe ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe --pass
g++ -std=c++14 -I../include/nonstd -o ring-span-lite.t.exe ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe --pass
g++ -std=c++17 -I../include/nonstd -o ring-span-lite.t.exe ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe --pass

cl -EHsc -I../include/nonstd ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe --pass
#endif

// end of file
