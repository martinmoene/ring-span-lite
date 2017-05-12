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
#if nsrs_RING_SPAN_LITE_EXTENSION
    // Note: hack: depends on iterator implementation to use index [0..size());
    // create begin iterator with index 0 (it and bgn are on different containers!):
    typename RS::value_type arr[1]; ::nonstd::detail::ring_iterator< RS, true> bgn = RS( arr, arr + 1 ).cbegin();

    return os << "[ring_iterator: " << (it - bgn) << "]";
#else
    return os << "[ring_iterator: ?]";
#endif
}

template< class RS, bool is_const >
inline std::ostream & operator<<( std::ostream & os, std::reverse_iterator<::nonstd::detail::ring_iterator<RS, is_const> > const & it )
{
    return os << "[reverse ring_iterator: ?]";
}

}

namespace lest {

using ::nonstd::operator<<;

} // namespace lest

#endif // ring_span_lite_T_INCLUDED

// end of file
