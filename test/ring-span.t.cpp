//
// ring-span-lite, a C++yy-like ring span type for C++98 and later.
// For more information see https://github.com/martinmoene/ring-span-lite
//
// Copyright 2017-2018 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef  nsrs_RING_SPAN_LITE_T_HEADER
#define  nsrs_RING_SPAN_LITE_T_HEADER  "ring-span-main.t.hpp"
#endif

#include nsrs_RING_SPAN_LITE_T_HEADER

#include <numeric>

using namespace nonstd;

typedef ring_span<int>::size_type size_type;
typedef ring_span<int>::iterator::difference_type difference_type;

#if nsrs_CPP11_OR_GREATER

template< typename T, size_t N >
inline constexpr size_t dim( T (&arr)[N] )
{
    return N;
}
#else
# define dim(arr) ( sizeof(arr) / sizeof(0[arr]) )
#endif

namespace tst {

// std::equal() requires iterator::operator-(),
// which is not available with nsrs_CONFIG_STRICT_P0059

template< typename InputIt1, typename InputIt2 >
bool equal( InputIt1 first1, InputIt1 last1, InputIt2 first2 )
{
#if nsrs_CONFIG_STRICT_P0059
    return true;
#else
    return std::equal( first1, last1, first2 );
#endif
}

template< typename InputIt1, typename InputIt2, typename T >
T inner_product( InputIt1 first1, InputIt1 last1, InputIt2 first2, T init )
{
#if nsrs_CONFIG_STRICT_P0059
    return init;
#else
    return std::inner_product( first1, last1, first2, init );
#endif
}

} // namespace tst

#if nsrs_CPP11_OR_GREATER
struct noncopyable
{
    char c; int i;
    noncopyable() : c(), i() {}
    noncopyable( char k, int x ) : c(k), i(x) {}
    noncopyable( noncopyable && ) = default;
    noncopyable& operator=( noncopyable && ) = default;

    noncopyable( noncopyable const & ) = delete;
    noncopyable& operator=( noncopyable const & ) = delete;
};

struct oracle
{
    enum state_t { init, moved_from, };

    static state_t & state() { static state_t s = init; return s; }
    static int     & value() { static int     v = 0;    return v; }

    oracle( int v = 0 ) noexcept { state() = init; value() = v; }
    oracle( oracle && ) noexcept { state() = moved_from; }
    oracle & operator=( oracle && ) noexcept { state() = moved_from; return *this; }
};
#endif

CASE( "ring_span: Allows to construct an empty span from an iterator pair" )
{
    int arr[] = { 1, 2, 3, };

    ring_span<int> rs( &arr[0], &arr[0] + dim(arr) );

    EXPECT( rs.size()     == size_type(0) );
    EXPECT( rs.capacity() == dim(arr)     );
}

CASE( "ring_span: Allows to construct an empty span from an iterator pair - capacity is power of 2" )
{
#if nsrs_RING_SPAN_LITE_EXTENSION
    int arr[] = { 1, 2, 3, 4, };

    ring_span<int, nonstd::default_popper<int>, true> rs( &arr[0], &arr[0] + dim(arr) );

    EXPECT( rs.size()     == size_type(0) );
    EXPECT( rs.capacity() == dim(arr)     );
#else
    EXPECT( !!"'size is power of 2' is not available (nsrs_CONFIG_STRICT_P0059)" );
#endif
}

CASE( "ring_span: Allows to construct a partially filled span from an iterator pair and iterator, size" )
{
    int arr[] = { 7, 7, 1, 2, 3, 7, 7, };
    size_type first = 2;
    size_type count = 3;

    ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0] + first, count );

    EXPECT( rs.size()     == count    );
    EXPECT( rs.capacity() == dim(arr) );
    EXPECT( tst::equal( rs.begin(), rs.end(), &arr[0] + first ) );
}

