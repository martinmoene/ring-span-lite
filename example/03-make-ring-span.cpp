// example/03-make-ring-span.cpp
// make_ring_span() requires move semantics (C++11 or later).

#include "nonstd/ring_span.hpp"

#include <array>
#include <string>
#include <vector>

namespace nonstd {

// Tag to create empty ring_span:

const struct make_empty_t{} make_empty;

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

} // namespace nonstd

//------------------------------------------------------------------------
// Application:

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

// cl -nologo -EHsc -I../include 03-make-ring-span.cpp && 03-make-ring-span
// g++ -std=c++11 -Wall -fno-exceptions -I../include -o 03-make-ring-span 03-make-ring-span.cpp && 03-make-ring-span

// Output:
// spn_c_arr_filled: capacity: 3 size: 3, content: { 0, 1, 2, }
// spn_c_arr_empty: capacity: 3 size: 0, content: { }
// spn_std_arr_filled: capacity: 4 size: 4, content: { 0, 1, 2, 3, }
// spn_std_arr_f_empty: capacity: 4 size: 0, content: { }
// spn_std_vec_filled: capacity: 5 size: 5, content: { 0, 1, 2, 3, 4, }
// spn_std_vec_empty: capacity: 5 size: 0, content: { }
