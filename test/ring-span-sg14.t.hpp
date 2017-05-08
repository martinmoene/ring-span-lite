// Copyright (c) 2017 Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
//
// This code is licensed under the MIT License (MIT).

#pragma once

#ifndef ring_span_lite_T_INCLUDED
#define ring_span_lite_T_INCLUDED

// Compiler detection (C++17 is speculative):

#define nsrs_CPP11_OR_GREATER  ( __cplusplus >= 201103L )
#define nsrs_CPP14_OR_GREATER  ( __cplusplus >= 201402L )
#define nsrs_CPP17_OR_GREATER  ( __cplusplus >= 201700L )

// Determine MSVC version (e.g. 14 is VC14, (VS2015/VS2017):

#if defined(_MSC_VER) && !defined(__clang__)
# define nsrs_COMPILER_MSVC_VERSION   (_MSC_VER / 100 - 5 - (_MSC_VER < 1900))
#else
# define nsrs_COMPILER_MSVC_VERSION   0
#endif

// Consider VC14 as C++11, C++14 for SG14 ring_span:

#if nsrs_COMPILER_MSVC_VERSION >= 14
# undef  nsrs_CPP11_OR_GREATER
# define nsrs_CPP11_OR_GREATER  1
# undef  nsrs_CPP14_OR_GREATER
# define nsrs_CPP14_OR_GREATER  1
#endif

#include "sg14-ring.h"
#include "lest_cpp03.hpp"

using namespace sg14;

#define ring_span_lite_VERSION "(sg14)"

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

namespace sg14 {

// provide missing swap:

template< class T, class Popper >
void swap( ring_span<T,Popper> & lhs, ring_span<T,Popper> & rhs ) noexcept( noexcept( lhs.swap(rhs) ) )
{
    lhs.swap(rhs);
}

// use oparator<< instead of to_string() overload;
// see  http://stackoverflow.com/a/10651752/437272

template< typename T, class Popper>
inline std::ostream & operator<<( std::ostream & os, ::sg14::ring_span<T, Popper> const & rs )
{
    os << "[ring_span: "; std::copy( rs.begin(), rs.end(), std::ostream_iterator<T>(os, ", ") ); return os << "]";
}

}

namespace lest {

using ::sg14::operator<<;

} // namespace lest

#endif // ring_span_lite_T_INCLUDED

// end of file