CASE( "ring_span: Allows to construct a partially filled span from an iterator pair and iterator, size - capacity is power of 2" )
{
#if nsrs_RING_SPAN_LITE_EXTENSION
    int arr[] = { 7, 7, 1, 2, 3, 7, 7, 8, };
    size_type first = 2;
    size_type count = 3;

    ring_span<int, nonstd::default_popper<int>, true> rs( &arr[0], &arr[0] + dim(arr), &arr[0] + first, count );

    EXPECT( rs.size()     == count    );
    EXPECT( rs.capacity() == dim(arr) );
    EXPECT( tst::equal( rs.begin(), rs.end(), &arr[0] + first ) );
#else
    EXPECT( !!"'size is power of 2' is not available (nsrs_CONFIG_STRICT_P0059)" );
#endif

}

CASE( "ring_span: Constructing a span with size exceeding capacity asserts m_size <= m_capacity" "[.assert]" )
{
    int arr[] = { 1, 2, 3, 4, 5 };

    ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) + 1 );
}

CASE( "ring_span: Disallows to copy-construct from a ring_span (compile-time)" )
{
#if nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS
    int arr[] = { 1, 2, 3, }; ring_span<int> rs1( &arr[0], &arr[0] + dim(arr) );

    ring_span<int> rs2( rs1 );
#else
    EXPECT( !!"ring_span: check for compile-time error (define nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS)" );
#endif
}

CASE( "ring_span: Disallows to copy-assign from a ring_span (compile-time)" )
{
#if nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS
    int arr[] = { 1, 2, 3, };
    ring_span<int> rs1( &arr[0], &arr[0] + dim(arr) );
    ring_span<int> rs2( &arr[0], &arr[0] + dim(arr) );

    rs2 = rs1;
#else
    EXPECT( !!"ring_span: check for compile-time error (define nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS)" );
#endif
}

CASE( "ring_span: Allows to move-construct from a ring_span (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    int arr[] = { 1, 2, 3, };

    ring_span<int> rs( ring_span<int>( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) ) );

    EXPECT( rs.size()     == dim(arr) );
    EXPECT( rs.capacity() == dim(arr) );
    EXPECT( tst::equal( rs.begin(), rs.end(), &arr[0] ) );
#else
    EXPECT( !!"ring_span: move-construction is not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to move-assign from a ring_span (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr) );

    rs = ring_span<int>( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.size()     == dim(arr) );
    EXPECT( rs.capacity() == dim(arr) );
    EXPECT( tst::equal( rs.begin(), rs.end(), &arr[0] ) );
#else
    EXPECT( !!"ring_span: move-assignment is not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to obtain the capacity of a span" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr) );

    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Allows to obtain the number of elements in a span (size)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.size() == rs.capacity() );
}

CASE( "ring_span: Allows to check for an empty span" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr) );

    EXPECT( rs.empty() );
}

CASE( "ring_span: Allows to check for a full span" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.full() );
}

CASE( "ring_span: Allows to observe the element at the specified index" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"operator[] is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs[0] == arr[0] );
    EXPECT( rs[1] == arr[1] );
    EXPECT( rs[2] == arr[2] );
#endif
}

CASE( "ring_span: Allows to observe the element at the front" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.front() == arr[0] );
}

CASE( "ring_span: Allows to observe the element at the back" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.back() == arr[dim(arr) - 1] );
}

CASE( "ring_span: Allows to obtain and remove the element at the front" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.pop_front() == arr[0] );
}

CASE( "ring_span: Allows to obtain and remove the element at the back" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"pop_back() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.pop_back() == arr[dim(arr) - 1] );
#endif
}

CASE( "ring_span: Allows to copy-insert an element at the front" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"push_front() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    int new_element = 7;

    rs.push_front( new_element );

    EXPECT( rs.front() == new_element );
#endif
}

CASE( "ring_span: Allows to move-insert an element at the front (C++11)" " [extension]" )
{
#if nsrs_CPP11_OR_GREATER
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"push_front() is not available (SG14)" );
#else
    oracle arr[3]; ring_span<oracle> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    int value = 7;

    rs.push_front( oracle(value) );

    EXPECT( oracle::state()    == oracle::moved_from );
    EXPECT( rs.front().value() == value              );
