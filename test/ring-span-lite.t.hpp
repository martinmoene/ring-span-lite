// Copyright (c) 2017 Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
//
// This code is licensed under the MIT License (MIT).

#pragma once

#ifndef ring_span_lite_T_INCLUDED
#define ring_span_lite_T_INCLUDED

#include "ring_span.hpp"
#include "lest_cpp03.hpp"

using namespace nonstd;

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

namespace nonstd {

// use oparator<< instead of to_string() overload;
// see  http://stackoverflow.com/a/10651752/437272

template< typename T, class Popper>
inline std::ostream & operator<<( std::ostream & os, ::nonstd::ring_span<T, Popper> const & rs )
{
    os << "[ring_span: "; std::copy( rs.begin(), rs.end(), std::ostream_iterator<T>(os, ", ") ); return os << "]";
}

template< class RS, bool is_const >
inline std::ostream & operator<<( std::ostream & os, ::nonstd::detail::ring_iterator<RS, is_const> const & it )
{
    return os << "[ring_iterator: ?]";
}

}

namespace lest {

using ::nonstd::operator<<;

} // namespace lest

#endif // ring_span_lite_T_INCLUDED

// end of file
