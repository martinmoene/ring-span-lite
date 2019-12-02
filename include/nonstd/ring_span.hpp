//
// Copyright (c) 2015 Arthur O'Dwyer
// Copyright 2017-2019 by Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef NONSTD_RING_SPAN_LITE_HPP
#define NONSTD_RING_SPAN_LITE_HPP

#define ring_span_lite_MAJOR  0
#define ring_span_lite_MINOR  3
#define ring_span_lite_PATCH  0

#define ring_span_lite_VERSION  nsrs_STRINGIFY(ring_span_lite_MAJOR) "." nsrs_STRINGIFY(ring_span_lite_MINOR) "." nsrs_STRINGIFY(ring_span_lite_PATCH)

#define nsrs_STRINGIFY(  x )  nsrs_STRINGIFY_( x )
#define nsrs_STRINGIFY_( x )  #x

// ring-span-lite configuration:

#define nsrs_RING_SPAN_DEFAULT  0
#define nsrs_RING_SPAN_NONSTD   1
#define nsrs_RING_SPAN_STD      2

#if !defined( nsrs_CONFIG_SELECT_RING_SPAN )
# define nsrs_CONFIG_SELECT_RING_SPAN  ( nsrs_HAVE_STD_RING_SPAN ? nsrs_RING_SPAN_STD : nsrs_RING_SPAN_NONSTD )
#endif

#ifndef  nsrs_CONFIG_STRICT_P0059
# define nsrs_CONFIG_STRICT_P0059  0
#endif

#define nsrs_RING_SPAN_LITE_EXTENSION  (! nsrs_CONFIG_STRICT_P0059)

#ifndef  nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS
# define nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS  0
#endif

// C++ language version detection (C++20 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef   nsrs_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(__clang__)
#  define nsrs_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define nsrs_CPLUSPLUS  __cplusplus
# endif
#endif

#define nsrs_CPP98_OR_GREATER  ( nsrs_CPLUSPLUS >= 199711L )
#define nsrs_CPP11_OR_GREATER  ( nsrs_CPLUSPLUS >= 201103L )
#define nsrs_CPP11_OR_GREATER_ ( nsrs_CPLUSPLUS >= 201103L )
#define nsrs_CPP14_OR_GREATER  ( nsrs_CPLUSPLUS >= 201402L )
#define nsrs_CPP17_OR_GREATER  ( nsrs_CPLUSPLUS >= 201703L )
#define nsrs_CPP20_OR_GREATER  ( nsrs_CPLUSPLUS >= 202000L )

// Use C++XX std::ring_span if available and requested:

#define  nsrs_HAVE_STD_RING_SPAN  0

//#if nsrs_CPP17_OR_GREATER && defined(__has_include )
//# if __has_include( <any> )
//#  define nsrs_HAVE_STD_RING_SPAN  1
//# else
//#  define nsrs_HAVE_STD_RING_SPAN  0
//# endif
//#else
//# define  nsrs_HAVE_STD_RING_SPAN  0
//#endif

#define  nsrs_USES_STD_RING_SPAN  ( (nsrs_CONFIG_SELECT_RING_SPAN == nsrs_RING_SPAN_STD) || ((nsrs_CONFIG_SELECT_RING_SPAN == nsrs_RING_SPAN_DEFAULT) && nsrs_HAVE_STD_RING_SPAN) )

// Compiler versions:
//
// MSVC++ 6.0  _MSC_VER == 1200 (Visual Studio 6.0)
// MSVC++ 7.0  _MSC_VER == 1300 (Visual Studio .NET 2002)
// MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio .NET 2003)
// MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
// MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
// MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
// ............_MSVC_LANG: 201402 for -std:c++14, default
// MSVC++ 14.1 _MSC_VER >= 1910 (Visual Studio 2017)
// ............_MSVC_LANG: 201402 for -std:c++14, default
// ............_MSVC_LANG: 201703 for -std:c++17

