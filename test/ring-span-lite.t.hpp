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

inline std::ostream & operator<<( std::ostream & os, ::nonstd::ring_span const & v )
{
    return os << "[ring_span:...]";
}

}

namespace lest {

using ::nonstd::operator<<;

} // namespace lest

#endif // ring_span_lite_T_INCLUDED

// end of file