#endif
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to emplace an element at the front (C++11)" " [extension]" )
{
#if nsrs_CPP11_OR_GREATER
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"emplace_front() is not available (SG14)" );
#else
    SETUP("") {
    SECTION("of non-full ring_span")
    {
        noncopyable arr[3]; ring_span<noncopyable> rs( &arr[0], &arr[0] + dim(arr) );

        rs.emplace_front( 'a', 7 );

        EXPECT( rs.front().c == 'a' );
        EXPECT( rs.front().i ==  7  );
    }
    SECTION("of full ring_span")
    {
        noncopyable arr[3]; ring_span<noncopyable> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
        EXPECT( rs.front().c == char() );
        EXPECT( rs.front().i ==  int() );

        rs.emplace_front( 'a', 7 );

        EXPECT( rs.front().c == 'a' );
        EXPECT( rs.front().i ==  7  );
    }}
#endif
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to copy-insert an element at the back" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    int new_element = 7;

    rs.push_back( new_element );

    EXPECT( rs.back() == new_element );
}

CASE( "ring_span: Allows to move-insert an element at the back (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    oracle arr[3]; ring_span<oracle> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    int value = 7;

    rs.push_back( oracle(value) );

    EXPECT( oracle::state()   == oracle::moved_from );
    EXPECT( rs.back().value() == value              );
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to emplace an element at the back (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    SETUP("") {
    SECTION("of non-full ring_span")
    {
        noncopyable arr[3]; ring_span<noncopyable> rs( &arr[0], &arr[0] + dim(arr) );

        rs.emplace_back( 'a', 7 );

        EXPECT( rs.back().c == 'a' );
        EXPECT( rs.back().i ==  7  );
    }
    SECTION("of full ring_span")
    {
        noncopyable arr[3]; ring_span<noncopyable> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
        EXPECT( rs.back().c == char() );
        EXPECT( rs.back().i ==  int() );

        rs.emplace_back( 'a', 7 );

        EXPECT( rs.back().c == 'a' );
        EXPECT( rs.back().i ==  7  );
    }}
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "ring_span: Adding an element to an empty span makes it non-empty (front)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"push_front() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr) );
    EXPECT( rs.empty() );

    rs.push_front( 7 );

    EXPECT( ! rs.empty() );
#endif
}

CASE( "ring_span: Adding an element to an empty span makes it non-empty (back)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr) );
    EXPECT( rs.empty() );

    rs.push_back( 7 );

    EXPECT( ! rs.empty() );
}

CASE( "ring_span: Adding an element to an empty span doesn't change its capacity (front)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"push_front() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr) );
    EXPECT( rs.empty()                );
    EXPECT( rs.capacity() == dim(arr) );

    rs.push_front( 7 );

    EXPECT( rs.capacity() == dim(arr) );
#endif
}

CASE( "ring_span: Adding an element to an empty span doesn't change its capacity (back)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr) );
    EXPECT( rs.empty()                );
    EXPECT( rs.capacity() == dim(arr) );

    rs.push_back( 7 );

    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Adding an element to a full span leaves it full (front)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"push_front() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full() );

    rs.push_front( 7 );

    EXPECT( rs.full() );
#endif
}

CASE( "ring_span: Adding an element to a full span leaves it full (back)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full() );

    rs.push_back( 7 );

    EXPECT( rs.full() );
}

CASE( "ring_span: Adding an element to a full span doesn't change its capacity (front)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"push_front() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full()                 );
    EXPECT( rs.capacity() == dim(arr) );

    rs.push_front( 7 );

    EXPECT( rs.capacity() == dim(arr) );
#endif
}

CASE( "ring_span: Adding an element to a full span doesn't change its capacity (back)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full()                 );
    EXPECT( rs.capacity() == dim(arr) );

    rs.push_back( 7 );

    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Removing an element from an empty span asserts !empty (front)" "[.assert]" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr) );
    EXPECT( rs.empty() );

    (void) rs.pop_front();
}

CASE( "ring_span: Removing an element from an empty span asserts !empty (back)"  "[extension][.assert]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"pop_back() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr) );
    EXPECT( rs.empty() );

    (void) rs.pop_back();
#endif
}

CASE( "ring_span: Removing an element from a span with one element makes it empty (front)" )
{
    int arr[] = { 1, };
    ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full() );

    (void) rs.pop_front();

    EXPECT( rs.empty() );
}