#if defined(_MSC_VER ) && !defined(__clang__)
# define nsrs_COMPILER_MSVC_VER      (_MSC_VER )
# define nsrs_COMPILER_MSVC_VERSION  (_MSC_VER / 10 - 10 * ( 5 + (_MSC_VER < 1900 ) ) )
#else
# define nsrs_COMPILER_MSVC_VER      0
# define nsrs_COMPILER_MSVC_VERSION  0
#endif

#define nsrs_COMPILER_VERSION( major, minor, patch ) ( 10 * ( 10 * (major) + (minor) ) + (patch) )

#if defined(__clang__)
# define nsrs_COMPILER_CLANG_VERSION nsrs_COMPILER_VERSION( __clang_major__, __clang_minor__, __clang_patchlevel__ )
#else
# define nsrs_COMPILER_CLANG_VERSION 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
# define nsrs_COMPILER_GNUC_VERSION nsrs_COMPILER_VERSION( __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__ )
#else
# define nsrs_COMPILER_GNUC_VERSION 0
#endif

// half-open range [lo..hi):
//#define nsrs_BETWEEN( v, lo, hi ) ( (lo) <= (v) && (v) < (hi) )

// Presence of language and library features:

#ifdef _HAS_CPP0X
# define nsrs_HAS_CPP0X  _HAS_CPP0X
#else
# define nsrs_HAS_CPP0X  0
#endif

// Unless defined otherwise below, consider VC14 as C++11 for ring-span-lite:

#if nsrs_COMPILER_MSVC_VER >= 1900
# undef  nsrs_CPP11_OR_GREATER
# define nsrs_CPP11_OR_GREATER  1
#endif

#define nsrs_CPP11_90   (nsrs_CPP11_OR_GREATER_ || nsrs_COMPILER_MSVC_VER >= 1500)
#define nsrs_CPP11_100  (nsrs_CPP11_OR_GREATER_ || nsrs_COMPILER_MSVC_VER >= 1600)
#define nsrs_CPP11_110  (nsrs_CPP11_OR_GREATER_ || nsrs_COMPILER_MSVC_VER >= 1700)
#define nsrs_CPP11_120  (nsrs_CPP11_OR_GREATER_ || nsrs_COMPILER_MSVC_VER >= 1800)
#define nsrs_CPP11_140  (nsrs_CPP11_OR_GREATER_ || nsrs_COMPILER_MSVC_VER >= 1900)

#define nsrs_CPP14_000  (nsrs_CPP14_OR_GREATER)
#define nsrs_CPP17_000  (nsrs_CPP17_OR_GREATER)

// Presence of C++11 language features:

// half-open range [lo..hi):
#define nsrs_BETWEEN( v, lo, hi ) ( (lo) <= (v) && (v) < (hi) )

// Presence of C++11 language features:

#define nsrs_HAVE_CONSTEXPR_11          nsrs_CPP11_140
#define nsrs_HAVE_IS_DEFAULT            nsrs_CPP11_140
#define nsrs_HAVE_IS_DELETE             nsrs_CPP11_140
#define nsrs_HAVE_NOEXCEPT              nsrs_CPP11_140
#define nsrs_HAVE_NULLPTR               nsrs_CPP11_100

// Presence of C++14 language features:

#define nsrs_HAVE_CONSTEXPR_14          nsrs_CPP14_000

// Presence of C++17 language features:
// no tag

// Presence of C++ library features:
// no tag

// Compiler warning suppression:

#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wundef"
# define nsrs_RESTORE_WARNINGS()   _Pragma( "clang diagnostic pop" )

#elif defined __GNUC__
# pragma GCC   diagnostic push
# pragma GCC   diagnostic ignored "-Wundef"
# define nsrs_RESTORE_WARNINGS()   _Pragma( "GCC diagnostic pop" )

