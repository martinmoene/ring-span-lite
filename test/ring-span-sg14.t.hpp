// Copyright (c) 2017 Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
//
// This code is licensed under the MIT License (MIT).

#pragma once

#ifndef ring_span_lite_T_INCLUDED
#define ring_span_lite_T_INCLUDED

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
