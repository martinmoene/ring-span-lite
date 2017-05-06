//
// Copyright (c) 2015 Arthur O'Dwyer
// Copyright (c) 2017 Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#ifndef NONSTD_RING_SPAN_LITE_HPP
#define NONSTD_RING_SPAN_LITE_HPP

#include <cassert>
#include <iterator>
#include <utility>

#define  ring_span_lite_VERSION "0.0.0"

// ring-span-lite configuration:

// Compiler detection (C++17 is speculative):

#define nsrs_CPP11_OR_GREATER  ( __cplusplus >= 201103L )
#define nsrs_CPP14_OR_GREATER  ( __cplusplus >= 201402L )
#define nsrs_CPP17_OR_GREATER  ( __cplusplus >= 201700L )

// half-open range [lo..hi):
#define nsrs_BETWEEN( v, lo, hi ) ( lo <= v && v < hi )

#if defined(_MSC_VER) && !defined(__clang__)
# define nsrs_COMPILER_MSVC_VERSION   (_MSC_VER / 100 - 5 - (_MSC_VER < 1900))
#else
# define nsrs_COMPILER_MSVC_VERSION   0
#endif

#if defined __GNUC__
# define nsrs_COMPILER_GNUC_VERSION  __GNUC__
#else
# define nsrs_COMPILER_GNUC_VERSION    0
#endif

#if nsrs_BETWEEN(nsrs_COMPILER_MSVC_VERSION, 7, 14 )
# pragma warning( push )
# pragma warning( disable: 4345 )   // initialization behavior changed
#endif

// Presence of C++11 language features:

#if nsrs_CPP11_OR_GREATER || nsrs_COMPILER_MSVC_VERSION >= 10
# define nsrs_HAVE_AUTO  1
# define nsrs_HAVE_NULLPTR  1
# define nsrs_HAVE_STATIC_ASSERT  1
#endif

#if nsrs_CPP11_OR_GREATER || nsrs_COMPILER_MSVC_VERSION >= 12
# define nsrs_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG  1
# define nsrs_HAVE_INITIALIZER_LIST  1
#endif

#if nsrs_CPP11_OR_GREATER || nsrs_COMPILER_MSVC_VERSION >= 14
# define nsrs_HAVE_ALIAS_TEMPLATE  1
# define nsrs_HAVE_CONSTEXPR_11  1
# define nsrs_HAVE_ENUM_CLASS  1
# define nsrs_HAVE_EXPLICIT_CONVERSION  1
# define nsrs_HAVE_IS_DEFAULT  1
# define nsrs_HAVE_IS_DELETE  1
# define nsrs_HAVE_NOEXCEPT  1
#endif

// Presence of C++14 language features:

#if nsrs_CPP14_OR_GREATER
# define nsrs_HAVE_CONSTEXPR_14  1
#endif

// Presence of C++17 language features:

#if nsrs_CPP17_OR_GREATER
# define nsrs_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE  1
#endif

// Presence of C++ library features:

#if nsrs_COMPILER_GNUC_VERSION
# define nsrs_HAVE_TR1_TYPE_TRAITS  1
# define nsrs_HAVE_TR1_ADD_POINTER  1
#endif

#if nsrs_CPP11_OR_GREATER || nsrs_COMPILER_MSVC_VERSION >= 9
# define nsrs_HAVE_TYPE_TRAITS  1
# define nsrs_HAVE_STD_ADD_POINTER  1
#endif

#if nsrs_CPP11_OR_GREATER || nsrs_COMPILER_MSVC_VERSION >= 11
# define nsrs_HAVE_ARRAY  1
#endif

#if nsrs_CPP11_OR_GREATER || nsrs_COMPILER_MSVC_VERSION >= 12
# define nsrs_HAVE_CONDITIONAL  1
#endif

#if nsrs_CPP11_OR_GREATER || nsrs_COMPILER_MSVC_VERSION >= 14 || (nsrs_COMPILER_MSVC_VERSION >= 9 && _HAS_CPP0X)
# define nsrs_HAVE_CONTAINER_DATA_METHOD  1
#endif

#if nsrs_CPP11_OR_GREATER || nsrs_COMPILER_MSVC_VERSION >= 12
# define nsrs_HAVE_REMOVE_CV  1
#endif

#if nsrs_CPP11_OR_GREATER || nsrs_COMPILER_MSVC_VERSION >= 14
# define nsrs_HAVE_SIZED_TYPES  1
#endif

// For the rest, consider VC14 as C++11 for ring-span-lite:

#if nsrs_COMPILER_MSVC_VERSION >= 14
# undef  nsrs_CPP11_OR_GREATER
# define nsrs_CPP11_OR_GREATER  1
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

// additional includes:

#if nsrs_HAVE_INITIALIZER_LIST
# include <initializer_list>
#endif