#elif nsrs_COMPILER_MSVC_VERSION >= 140
# define nsrs_DISABLE_MSVC_WARNINGS(codes)  __pragma(warning(push))  __pragma(warning(disable: codes))
# define nsrs_RESTORE_WARNINGS()            __pragma(warning(pop ))

// Suppress the following MSVC warnings:
// - C4345: initialization behavior changed
//
// Suppress the following MSVC GSL warnings:
// - C26439, gsl::f.6 : special function 'function' can be declared 'noexcept'
// - C26440, gsl::f.6 : function 'function' can be declared 'noexcept'
// - C26472, gsl::t.1 : don't use a static_cast for arithmetic conversions;
//                      use brace initialization, gsl::narrow_cast or gsl::narrow
// - C26473: gsl::t.1 : don't cast between pointer types where the source type and the target type are the same
// - C26481: gsl::b.1 : don't use pointer arithmetic. Use span instead
// - C26490: gsl::t.1 : don't use reinterpret_cast

nsrs_DISABLE_MSVC_WARNINGS( 4345 26439 26440 26472 26473 26481 26490 )

#else
# define nsrs_RESTORE_WARNINGS()  /*empty*/
#endif

// C++ feature usage:

#if nsrs_HAVE_CONSTEXPR_11
# define nsrs_constexpr constexpr
#else
# define nsrs_constexpr /*constexpr*/
#endif

#if nsrs_HAVE_CONSTEXPR_14
# define nsrs_constexpr14 constexpr
#else
# define nsrs_constexpr14 /*constexpr*/
#endif

#if nsrs_HAVE_NOEXCEPT
# define nsrs_noexcept noexcept
# define nsrs_noexcept_op noexcept
#else
# define nsrs_noexcept /*noexcept*/
# define nsrs_noexcept_op(expr) /*noexcept(expr)*/
#endif

#if nsrs_HAVE_NULLPTR
# define nsrs_nullptr nullptr
#else
# define nsrs_nullptr NULL
#endif

// Method enabling

#if nsrs_CPP11_OR_GREATER

#define nsrs_REQUIRES_0(...) \
    template< bool B = (__VA_ARGS__), typename std::enable_if<B, int>::type = 0 >

#define nsrs_REQUIRES_T(...) \
    , typename = typename std::enable_if< (__VA_ARGS__), nonstd::ring_span_lite::detail::enabler >::type

#endif

// includes:

#include <cassert>
#include <iterator>
#include <utility>

// additional includes:

#if ! nsrs_CPP11_OR_GREATER
# include <algorithm>           // std::swap() until C++11
#endif

namespace nonstd { namespace ring_span_lite {

namespace detail {
/*enum*/ struct enabler{};
}

// type traits C++11:

namespace std11 {

#if nsrs_CPP11_OR_GREATER
using std::move;
#else
template< typename T > T const & move( T const & t ) { return t; }
#endif

template< bool B, class T, class F >
struct conditional { typedef T type; };

template< class T, class F >
struct conditional<false, T, F> { typedef F type; };

} // namespace std11

// type traits C++17:

namespace std17 {

#if nsrs_CPP17_OR_GREATER

using std::is_swappable;
using std::is_nothrow_swappable;

#elif nsrs_CPP11_OR_GREATER

namespace detail {

using std::swap;

struct is_swappable
{
    template< typename T, typename = decltype( swap( std::declval<T&>(), std::declval<T&>() ) ) >
    static std::true_type test( int );

    template< typename >
    static std::false_type test(...);
};

struct is_nothrow_swappable
{
    // wrap noexcept(epr) in separate function as work-around for VC140 (VS2015):

    template< typename T >
    static constexpr bool test()
    {
        return noexcept( swap( std::declval<T&>(), std::declval<T&>() ) );
    }

    template< typename T >
    static auto test( int ) -> std::integral_constant<bool, test<T>()>{}