CASE( "ring_span: Removing an element from a span with one element makes it empty (back)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"pop_back() is not available (SG14)" );
#else
    int arr[] = { 1, };
    ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full() );

    (void) rs.pop_back();

    EXPECT( rs.empty() );
#endif
}

CASE( "ring_span: Removing an element from a span with one element doesn't change its capacity (front)" )
{
    int arr[] = { 1, };
    ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full() );
    EXPECT( rs.capacity() == dim(arr) );

    (void) rs.pop_front();

    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Removing an element from a span with one element doesn't change its capacity (back)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"pop_back() is not available (SG14)" );
#else
    int arr[] = { 1, };
    ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full() );
    EXPECT( rs.capacity() == dim(arr) );

    (void) rs.pop_back();

    EXPECT( rs.capacity() == dim(arr) );
#endif
}

CASE( "ring_span: Removing an element from a full span makes it not full (front)" )
{
    int arr[] = { 1, 2, 3, };
    ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full() );

    (void) rs.pop_front();

    EXPECT( ! rs.full() );
}

CASE( "ring_span: Removing an element from a full span makes it not full (back)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"pop_back() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, };
    ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full() );

    (void) rs.pop_back();

    EXPECT( ! rs.full() );
#endif
}

CASE( "ring_span: Removing an element from a full span doesn't change its capacity (front)" )
{
    int arr[] = { 1, 2, 3, };
    ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full() );
    EXPECT( rs.capacity() == dim(arr) );

    (void) rs.pop_front();

    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Removing an element from a full span doesn't change its capacity (back)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"pop_back() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, };
    ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( rs.full() );
    EXPECT( rs.capacity() == dim(arr) );

    (void) rs.pop_back();

    EXPECT( rs.capacity() == dim(arr) );
#endif
}

CASE( "ring_span: Allows to swap spans (member)" )
{
    int arr1[] = { 1, 2, 3, }; ring_span<int> rs1( &arr1[0], &arr1[0] + dim(arr1), &arr1[0], dim(arr1) );
    int arr2[] = { 9, 8, 7, }; ring_span<int> rs2( &arr2[0], &arr2[0] + dim(arr2), &arr2[0], dim(arr2) );

    rs1.swap( rs2 );

    EXPECT( tst::equal( rs1.begin(), rs1.end(), &arr2[0] ) );
    EXPECT( tst::equal( rs2.begin(), rs2.end(), &arr1[0] ) );
}

CASE( "ring_span: Allows to swap spans (non-member)" )
{
    int arr1[] = { 1, 2, 3, }; ring_span<int> rs1( &arr1[0], &arr1[0] + dim(arr1), &arr1[0], dim(arr1) );
    int arr2[] = { 9, 8, 7, }; ring_span<int> rs2( &arr2[0], &arr2[0] + dim(arr2), &arr2[0], dim(arr2) );

    using std::swap;
    swap( rs1, rs2 );

    EXPECT( tst::equal( rs1.begin(), rs1.end(), &arr2[0] ) );
    EXPECT( tst::equal( rs2.begin(), rs2.end(), &arr1[0] ) );
}