#if nsrs_HAVE_TYPE_TRAITS
# include <type_traits>
#elif nsrs_HAVE_TR1_TYPE_TRAITS
# include <tr1/type_traits>
#endif

namespace nonstd {
    
#if nsrs_CPP11_OR_GREATER
using std::move;
#else
template< typename T > T const & move( T const  & t ) { return t; }
#endif 

template< bool B, class T, class F >
struct conditional { typedef T type; };
 
template< class T, class F >
struct conditional<false, T, F> { typedef F type; };

#if nsrs_CPP11_OR_GREATER

template< bool B, class T, class F >
using conditional_t = typename conditional<B,T,F>::type;

template< bool B, class T = void >
using enable_if_t = typename std::enable_if<B,T>::type;

#endif

//
// element extraction policies:
//

template< class T > 
struct null_popper
{
    typedef void return_type;
    
    void operator()( T & ) {}
};

template< class T > 
struct default_popper
{
    typedef T return_type;
    
    T operator()( T & t )
    {
        return nonstd::move( t );
    }
};

template< class T > 
struct copy_popper
{
    typedef T return_type;
    
#if nsrs_CPP11_OR_GREATER
    copy_popper( T && t )
    : copy( std::move(t) ) 
    {}
#else
    copy_popper( T const & t )
    : copy( t ) 
    {}
#endif

    T operator()( T & t )
    {
        T old = t;
        t = copy;
        return old;
    }

    T copy;
};

//
// element insertion policies:
//

template< class T > 
struct default_pusher {};

// forward decleare iterator:

template< class, bool >
class ring_iterator;

//
// ring span:
//
template
< 
    class T
    , class Popper = default_popper<T>
//  , class Pusher = default_pusher<T> 
>
class ring_span
{
public:
    typedef T   value_type;
    typedef T * pointer;
    typedef T & reference;
    typedef T const & const_reference;

    typedef std::size_t size_type;

    typedef ring_span< T, Popper /*, Pusher*/ > type;
    
    typedef ring_iterator< type, false          > iterator;
    typedef ring_iterator< type, true           > const_iterator;
    typedef std::reverse_iterator<iterator      > reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // construction:
    
    template< class ContiguousIterator >
    ring_span( 
        ContiguousIterator   begin
        , ContiguousIterator end
        , Popper popper = Popper()
//      , Pusher pusher = Pusher() 
    ) nsrs_noexcept
    : m_data     ( & *begin )
    , m_size     ( 0 )
    , m_capacity ( end - begin )
    , m_front_idx( 0 )
    , m_popper   ( nonstd::move( popper ) )
//  , m_pusher   ( nonstd::move( pusher ) )
    {}

    template< class ContiguousIterator >
    ring_span( 
        ContiguousIterator   begin
        , ContiguousIterator end
        , ContiguousIterator first
        , size_type          size
        , Popper popper = Popper()
//      , Pusher pusher = Pusher() 
    ) nsrs_noexcept
    : m_data     ( & * begin )
    , m_size     ( size )
    , m_capacity ( end - begin   )
    , m_front_idx( first - begin )
    , m_popper   ( nonstd::move( popper ) )
//  , m_pusher   ( nonstd::move( pusher ) )
    {}

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

    // element insertion, extraction:
    
    typename Popper::return_type pop_front()
    {
        assert( ! empty() );

        typename Popper::return_type & element = front_();
        increment_front_();

        return m_popper( element );
    }

    typename Popper::return_type pop_back()
    {
        assert( ! empty() );
        
        typename Popper::return_type & element = back_();
        decrement_back_();
        
        return m_popper( element );
    }

#if nsrs_CPP11_OR_GREATER
    template< bool b = true, typename = nonstd::enable_if_t<b && std::is_copy_assignable<T>::value> >
    void push_back( value_type const & value) nsrs_noexcept_op(( std::is_nothrow_copy_assignable<T>::value ))
#else
    void push_back( value_type const & value )
#endif
    {
        if ( full() )  increment_front_and_back_();
        else           increment_back_();

        back_() = value;
    }

#if nsrs_CPP11_OR_GREATER
    template< bool b = true, typename = nonstd::enable_if_t<b && std::is_move_assignable<T>::value> >
    void push_back( value_type && value ) nsrs_noexcept_op(( std::is_nothrow_move_assignable<T>::value ))
    {
        if ( full() )  increment_front_and_back_();
        else           increment_back_();

        back_() = std::move( value );
    }
#endif

#if nsrs_CPP11_OR_GREATER
    template< class... Args >
    void emplace_back( Args &&... args ) 
        nsrs_noexcept_op(( std::is_nothrow_constructible<T, Args...>::value && std::is_nothrow_move_assignable<T>::value ))
    {
        if ( full() )  increment_front_and_back_();
        else           increment_back_();

        back_() = T( std::forward<Args>(args)...);
   }
#endif