    template< typename >
    static std::false_type test(...);
};

} // namespace detail

// is [nothow] swappable:

template< typename T >
struct is_swappable : decltype( detail::is_swappable::test<T>(0) ){};

template< typename T >
struct is_nothrow_swappable : decltype( detail::is_nothrow_swappable::test<T>(0) ){};

#endif // nsrs_CPP17_OR_GREATER

} // namespace std17

//
// element extraction policies:
//

template< class T >
struct null_popper
{
    typedef void return_type;

    void operator()( T & ) const nsrs_noexcept {}
};

template< class T >
struct default_popper
{
    typedef T return_type;

    T operator()( T & t ) const
    {
        return std11::move( t );
    }
};

template< class T >
struct copy_popper
{
    typedef T return_type;

#if  nsrs_RING_SPAN_LITE_EXTENSION
# if nsrs_CPP11_OR_GREATER
    copy_popper( T t )
    : m_copy( std::move(t) )
    {}
# else
    copy_popper( T const & t )
    : m_copy( t )
    {}
# endif
#else
    copy_popper( T && t )
    : copy( std::move(t) )
    {}
#endif

    T operator()( T & t ) const
    {
        using std::swap;
        T result( m_copy ); swap( t, result ); return result;
    }

    T m_copy;
};

// forward-declare iterator:

namespace detail {

template< class, bool >
class ring_iterator;

}

//
// ring span:
//
template< class T, class Popper = default_popper<T> >
class ring_span
{
public:
    typedef T   value_type;
    typedef T * pointer;
    typedef T & reference;
    typedef T const & const_reference;

    typedef std::size_t size_type;

    typedef ring_span< T, Popper > type;

    typedef detail::ring_iterator< type, false  > iterator;
    typedef detail::ring_iterator< type, true   > const_iterator;

#if nsrs_RING_SPAN_LITE_EXTENSION
    typedef std::reverse_iterator<iterator      > reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#endif

    // construction:

    template< class ContiguousIterator >
    ring_span(
        ContiguousIterator   begin
        , ContiguousIterator end
        , Popper popper = Popper()
    ) nsrs_noexcept
    : m_data     ( &* begin )
    , m_size     ( 0 )
    , m_capacity ( static_cast<size_type>( end - begin ) )
    , m_front_idx( 0 )
    , m_popper   ( std11::move( popper ) )
    {}

    template< class ContiguousIterator >
    ring_span(
        ContiguousIterator   begin
        , ContiguousIterator end
        , ContiguousIterator first
        , size_type          size
        , Popper popper = Popper()
    ) nsrs_noexcept
    : m_data     ( &* begin )
    , m_size     ( size     )
    , m_capacity ( static_cast<size_type>( end   - begin ) )
    , m_front_idx( static_cast<size_type>( first - begin ) )
    , m_popper   ( std11::move( popper ) )
    {
        assert( m_size <= m_capacity );
    }

#if nsrs_HAVE_IS_DEFAULT
    ring_span( ring_span && ) = default;
    ring_span& operator=( ring_span && ) = default;
#else
private:
    ring_span( ring_span const & );
    ring_span & operator=( ring_span const & );
public:
#endif

    // observers:

    bool empty() const nsrs_noexcept
    {
        return m_size == 0;
    }

    bool full() const nsrs_noexcept
    {
        return m_size == m_capacity;
    }

    size_type size() const nsrs_noexcept
    {
        return m_size;
    }

    size_type capacity() const nsrs_noexcept
    {
        return m_capacity;
    }

    // element access:

#if nsrs_RING_SPAN_LITE_EXTENSION
    reference operator[]( size_type idx ) nsrs_noexcept
    {
        return assert( idx < m_size ), at_(idx);
    }

    const_reference operator[]( size_type idx ) const nsrs_noexcept
    {
        return assert( idx < m_size ), at_(idx);
    }
#endif

    reference front() nsrs_noexcept
    {
        return *begin();
    }

    const_reference front() const nsrs_noexcept
    {
        return *begin();
    }

    reference back() nsrs_noexcept
    {
        return *(--end());
    }

    const_reference back() const nsrs_noexcept
    {
        return *(--end());
    }

