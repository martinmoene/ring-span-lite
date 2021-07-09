// Copyright 2017-2018 by Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef RING_SPAN_LITE_T_INCLUDED
#define RING_SPAN_LITE_T_INCLUDED

#include "nonstd/ring.hpp"
#include "nonstd/ring_span.hpp"

// Compiler warning suppression for usage of lest:

#ifdef __clang__
# pragma clang diagnostic ignored "-Wstring-conversion"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wunused-template"
# pragma clang diagnostic ignored "-Wunused-function"
# pragma clang diagnostic ignored "-Wunused-member-function"
#elif defined __GNUC__
# pragma GCC   diagnostic ignored "-Wunused-parameter"
# pragma GCC   diagnostic ignored "-Wunused-function"
#endif

#include "lest_cpp03.hpp"

#define CASE( name ) lest_CASE( specification(), name )

// Attribute externally visible for -fwhole-program:

#if defined(__GNUC__) && !defined(__clang__)
# define any_ATTRIBUTE_EXT_VIS  __attribute__((externally_visible))
#else
# define any_ATTRIBUTE_EXT_VIS
#endif

extern lest::tests & specification() any_ATTRIBUTE_EXT_VIS;

namespace nonstd { namespace ring_span_lite {

// use oparator<< instead of to_string() overload;
// see  http://stackoverflow.com/a/10651752/437272

template< typename T, class Popper>
inline std::ostream & operator<<( std::ostream & os, ring_span<T, Popper> const & rs )
{
#if nsrs_CONFIG_STRICT_P0059
    return os << "[ring_span: ???]";
#else
    os << "[ring_span: "; std::copy( rs.begin(), rs.end(), std::ostream_iterator<T>(os, ", ") ); return os << "]";
#endif
}

namespace detail {

template< class RS, bool is_const >
inline std::ostream & operator<<( std::ostream & os, ring_iterator<RS, is_const> const & it )
{
#if nsrs_RING_SPAN_LITE_EXTENSION
    // Note: hack: depends on iterator implementation to use index [0..size());
    // create begin iterator with index 0 (it and bgn are on different containers!):
    typename RS::value_type arr[1]; ring_iterator< RS, true> bgn = RS( arr, arr + 1 ).cbegin();

    return os << "[ring_iterator: " << (it - bgn) << "]";
#else
    return os << "[ring_iterator: ?]";
#endif
}

template< class RS, bool is_const >
inline std::ostream & operator<<( std::ostream & os, std::reverse_iterator< ring_iterator<RS, is_const> > const & it )
{
#if nsrs_RING_SPAN_LITE_EXTENSION
    // Note: hack: depends on iterator implementation to use index [0..size());
    // create begin iterator with index 0 (it and bgn are on different containers!):
    typename RS::value_type arr[1]; std::reverse_iterator< ring_iterator< RS, true> > bgn = RS( arr, arr + 1 ).crbegin();

    return os << "[reverse ring_iterator: " << -(it - bgn) << "]";
#else
    return os << "[reverse ring_iterator: ?]";
#endif
}

} // namespace detail
} // namespace ring_span_lite
} // namespace nonstd

namespace lest {

using ::nonstd::ring_span_lite::operator<<;
using ::nonstd::ring_span_lite::detail::operator<<;

} // namespace lest

#endif // RING_SPAN_LITE_T_INCLUDED

// end of file