CASE( "ring_span: Allows to appear in range-for (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    std::vector<int> vec;
    for ( auto x : rs )
    {
        vec.push_back( x );
    }

    EXPECT( tst::equal( vec.begin(), vec.end(), &arr[0] ) );
#else
    EXPECT( !!"range-for is not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows iteration (non-const)" )
{
    int arr[] = { 1, 2, 3, };
    ring_span<int>        rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int> const crs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( tst::equal(  rs.begin(),  rs.end(), &arr[0] ) );
    EXPECT( tst::equal( crs.begin(), crs.end(), &arr[0] ) );
}

CASE( "ring_span: Allows iteration (const)" )
{
    int arr[] = { 1, 2, 3, };
    ring_span<int>        rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int> const crs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( tst::equal(  rs.cbegin(),  rs.cend(), &arr[0] ) );
    EXPECT( tst::equal( crs.cbegin(), crs.cend(), &arr[0] ) );
}

CASE( "ring_span: Allows iteration (mixed const-non-const)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator bgn = rs.begin();

    EXPECT( bgn < rs.cend() );
    EXPECT( tst::equal( rs.cbegin(), rs.cend(), bgn ) );
}

CASE( "ring_span: Allows reverse iteration (non-const)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"rbegin(), rend() are not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, };
    ring_span  <int> rs ( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    std::vector<int> vec( &arr[0], &arr[0] + dim(arr) );

    EXPECT( tst::equal( rs.rbegin(), rs.rend(), vec.rbegin() ) );
#endif
}

CASE( "ring_span: Allows reverse iteration (const)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"crbegin(), crend() are not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, };
    ring_span  <int> rs ( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    std::vector<int> vec( &arr[0], &arr[0] + dim(arr) );

    EXPECT( tst::equal( rs.crbegin(), rs.crend(), vec.rbegin() ) );
#endif
}

CASE( "ring_span: Allows reverse iteration (mixed const-non-const)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"rbegin(), rend() are not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, };
    ring_span  <int> rs ( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::reverse_iterator rbgn = rs.rbegin();

    EXPECT( tst::equal( rs.crbegin(), rs.crend(), rbgn ) );
#endif
}

CASE( "ring_span: A span with capacity zero is both empty and full" )
{
    int arr[] = { 1, }; ring_span<int> rs( &arr[0], &arr[0] + 0 );

    EXPECT( rs.empty() );
    EXPECT( rs.full()  );
}

CASE( "ring_span: A full span is a delay-line of capacity elements (back-front)" )
{
    size_type arr[] = { 1, 2, 3, }; ring_span<size_type> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    for ( size_type x = 4; x < 10; rs.push_back( x ), ++x )
    {
        EXPECT( rs.pop_front() == x - rs.capacity() );
    }
}

CASE( "ring_span: A full span is a delay-line of capacity elements (front-back)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"push_front(), pop_back() are not available (SG14)" );
#else
    size_type arr[] = { 3, 2, 1, }; ring_span<size_type> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    for ( size_type x = 4; x < 10; rs.push_front( x ), ++x )
    {
        EXPECT( rs.pop_back() == x - rs.capacity() );
    }
#endif
}

CASE( "ring_span: A non-full span is a stack of capacity elements (back)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"pop_back() is not available (SG14)" );
#else
    size_type arr[] = { 7, 7, 7, }; ring_span<size_type> rs( &arr[0], &arr[0] + dim(arr) );

    for ( size_type x = 1; x <= 3; ++x )
    {
        rs.push_back( x );
    }
    for ( size_type x = 3; x != 0 ; --x )
    {
        EXPECT( x == rs.size()     );
        EXPECT( x == rs.pop_back() );
    }
#endif
}

CASE( "ring_span: A non-full span is a stack of capacity elements (front)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"push_front() is not available (SG14)" );
#else
    size_type arr[] = { 7, 7, 7, }; ring_span<size_type> rs( &arr[0], &arr[0] + dim(arr) );

    for ( size_type x = 1; x <= 3; ++x )
    {
        rs.push_front( x );
    }
    for ( size_type x = 3; x >= 1 ; --x )
    {
        EXPECT( x == rs.size()      );
        EXPECT( x == rs.pop_front() );
    }
#endif
}

CASE( "ring_span: A non-full span behaves like an harmonica (back-front)" )
{
    size_type arr[] = { 7, 7, 7, }; ring_span<size_type> rs( &arr[0], &arr[0] + dim(arr) );

    for ( size_type x = 0; x < 3; rs.push_back( x ), ++x )
    {
        EXPECT( x == rs.size()  );
    }
    for ( size_type x = 0; x < 3; ++x )
    {
        EXPECT( x == 3 - rs.size()  );
        EXPECT( x == rs.pop_front() );
    }
}

CASE( "ring_span: A non-full span behaves like an harmonica (front-back)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"push_front(), pop_back() are not available (SG14)" );
#else
    size_type arr[] = { 7, 7, 7, }; ring_span<size_type> rs( &arr[0], &arr[0] + dim(arr) );

    for ( size_type x = 0; x < 3; rs.push_front( x ), ++x )
    {
        EXPECT( x == rs.size() );
    }
    for ( size_type x = 0; x < 3; ++x )
    {
        EXPECT( x == 3 - rs.size() );
        EXPECT( x == rs.pop_back() );
    }
#endif
}

CASE( "ring_iterator: Allows conversion to const ring_iterator" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"conversion to const iterator is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator bgn = rs.begin();

    ring_span<int>::const_iterator cbgn = bgn;

    EXPECT( tst::equal( cbgn, rs.cend(), rs.cbegin() ) );
#endif
}

CASE( "ring_iterator: Allows to dereference iterator (operator*())" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( *rs.begin() == arr[0] );
}

namespace op_arrow { struct S { int v; }; }

CASE( "ring_iterator: Allows to dereference iterator (operator->())" )
{
    using op_arrow::S;

    S arr[] = { {1}, {2}, {3}, }; ring_span<S> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.begin()->v == arr[0].v );
}