    // iteration:

    iterator begin() nsrs_noexcept
    {
        return iterator( 0, this );
    }

    const_iterator begin() const nsrs_noexcept
    {
        return cbegin();
    }

    const_iterator cbegin() const nsrs_noexcept
    {
        return const_iterator( 0, this );
    }

    iterator end() nsrs_noexcept
    {
        return iterator( size(), this );
    }

    const_iterator end() const nsrs_noexcept
    {
        return cend();
    }

    const_iterator cend() const nsrs_noexcept
    {
        return const_iterator( size(), this );
    }

#if nsrs_RING_SPAN_LITE_EXTENSION

    reverse_iterator rbegin() nsrs_noexcept
    {
        return reverse_iterator( end() );
    }

    reverse_iterator rend() nsrs_noexcept
    {
        return reverse_iterator( begin() );
    }

    const_reverse_iterator rbegin() const nsrs_noexcept
    {
        return crbegin();
    }

    const_reverse_iterator rend() const nsrs_noexcept
    {
        return crend();
    }

    const_reverse_iterator crbegin() const nsrs_noexcept
    {
        return const_reverse_iterator( cend() );
    }

    const_reverse_iterator crend() const nsrs_noexcept
    {
        return const_reverse_iterator(cbegin());
    }
#endif

    // element insertion, extraction:

    typename Popper::return_type pop_front()
    {
        assert( ! empty() );

        reference element = front_();
        increment_front_();

        return m_popper( element );
    }

#if nsrs_RING_SPAN_LITE_EXTENSION
    typename Popper::return_type pop_back()
    {
        assert( ! empty() );

        reference element = back_();
        decrement_back_();

        return m_popper( element );
    }
#endif

#if nsrs_CPP11_OR_GREATER
    nsrs_REQUIRES_0( std::is_copy_assignable<T>::value )
    void push_back( value_type const & value) noexcept( std::is_nothrow_copy_assignable<T>::value )
#else
    void push_back( value_type const & value )
#endif
    {
        if ( full() )  increment_front_and_back_();
        else           increment_back_();

        back_() = value;
    }

#if nsrs_CPP11_OR_GREATER
    nsrs_REQUIRES_0( std::is_move_assignable<T>::value )
    void push_back( value_type && value ) noexcept( std::is_nothrow_move_assignable<T>::value )
    {
        if ( full() )  increment_front_and_back_();
        else           increment_back_();

        back_() = std::move( value );
    }

    template< typename... Args
        nsrs_REQUIRES_T(
            std::is_constructible<T, Args&&...>::value
            && std::is_move_assignable<T>::value
        )
    >
    void emplace_back( Args &&... args ) noexcept
    (
        std::is_nothrow_constructible<T, Args...>::value
        && std::is_nothrow_move_assignable<T>::value
    )
    {
        if ( full() )  increment_front_and_back_();
        else           increment_back_();

        back_() = T( std::forward<Args>(args)...);
   }
#endif

#if nsrs_RING_SPAN_LITE_EXTENSION

#if nsrs_CPP11_OR_GREATER
    nsrs_REQUIRES_0( std::is_copy_assignable<T>::value )
    void push_front( T const & value ) noexcept(( std::is_nothrow_copy_assignable<T>::value ))
#else
    void push_front( T const & value )
#endif
    {
        if ( full() ) decrement_front_and_back_();
        else          decrement_front_();

        front_() = value;
    }

#if nsrs_CPP11_OR_GREATER
    nsrs_REQUIRES_0( std::is_move_assignable<T>::value )
    void push_front( T && value ) noexcept(( std::is_nothrow_move_assignable<T>::value ))
    {
        if ( full() ) decrement_front_and_back_();
        else          decrement_front_();

        front_() = std::move(value);
    }

