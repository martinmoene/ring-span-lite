//
// Copyright (c) 2018 Arthur O'Dwyer
// Copyright 2020-2020 by Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
//
// See https://quuxplusone.github.io/blog/2018/05/30/reference-types-with-metadata-cause-problems/
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NONSTD_RING_LITE_HPP
#define NONSTD_RING_LITE_HPP

#include <nonstd/ring_span.hpp>

#if nsrs_CPP11_OR_GREATER
# include <array>
#endif

namespace nonstd {

namespace nsrs {

namespace std11 {

template< class T, T v > struct integral_constant { enum { value = v }; };
template< bool B       > struct bool_constant : integral_constant<bool, B>{};

typedef bool_constant< true  > true_type;
typedef bool_constant< false > false_type;

template< class C >
typename C::iterator begin( C & c ) { return c.begin(); }

template< class C >
typename C::iterator end( C & c ) { return c.end(); }

template< typename T, std::size_t N >
T * begin( T (&array)[N] ) { return &array[0]; }

template< typename T, std::size_t N >
T * end( T (&array)[N] ) { return &array[N]; }

} // namespace std11

template< class Q >
struct is_array : std11::false_type {};

template< class T >
struct is_array<T[]> : std11::true_type {};

template< class T, std::size_t N >
struct is_array<T[N]> : std11::true_type {};

template< class Q >
struct is_std_array_oracle : std11::false_type{};

//#if nsrs_HAVE( ARRAY )
#if nsrs_CPP11_OR_GREATER

template< class T, std::size_t Extent >
struct is_std_array_oracle< std::array<T, Extent> > : std11::true_type{};

#endif

template< class Q >
struct is_std_array : is_std_array_oracle< Q >{};

template< typename Container >
struct vt
{
    typedef typename Container::value_type value_type;
};

template< typename T, std::size_t N >
struct vt< T[N] >
{
    typedef T value_type;
};

template
<
    typename Container /*= std::vector<T>*/
#if nsrs_RING_SPAN_LITE_EXTENSION
    , bool CapacityIsPowerOf2 = false
#endif
>
class ring
{
public:
#if nsrs_RING_SPAN_LITE_EXTENSION
    typedef ring_span<
        typename vt<Container>::value_type
        , default_popper<typename vt<Container>::value_type>
        , CapacityIsPowerOf2
    > RingSpan;
#else
    typedef ring_span< typename vt<Container>::value_type > RingSpan;
#endif

    typedef typename RingSpan::value_type       value_type;
    typedef typename RingSpan::size_type        size_type;
    typedef typename RingSpan::reference        reference;
    typedef typename RingSpan::const_reference  const_reference;
    typedef typename RingSpan::iterator         iterator;
    typedef typename RingSpan::const_iterator   const_iterator;

#if nsrs_RING_SPAN_LITE_EXTENSION
    typedef typename RingSpan::reverse_iterator         reverse_iterator;
    typedef typename RingSpan::const_reverse_iterator   const_reverse_iterator;
#endif

    // construct from C-Array, std::array:

    nsrs_REQUIRES_0((
        is_array<Container>::value
        || is_std_array<Container>::value
    ))
    explicit ring()
        : cont{}
        , rs( std11::begin(cont), std11::end(cont) )
    {}

    // construct from container not being C-Array or std::array:

    nsrs_REQUIRES_0((
        !is_array<Container>::value
        && !is_std_array<Container>::value
    ))
    explicit ring( size_type capacity )
        : cont( capacity )
        , rs( cont.begin(), cont.end() )
    {}

    // observers:

    bool empty() const nsrs_noexcept
    {
        return rs.empty();
    }

    bool full() const nsrs_noexcept
    {
        return rs.full();
    }

    size_type size() const nsrs_noexcept
    {
        return rs.size();
    }

    size_type capacity() const nsrs_noexcept
    {
        return rs.capacity();
    }

    // element access:

    reference front() nsrs_noexcept
    {
        return rs.front();
    }

    const_reference front() const nsrs_noexcept
    {
        return rs.front();
    }

    reference back() nsrs_noexcept
    {
        return rs.back();
    }

    const_reference back() const nsrs_noexcept
    {
        return rs.back();
    }

    // iteration:

    iterator begin() nsrs_noexcept
    {
        return rs.begin();
    }

    const_iterator begin() const nsrs_noexcept
    {
        return rs.begin();
    }