CASE( "ring_iterator: Allows to index from iterator (operator[](size_t))" )
{
    using op_arrow::S;

    S arr[] = { {1}, {2}, {3}, }; ring_span<S> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.begin()[0].v == arr[0].v );
    EXPECT( rs.begin()[1].v == arr[1].v );
    EXPECT( (rs.begin() + 1)[0].v == arr[1].v );
    EXPECT( (rs.begin() + 1)[1].v == arr[2].v );
    // explicit const iterator
    EXPECT( rs.cbegin()[0].v == arr[0].v );
}

CASE( "ring_iterator: Allows to increment iterator (prefix)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator pos = rs.begin();

    EXPECT( *++pos == arr[1] );
}

CASE( "ring_iterator: Allows to increment iterator (postfix)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator pos = rs.begin();

    EXPECT( *pos++ == arr[0] );
}

CASE( "ring_iterator: Allows to decrement iterator (prefix)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator pos = ++rs.begin();

    EXPECT( *--pos == arr[0] );
}

CASE( "ring_iterator: Allows to decrement iterator (postfix)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator pos = ++rs.begin();

    EXPECT( *pos-- == arr[1] );
}

CASE( "ring_iterator: Allows to advance iterator (+=)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"iterator advancement is not implemented properly (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator pos = rs.begin();

    EXPECT( *(pos += 2) == arr[2] );
#endif
}

CASE( "ring_iterator: Allows to advance iterator (-=)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"iterator advancement is not implemented properly (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator pos = ++rs.begin(); ++pos;

    EXPECT( *(pos -= 2) == arr[0] );
#endif
}

CASE( "ring_iterator: Allows to offset iterator (+)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"iterator advancement is not implemented properly (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator pos = rs.begin();

    EXPECT( *(pos + 2) == arr[2] );
    EXPECT( *(2 + pos) == arr[2] );
#endif
}

CASE( "ring_iterator: Allows to offset iterator (-)" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"iterator advancement is not implemented properly (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator pos = ++rs.begin(); ++pos;

    EXPECT( *(pos - 2) == arr[0] );
#endif
}

CASE( "ring_iterator: Allows to obtain difference of iterators" " [extension]" )
{
#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"iterator difference is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.end() - rs.begin() == difference_type( dim(arr) ) );
#endif
}

CASE( "ring_iterator: Allows to compare iterators (==)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.begin()   == rs.begin()   );
    EXPECT( rs.cbegin()  == rs.cbegin()  );

#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"rbegin(), rend() are not available (SG14)" );
#else
    EXPECT( rs.rbegin()  == rs.rbegin()  );
    EXPECT( rs.crbegin() == rs.crbegin() );
#endif
}

CASE( "ring_iterator: Allows to compare iterators (!=)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.begin()   != rs.end() );
    EXPECT( rs.cbegin()  != rs.cend()  );

#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"rbegin(), rend() are not available (SG14)" );
#else
    EXPECT( rs.rbegin()  != rs.rend()  );
    EXPECT( rs.crbegin() != rs.crend() );
#endif
}

CASE( "ring_iterator: Allows to compare iterators (<)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.begin()   < rs.end()   );
    EXPECT( rs.cbegin()  < rs.cend()  );

#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"rbegin(), rend() are not available (SG14)" );
#else
    EXPECT( rs.rbegin()  < rs.rend()  );
    EXPECT( rs.crbegin() < rs.crend() );
#endif
}

CASE( "ring_iterator: Allows to compare iterators (<=)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.begin()   <= rs.end()     );
    EXPECT( rs.begin()   <= rs.begin()   );
    EXPECT( rs.cbegin()  <= rs.cend()    );
    EXPECT( rs.cbegin()  <= rs.cbegin()  );

#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"rbegin(), rend() are not available (SG14)" );
#else
    EXPECT( rs.rbegin()  <= rs.rend()    );
    EXPECT( rs.rbegin()  <= rs.rbegin()  );
    EXPECT( rs.crbegin() <= rs.crend()   );
    EXPECT( rs.crbegin() <= rs.crbegin() );
#endif
}

CASE( "ring_iterator: Allows to compare iterators (>)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.end()   > rs.begin()   );
    EXPECT( rs.cend()  > rs.cbegin()  );

#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"rbegin(), rend() are not available (SG14)" );
#else
    EXPECT( rs.rend()  > rs.rbegin()  );
    EXPECT( rs.crend() > rs.crbegin() );
#endif
}

CASE( "ring_iterator: Allows to compare iterators (>=)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.end()     >= rs.begin()   );
    EXPECT( rs.begin()   >= rs.begin()   );
    EXPECT( rs.cend()    >= rs.cbegin()  );
    EXPECT( rs.cbegin()  >= rs.cbegin()  );

#if nsrs_CONFIG_STRICT_P0059
    EXPECT( !!"rbegin(), rend() are not available (SG14)" );
#else
    EXPECT( rs.rend()    >= rs.rbegin()  );
    EXPECT( rs.rbegin()  >= rs.rbegin()  );
    EXPECT( rs.crend()   >= rs.crbegin() );
    EXPECT( rs.crbegin() >= rs.crbegin() );
#endif
}

CASE( "ring_iterator: Allows to compare iterators (mixed const-non-const)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    ring_span<int>::iterator bgn = rs.begin();

    EXPECT(    bgn    != rs.cend() );
    EXPECT(    bgn    <  rs.cend() );
    EXPECT(    bgn    <= rs.cend() );
    EXPECT( rs.cend() >     bgn    );
    EXPECT( rs.cend() >=    bgn    );
}

CASE( "null_popper: A null popper returns void" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int, null_popper<int> > rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    rs.pop_front();
}

CASE( "null_popper: A null popper leaves the original element unchanged" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int, null_popper<int> > rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    rs.pop_front();

    EXPECT( arr[0] == 1 );
}

CASE( "default_popper: A default popper returns the element" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int, default_popper<int> > rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    EXPECT( rs.pop_front() == arr[0] );
}

CASE( "default_popper: A default popper moves the element (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    oracle arr[1]; ring_span<oracle, default_popper<oracle> > rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );
    EXPECT( oracle::state() == oracle::init );

    (void) rs.pop_front();

    EXPECT( oracle::state() == oracle::moved_from );
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "default_popper: A default popper leaves the original element unchanged" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int, default_popper<int> > rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    (void) rs.pop_front();

    EXPECT( arr[0] == 1 );
}

CASE( "copy_popper: A copy popper returns the element" )
{
#if nsrs_CONFIG_STRICT_P0059
    int arr[] = { 1, 2, 3, }; ring_span<int, copy_popper<int> > rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr), copy_popper<int>(7) );
#else
    int replacement = 7;
    int arr[] = { 1, 2, 3, }; ring_span<int, copy_popper<int> > rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr), copy_popper<int>(replacement) );