    template< typename... Args
        nsrs_REQUIRES_T(
            std::is_constructible<T, Args&&...>::value
            && std::is_move_assignable<T>::value
        )
    >
    void emplace_front( Args&&... args ) noexcept
    (
        std::is_nothrow_constructible<T, Args...>::value
        && std::is_nothrow_move_assignable<T>::value
    )
    {
        if ( full() ) decrement_front_and_back_();
        else          decrement_front_();

        front_() = T( std::forward<Args>(args)...);
    }
#endif
#endif // nsrs_RING_SPAN_LITE_EXTENSION

    // swap:

    void swap( type & rhs )
#if nsrs_CPP11_OR_GREATER
        noexcept(
            std17::is_nothrow_swappable<Popper>::value
        )
#endif
    {
        using std::swap;
        swap( m_data     , rhs.m_data      );
        swap( m_size     , rhs.m_size      );
        swap( m_capacity , rhs.m_capacity  );
        swap( m_front_idx, rhs.m_front_idx );
        swap( m_popper   , rhs.m_popper    );
    }

private:
    friend class detail::ring_iterator<ring_span, true >;   // const_iterator;
    friend class detail::ring_iterator<ring_span, false>;   // iterator;

    size_type normalize_( size_type const idx ) const nsrs_noexcept
    {
        return idx % m_capacity;
    }

    reference at_( size_type idx ) nsrs_noexcept
    {
        return m_data[ normalize_(m_front_idx + idx) ];
    }

    const_reference at_( size_type idx ) const nsrs_noexcept
    {
        return m_data[ normalize_(m_front_idx + idx) ];
    }

    reference front_() nsrs_noexcept
    {
        return *( m_data + m_front_idx );
    }

    const_reference front_() const nsrs_noexcept
    {
        return *( m_data + m_front_idx );
    }

    reference back_() nsrs_noexcept
    {
        return *( m_data + normalize_(m_front_idx + m_size - 1) );
    }

    const_reference back_() const nsrs_noexcept
    {
        return *( m_data + normalize_(m_front_idx + m_size - 1) );
    }

    void increment_front_() nsrs_noexcept
    {
        m_front_idx = normalize_(m_front_idx + 1);
        --m_size;
    }

    void decrement_front_() nsrs_noexcept
    {
        m_front_idx = normalize_(m_front_idx + m_capacity - 1);
        ++m_size;
    }

    void increment_back_() nsrs_noexcept
    {
        ++m_size;
    }

    void decrement_back_() nsrs_noexcept
    {
        --m_size;
    }

    void increment_front_and_back_() nsrs_noexcept
    {
        m_front_idx = normalize_( m_front_idx + 1 );
    }

    void decrement_front_and_back_() nsrs_noexcept
    {
        m_front_idx = normalize_( m_front_idx + m_capacity - 1 );
    }

private:
    pointer   m_data;
    size_type m_size;
    size_type m_capacity;
    size_type m_front_idx;
    Popper    m_popper;
};

// swap:

template< class T, class Popper >
inline void swap( ring_span<T,Popper> & lhs, ring_span<T,Popper> & rhs ) nsrs_noexcept_op( nsrs_noexcept_op( lhs.swap(rhs) ) )
{
    lhs.swap(rhs);
}

namespace detail {

//
// ring iterator:
//
#if 0
template< class RS, bool is_const >
class ring_iterator : public std::iterator
<
    std::random_access_iterator_tag
    , typename std11::conditional<is_const, const typename RS::value_type, typename RS::value_type>::type
>
#endif

template< class RS, bool is_const >
class ring_iterator
{
public:
    typedef ring_iterator<RS, is_const> type;

    typedef std::ptrdiff_t difference_type;
    typedef typename RS::value_type value_type;

    typedef typename std11::conditional<is_const, const value_type, value_type>::type * pointer;
    typedef typename std11::conditional<is_const, const value_type, value_type>::type & reference;
    typedef std::random_access_iterator_tag iterator_category;

#if nsrs_CPP11_OR_GREATER
    ring_iterator() = default;
#else
    ring_iterator() : m_idx(), m_rs() {}
#endif

#if nsrs_RING_SPAN_LITE_EXTENSION
    // conversion to const iterator:

