// example/03-make-ring-span.cpp
// make_ring_span() requires move semantics (C++11 or later).

#include "nonstd/ring_span.hpp"

#if nsrs_CPP17_OR_GREATER
# include <iterator> // for std::data(), std::size()
#endif

#ifndef PROVIDE_MAKE_RING_SPAN_FROM_GENERIC_CONTAINER
#define PROVIDE_MAKE_RING_SPAN_FROM_GENERIC_CONTAINER  0
#endif

namespace nonstd {

// Tag to create empty ring_span:

const struct make_empty_t{} make_empty;

} // namespace nonstd

#if PROVIDE_MAKE_RING_SPAN_FROM_GENERIC_CONTAINER

namespace nonstd {
namespace detail {

template< typename C >
inline auto vtype( C & ) -> typename C::value_type
{
    return {};
}

#if nsrs_CPP17_OR_GREATER

using std::data;
using std::size;

#else // nsrs_CPP17_OR_GREATER

template< typename T, size_t N >
inline auto vtype( T(&)[N] ) -> T
{
    return {};
}

template< typename T, size_t N >
inline auto data( T(&arr)[N] ) -> T*
{
    return &arr[0];
}

template< typename T, size_t N >
inline auto size( T(&)[N] ) -> size_t
{
    return N;
}

template< typename C >
inline auto data( C & cont ) -> typename C::pointer
{
    return cont.data();
}

template< typename C >
inline auto size( C & cont ) -> typename C::size_type
{
    return cont.size();
}

#endif// nsrs_CPP17_OR_GREATER

} // namespace detail

// Create filled or empty ring_buffer from container:

template< typename C >
inline auto make_ring_span( C & cont ) -> ring_span<decltype(detail::vtype(cont))>
{
    return { detail::data(cont), detail::data(cont) + detail::size(cont), detail::data(cont), detail::size(cont) };
}

template< typename C >
inline auto make_ring_span( C & cont, make_empty_t ) -> ring_span<decltype(detail::vtype(cont))>
{
    return { detail::data(cont), detail::data(cont) + detail::size(cont) };
}

} // namespace nonstd

#else // PROVIDE_MAKE_RING_SPAN_FROM_GENERIC_CONTAINER

#if ! PROVIDE_MAKE_RING_SPAN_FROM_GENERIC_CONTAINER
# include <array>
# include <string>
# include <vector>
#endif

namespace nonstd {

// Create filled or empty ring_buffer from C-array:

template< typename T, size_t N >
inline auto make_ring_span( T(&arr)[N] ) -> ring_span<T>
{
    return { &arr[0], &arr[0] + N, &arr[0], N };
}

template< typename T, size_t N >
inline auto make_ring_span( T(&arr)[N], make_empty_t ) -> ring_span<T>
{
    return { &arr[0], &arr[0] + N };
}

// Create filled or empty ring_buffer from std::array:

template< typename T, size_t N >
inline auto make_ring_span( std::array<T,N> & arr ) -> ring_span<T>
{
    return { arr.data(), arr.data() + N, arr.data(), N };
}

template< typename T, size_t N >
inline auto make_ring_span( std::array<T,N> & arr, make_empty_t ) -> ring_span<T>
{
    return { arr.data(), arr.data() + N };
}

// Create filled or empty ring_buffer from std::vector:

template< typename T >
inline auto make_ring_span( std::vector<T> & vec ) -> ring_span<T>
{
    return { vec.data(), vec.data() + vec.size(), vec.data(), vec.size() };
}

template< typename T >
inline auto make_ring_span( std::vector<T> & vec, make_empty_t ) -> ring_span<T>
{
    return { vec.data(), vec.data() + vec.size() };
}

// Create filled or empty ring_buffer from std::basic_string:

template< typename T >
inline auto make_ring_span( std::basic_string<T> & str ) -> ring_span<T>
{
    return { str.data(), str.data() + str.size(), str.data(), str.size() };
}

template< typename T >
inline auto make_ring_span( std::basic_string<T> & str, make_empty_t ) -> ring_span<T>
{
    return { str.data(), str.data() + str.size() };
}

#endif // PROVIDE_MAKE_RING_SPAN_FROM_GENERIC_CONTAINER

} // namespace nonstd

//------------------------------------------------------------------------
// Application:

#include <iostream>
#include <iterator>
#include <numeric>

#if PROVIDE_MAKE_RING_SPAN_FROM_GENERIC_CONTAINER
# include <array>
# include <string>
# include <vector>
#endif

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
//  using nonstd::ring_span;
    using nonstd::make_ring_span;

    {
        int arr[] = { 0, 1 , 2 };

        auto spn_c_array_filled = make_ring_span( arr );
        auto spn_c_array_empty  = make_ring_span( arr, nonstd::make_empty );

        print( spn_c_array_filled );
        print( spn_c_array_empty  );
    }{
        std::array<int, 4> arr = {{ 0, 1 , 2, 3 }};

        auto spn_std_array_filled = make_ring_span( arr );
        auto spn_std_array_empty  = make_ring_span( arr, nonstd::make_empty );

        print( spn_std_array_filled );
        print( spn_std_array_empty );
    }{
        std::vector<int> vec( 5 );
        std::iota( vec.begin(), vec.end(), 0 );

        auto spn_std_vector_filled = make_ring_span( vec );
        auto spn_std_vector_empty  = make_ring_span( vec, nonstd::make_empty );

        print( spn_std_vector_filled );
        print( spn_std_vector_empty );
    }{
//        std::string text( "abcdef" );
//
//        auto spn_std_string_filled = make_ring_span( text );
//        auto spn_std_string_empty  = make_ring_span( text, nonstd::make_empty );
//
//        print( spn_std_string_filled );
//        print( spn_std_string_empty );
    }
}

// cl -nologo -DPROVIDE_MAKE_RING_SPAN_FROM_GENERIC_CONTAINER=1 -W4 -EHsc -I../include 03-make-ring-span.cpp && 03-make-ring-span
// g++ -std=c++11 -DPROVIDE_MAKE_RING_SPAN_FROM_GENERIC_CONTAINER=1 -Wall -fno-exceptions -I../include -o 03-make-ring-span 03-make-ring-span.cpp && 03-make-ring-span

// Output:
// spn_c_array_filled: capacity: 3 size: 3, content: { 0, 1, 2, }
// spn_c_array_empty: capacity: 3 size: 0, content: { }
// spn_std_array_filled: capacity: 4 size: 4, content: { 0, 1, 2, 3, }
// spn_std_array_empty: capacity: 4 size: 0, content: { }
// spn_std_vector_filled: capacity: 5 size: 5, content: { 0, 1, 2, 3, 4, }
// spn_std_vector_empty: capacity: 5 size: 0, content: { }