#endif
    int result = rs.pop_front() ;

    EXPECT( result == 1 );

}

CASE( "copy_popper: A copy popper replaces the original element" )
{
#if nsrs_CONFIG_STRICT_P0059
    int arr[] = { 1, 2, 3, }; ring_span<int, copy_popper<int> > rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr), copy_popper<int>(7) );

    (void) rs.pop_front() ;

    EXPECT( arr[0] == 7 );
#else
    int replacement = 7;
    int arr[] = { 1, 2, 3, }; ring_span<int, copy_popper<int> > rs( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr), copy_popper<int>(replacement) );

    (void) rs.pop_front() ;

    EXPECT( arr[0] == replacement );
#endif
}

// ring:

#include <vector>

CASE( "ring: Allows to create data owning ring from container" )
{
    ring< std::vector<int> > r( 3 );

    EXPECT( r.size() == 0u );
    EXPECT( r.capacity() == 3u );

    // overflow ring by one element:

    for ( size_t i = 0; i < r.size() + 1; ++i )
    {
        r.push_back( static_cast<int>( i ) );
    }

    EXPECT( r.size() == 3u );
    EXPECT( r.capacity() == 3u );

    EXPECT( r.front() == 1 );
    EXPECT( r.back()  == 3 );
#if nsrs_RING_SPAN_LITE_EXTENSION
    EXPECT( r[1]      == 2 );
#endif

    EXPECT( r.pop_front() == 1 );
    EXPECT( r.pop_front() == 2 );
    EXPECT( r.pop_front() == 3 );

    EXPECT( r.size() == 0u );

    unsigned int count = 0u;

#if nsrs_RING_SPAN_LITE_EXTENSION
    ++count;
    r.push_front( 42 );
    EXPECT( r.front() == 42 );
#endif

#if nsrs_CPP11_OR_GREATER
    ++count;
    r.emplace_back(  77 );
#if nsrs_RING_SPAN_LITE_EXTENSION
    ++count;
    r.emplace_front( 55 );
    EXPECT( r.front() == 55 );
#endif
    EXPECT( r.back () == 77 );
#endif
    EXPECT( r.size() == count );
}