    operator ring_iterator<RS, true>() const nsrs_noexcept
    {
        return ring_iterator<RS, true>( m_idx, m_rs );
    }
#endif

    // access content:

    reference operator*() const nsrs_noexcept
    {
        return m_rs->at_( m_idx );
    }

    // advance iterator:

    type & operator++() nsrs_noexcept
    {
        ++m_idx; return *this;
    }

    type operator++( int ) nsrs_noexcept
    {
        type r(*this); ++*this; return r;
    }

    type & operator--() nsrs_noexcept
    {
        --m_idx; return *this;
    }

    type operator--( int ) nsrs_noexcept
    {
        type r(*this); --*this; return r;
    }

#if defined(__clang__) || defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wsign-conversion"
#endif

    type & operator+=( int i ) nsrs_noexcept
    {
        m_idx += i; return *this;
    }

    type & operator-=( int i ) nsrs_noexcept
    {
        m_idx -= i; return *this;
    }

#if defined(__clang__) || defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

#if nsrs_RING_SPAN_LITE_EXTENSION

    template< bool C >
    difference_type operator-( ring_iterator<RS,C> const & rhs ) const nsrs_noexcept
    {
        return static_cast<difference_type>( this->m_idx ) - static_cast<difference_type>( rhs.m_idx );
    }
#endif

    // comparison:

    template< bool C >
    bool operator<( ring_iterator<RS,C> const & rhs ) const nsrs_noexcept
    {
        assert( this->m_rs == rhs.m_rs ); return ( this->m_idx < rhs.m_idx );
    }

    template< bool C >
    bool operator==( ring_iterator<RS,C> const & rhs ) const nsrs_noexcept
    {
        assert( this->m_rs == rhs.m_rs ); return ( this->m_idx == rhs.m_idx );
    }

    // other comparisons expressed in <, ==:

    template< bool C >
    inline bool operator!=( ring_iterator<RS,C> const & rhs ) const nsrs_noexcept
    {
        return ! ( *this == rhs );
    }

    template< bool C >
    inline bool operator<=( ring_iterator<RS,C> const & rhs ) const nsrs_noexcept
    {
        return ! ( rhs < *this );
    }

    template< bool C >
    inline bool operator>( ring_iterator<RS,C> const & rhs ) const nsrs_noexcept
    {
        return rhs < *this;
    }

    template< bool C >
    inline bool operator>=( ring_iterator<RS,C> const & rhs ) const nsrs_noexcept
    {
        return ! ( *this < rhs );
    }

private:
    friend RS;  // clang: non-class friend type 'RS' is a C++11 extension [-Wc++11-extensions]
    friend class ring_iterator<RS, ! is_const>;

    typedef typename RS::size_type size_type;
    typedef typename std11::conditional<is_const, const RS, RS>::type ring_type;

    ring_iterator( size_type idx, typename std11::conditional<is_const, const RS, RS>::type * rs ) nsrs_noexcept
    : m_idx( idx )
    , m_rs ( rs  )
    {}

private:
    size_type   m_idx;
    ring_type * m_rs;
};

// advanced iterator:

template< class RS, bool C >
inline ring_iterator<RS,C> operator+( ring_iterator<RS,C> it, int i ) nsrs_noexcept
{
    it += i; return it;
}

template< class RS, bool C >
inline ring_iterator<RS,C> operator-( ring_iterator<RS,C> it, int i ) nsrs_noexcept
{
    it -= i; return it;
}

} // namespace detail
} // namespace ring_span_lite

using ring_span_lite::ring_span;
using ring_span_lite::null_popper;
using ring_span_lite::default_popper;
using ring_span_lite::copy_popper;

} // namespace nonstd

nsrs_RESTORE_WARNINGS()

#endif // NONSTD_RING_SPAN_LITE_HPP