    const_iterator cbegin() const nsrs_noexcept
    {
        return rs.cbegin();
    }

    iterator end() nsrs_noexcept
    {
        return rs.end();
    }

    const_iterator end() const nsrs_noexcept
    {
        return rs.end();
    }

    const_iterator cend() const nsrs_noexcept
    {
        return rs.cend();
    }

#if nsrs_RING_SPAN_LITE_EXTENSION

    reverse_iterator rbegin() nsrs_noexcept
    {
        return rs.rbegin();
    }

    reverse_iterator rend() nsrs_noexcept
    {
        return rs.rend();
    }

    const_reverse_iterator rbegin() const nsrs_noexcept
    {
        return rs.rbegin();
    }

    const_reverse_iterator rend() const nsrs_noexcept
    {
        return rs.rend();
    }

    const_reverse_iterator crbegin() const nsrs_noexcept
    {
        return rs.crbegin();
    }

    const_reverse_iterator crend() const nsrs_noexcept
    {
        return rs.crend();
    }
#endif

#if nsrs_RING_SPAN_LITE_EXTENSION
    reference operator[]( size_type idx ) nsrs_noexcept
    {
        return rs[ idx ];
    }

    const_reference operator[]( size_type idx ) const nsrs_noexcept
    {
        return rs[ idx ];
    }
#endif

    // element insertion, extraction:

    value_type pop_front()
    {
        return rs.pop_front();
    }

#if nsrs_RING_SPAN_LITE_EXTENSION
    value_type pop_back()
    {
        return rs.pop_back();
    }
#endif

#if nsrs_CPP11_OR_GREATER
    nsrs_REQUIRES_0( std::is_copy_assignable<value_type>::value )
    void push_back( value_type const & value) noexcept( std::is_nothrow_copy_assignable<value_type>::value )
#else
    void push_back( value_type const & value )
#endif
    {
        rs.push_back( value );
    }

#if nsrs_CPP11_OR_GREATER
    nsrs_REQUIRES_0( std::is_move_assignable<value_type>::value )
    void push_back( value_type && value ) noexcept( std::is_nothrow_move_assignable<value_type>::value )
    {
        rs.push_back( std::move( value ) );
    }

    template< typename... Args
        nsrs_REQUIRES_T(
            std::is_constructible<value_type, Args&&...>::value
            && std::is_move_assignable<value_type>::value
        )
    >
    void emplace_back( Args &&... args ) noexcept
    (
        std::is_nothrow_constructible<value_type, Args...>::value
        && std::is_nothrow_move_assignable<value_type>::value
    )
    {
        rs.emplace_back( std::forward<Args>(args)... );
    }
#endif

#if nsrs_RING_SPAN_LITE_EXTENSION

#if nsrs_CPP11_OR_GREATER
    nsrs_REQUIRES_0( std::is_copy_assignable<value_type>::value )
    void push_front( value_type const & value ) noexcept(( std::is_nothrow_copy_assignable<value_type>::value ))
#else
    void push_front( value_type const & value )
#endif
    {
        rs.push_front( value );
    }

#if nsrs_CPP11_OR_GREATER
    nsrs_REQUIRES_0( std::is_move_assignable<value_type>::value )
    void push_front( value_type && value ) noexcept(( std::is_nothrow_move_assignable<value_type>::value ))
    {
        rs.push_front( std::move( value ) );
    }

    template< typename... Args
        nsrs_REQUIRES_T(
            std::is_constructible<value_type, Args&&...>::value
            && std::is_move_assignable<value_type>::value
        )
    >
    void emplace_front( Args&&... args ) noexcept
    (
        std::is_nothrow_constructible<value_type, Args...>::value
        && std::is_nothrow_move_assignable<value_type>::value
    )
    {
        rs.emplace_front( std::forward<Args>(args)... );
    }
#endif
#endif // nsrs_RING_SPAN_LITE_EXTENSION

    // swap:

    void swap( ring & rhs )
#if nsrs_CPP11_OR_GREATER
        noexcept(
            nonstd::ring_span_lite::std17::is_nothrow_swappable<typename RingSpan::Popper>::value
        )
#endif
    {
        rhs.swap( *this );
    }

private:
    Container cont;
    RingSpan  rs;
};

} // namespace nsrs

// Make types available in namespace nonstd:

using nsrs::ring;

} // namespace nonstd

#endif // NONSTD_RING_LITE_HPP