CASE( "ring: Allows to create data owning ring from container - capacity is power of 2" )
{
#if nsrs_RING_SPAN_LITE_EXTENSION
    ring< std::vector<int>, true > r( 4 );

    EXPECT( r.size() == 0u );
    EXPECT( r.capacity() == 4u );

    // overflow ring by one element:

    for ( size_t i = 0; i < r.size() + 1; ++i )
    {
        r.push_back( static_cast<int>( i ) );
    }

    EXPECT( r.size() == 4u );
    EXPECT( r.capacity() == 4u );

    EXPECT( r.front() == 1 );
    EXPECT( r.back()  == 4 );
    EXPECT( r[1]      == 2 );

    EXPECT( r.pop_front() == 1 );
    EXPECT( r.pop_front() == 2 );
    EXPECT( r.pop_front() == 3 );
    EXPECT( r.pop_front() == 4 );

    EXPECT( r.size() == 0u );

    r.push_front( 42 );
    EXPECT( r.front() == 42 );

#if nsrs_CPP11_OR_GREATER
    r.emplace_front( 55 );
    r.emplace_back(  77 );
    EXPECT( r.front() == 55 );
    EXPECT( r.back () == 77 );

    EXPECT( r.size() == 3u );
#endif
#else
    EXPECT( !!"'size is power of 2' is not available (nsrs_CONFIG_STRICT_P0059)" );
#endif
}

CASE( "tweak header: reads tweak header if supported " "[tweak]" )
{
#if nsrs_HAVE_TWEAK_HEADER
    EXPECT( RING_SPAN_TWEAK_VALUE == 42 );
#else
    EXPECT( !!"Tweak header is not available (nsrs_HAVE_TWEAK_HEADER: 0)." );
#endif
}

//------------------------------------------------------------------------
// Issues:

//------------------------------------------------------------------------
// Applets:

#include <iostream>

CASE( "ring_span: filter" "[.applet]" )
{
    double arr[]   = { 2.0 , 3.0, 5.0, };
    double coeff[] = { 0.25, 0.5, 0.25 };

    ring_span<double> buffer( &arr[0], &arr[0] + dim(arr), &arr[0], dim(arr) );

    std::cout << buffer << "\n";

    // new sample:
    buffer.push_back( 7.0 );

    std::cout << buffer << "\n";

    double result = tst::inner_product( buffer.begin(), buffer.end(), coeff, 0.0 );

    std::cout << "filter result: " << result << "\n";

    EXPECT( result == lest::approx(5.0) );
}

// end of file