    // swap:

    void swap( type & rhs ) nsrs_noexcept // nsrs_noexcept_op(std::is_nothrow_swappable<Popper>::value);
    {
        using std::swap;
        swap( m_data     , rhs.m_data      );
        swap( m_size     , rhs.m_size      );
        swap( m_capacity , rhs.m_capacity  );
        swap( m_front_idx, rhs.m_front_idx );
        swap( m_popper   , rhs.m_popper    );
//      swap( m_pusher   , rhs.m_pusher    );
    }

    friend void swap( ring_span & lhs, ring_span & rhs ) nsrs_noexcept_op(nsrs_noexcept_op(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }

private:
    friend iterator;
    friend const_iterator;

    size_type normalize_( size_type const idx ) const
    {
        return idx % m_capacity;
    }

    reference at( size_type idx ) nsrs_noexcept
    {
        return m_data[ normalize_(m_front_idx + idx) ];
    }

    const_reference at( size_type idx ) const nsrs_noexcept
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
//  Pusher    m_pusher;
};

//
// ring iterator:
//
#if 0
template< class RS, bool is_const >
class ring_iterator : public std::iterator
< 
    std::random_access_iterator_tag
    , typename nonstd::conditional<is_const, const typename RS::value_type, typename RS::value_type>::type 
>
#endif

template< class RS, bool is_const >
class ring_iterator
{
public:
    typedef ring_iterator<RS, is_const> type;

    typedef std::ptrdiff_t difference_type;
    typedef typename RS::value_type value_type;

    typedef typename nonstd::conditional<is_const, const value_type, value_type>::type * pointer;
    typedef typename nonstd::conditional<is_const, const value_type, value_type>::type & reference;
    typedef std::random_access_iterator_tag iterator_category;

#if nsrs_CPP11_OR_GREATER
    ring_iterator() = default;
#else
    ring_iterator() : m_idx(), m_rv() {}
#endif

    // access content:

    reference operator*() const nsrs_noexcept
    {
        return m_rv->at( m_idx );
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

    type & operator+=( int i ) nsrs_noexcept 
    { 
        m_idx += i; return *this; 
    }

    type & operator-=( int i ) nsrs_noexcept 
    { 
        m_idx -= i; return *this; 
    }

    friend difference_type operator-( type const & lhs, type const & rhs ) nsrs_noexcept // const nsrs_noexcept
    {
        // return diff normalize( idx - other.idx )
        return lhs.m_idx - rhs.m_idx;
    }
    
    // comparison:
    
    template< bool C >
    bool operator<( ring_iterator<RS,C> const & rhs ) const nsrs_noexcept
    {
        assert( m_rv == rhs.m_rv ); return ( m_idx < rhs.m_idx );
    }

    template< bool C >
    bool operator==( ring_iterator<RS,C> const & rhs ) const nsrs_noexcept
    {
        assert( m_rv == rhs.m_rv ); return ( m_idx == rhs.m_idx );
    }

private:
    friend RS;
    typedef typename RS::size_type size_type;
    typedef typename nonstd::conditional<is_const, const RS, RS>::type ring_type;

    ring_iterator( size_type idx, typename nonstd::conditional<is_const, const RS, RS>::type * rv) nsrs_noexcept
    : m_idx( idx )
    , m_rv ( rv  )
    {}

private:
    size_type   m_idx;
    ring_type * m_rv;
};

// advanced iterator:

template< class RS, bool C >
ring_iterator<RS,C> operator+( ring_iterator<RS,C> it, int i ) nsrs_noexcept 
{ 
    it += i; return it; 
}

template< class RS, bool C >
ring_iterator<RS,C> operator-( ring_iterator<RS,C> it, int i ) nsrs_noexcept 
{ 
    it -= i; return it; 
}

// other ring_iterator comparisons expressed in <, ==:

template< class RS, bool C >
bool operator!=( ring_iterator<RS,C> const & lhs, ring_iterator<RS,C> const & rhs ) nsrs_noexcept
{
    return ! ( lhs == rhs );
}

template< class RS, bool C >
bool operator<=( ring_iterator<RS,C> const & lhs, ring_iterator<RS,C> const & rhs ) nsrs_noexcept
{
    return ! ( lhs > rhs );
}

template< class RS, bool C >
bool operator>( ring_iterator<RS,C> const & lhs, ring_iterator<RS,C> const & rhs ) nsrs_noexcept
{
    return rhs < lhs;
}

template< class RS, bool C >
bool operator>=( ring_iterator<RS,C> const & lhs, ring_iterator<RS,C> const & rhs ) nsrs_noexcept
{
    return ! ( lhs < rhs );
}

} // namespace nonstd

#endif // NONSTD_RING_SPAN_LITE_HPP
