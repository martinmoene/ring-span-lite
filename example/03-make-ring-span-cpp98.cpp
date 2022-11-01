// example/04-make-ring-span-cpp98.cpp
// make_ring_span() requires move semantics (C++11 or later).

#include "nonstd/ring_span.hpp"

namespace nonstd {

// Tag to create empty ring_span:

const struct make_empty_t{} make_empty;

} // namespace nonstd

namespace nonstd {
namespace detail {

template< typename T, T v > struct integral_constant{};
typedef integral_constant< bool, true  > true_type;
typedef integral_constant< bool, false > false_type;

template < typename T > struct is_array : false_type {};
template < typename T > struct is_array< T[] > : true_type {};
template < typename T, size_t N > struct is_array< T[N] > : true_type {};

#define vtypeof( C ) \
   typename nonstd::detail::vtype<nonstd::detail::ident(C)>::type

template< typename C >
struct vtype
{
    typedef typename C::value_type type;
};

template< typename T >
struct vtype< T* >
{
    typedef T type;
};

template< typename C >
C & ident( C & c )
{
    return c;
}

template< typename T, size_t N >
T* ident( T(&a)[N] )
{
    return a;
}

template< typename T, size_t N >
inline T* data( T(&arr)[N] )
{
    return &arr[0];
}

template< typename T, size_t N >
inline size_t size( T(&)[N] )
{
    return N;
}

template< typename C >
inline typename C::pointer
data( C & cont )
{
    return cont.data();
}

template< typename C >
inline typename C::size_type
size( C & cont )
{
    return cont.size();
}

} // namespace detail

} // namespace nonstd

// Create filled or empty ring_buffer from container:

#define nsrs_MAKE_RING_SPAN( name, cont, T ) \
    nonstd::ring_span<T> name( \
        nonstd::detail::data(cont), nonstd::detail::data(cont) + nonstd::detail::size(cont), \
        nonstd::detail::data(cont), nonstd::detail::size(cont) )

#define nsrs_MAKE_RING_SPAN_EMPTY( name, cont, T ) \
    nonstd::ring_span<T> name( \
        nonstd::detail::data(cont), nonstd::detail::data(cont) + nonstd::detail::size(cont) )

//------------------------------------------------------------------------
// Application:

#include <array>
#include <string>
#include <vector>

#include <iostream>
#include <iterator>
#include <numeric>

template< typename T >
std::ostream & operator<<( std::ostream & os, nonstd::ring_span<T> const & spn )
{
    os << "{ ";
    std::copy( spn.cbegin(), spn.cend(), std::ostream_iterator<T>(os, ", ") );
    return os << "}";
}

#define print( spn ) \
    std::cout << #spn ": capacity: " << spn.capacity() << " size: " << spn.size() << ", content: " << spn << "\n";

int main()
{
    {
        int arr[] = { 0, 1 , 2 };

//        typedef typename nonstd::detail::vtype< int(&)[3]>::type value_type;
//        typedef typename nonstd::detail::vtype< std::vector<int> >::type value_type;

//        int aaa[10];
//        vtypeof(aaa) elem;

        nsrs_MAKE_RING_SPAN(       spn_c_array_filled, arr, int );
        nsrs_MAKE_RING_SPAN_EMPTY( spn_c_array_empty, arr, int );

        print( spn_c_array_filled );
        print( spn_c_array_empty  );
    }{
//        std::array<int, 4> arr = {{ 0, 1 , 2, 3 }};
//
//        nsrs_MAKE_RING_SPAN(       spn_std_array_filled, arr, int );
//        nsrs_MAKE_RING_SPAN_EMPTY( spn_std_array_empty, arr, int );
//
//        print( spn_std_array_filled );
//        print( spn_std_array_empty );
    }{
//        std::vector<int> vec( 5 );
//        std::iota( vec.begin(), vec.end(), 0 );
//
//        nsrs_MAKE_RING_SPAN(       spn_std_vector_filled, vec, int );
//        nsrs_MAKE_RING_SPAN_EMPTY( spn_std_vector_empty, vec, int );
//
//        print( spn_std_vector_filled );
//        print( spn_std_vector_empty );
    }{
//        std::string text( "abcdef" );
//
//        nsrs_MAKE_RING_SPAN( spn_std_string_filled, text, int );
//        nsrs_MAKE_RING_SPAN( spn_std_string_empty, text, int );
//
//        print( spn_std_string_filled );
//        print( spn_std_string_empty );
    }
}

// cl -nologo -W4 -EHsc -I../include 04-make-ring-span-cpp98.cpp && 04-make-ring-span-cpp98
// g++ -std=c++11 -Wall -fno-exceptions -I../include -o 04-make-ring-span-cpp98 04-make-ring-span-cpp98.cpp && 04-make-ring-span-cpp98

// Output:
