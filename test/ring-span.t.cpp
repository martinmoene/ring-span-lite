//
// ring-span-lite, a C++yy-like ring span type for C++98 and later.
// For more information see https://github.com/martinmoene/ring-span-lite
//
// Copyright (c) 2017 Martin Moene
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

#include "ring-span-lite.t.hpp"

#include <numeric>

using namespace nonstd;

typedef ring_span<int>::size_type size_type;

template< typename T, size_t N >
inline size_t dim( T (&arr)[N] )
{
    return N;
}

CASE( "ring_span: Allows to construct a full span from iterator pair" )
{
    int arr[] = { 1, 2, 3, };

    ring_span<int> rs( arr, arr + dim(arr) );

    EXPECT( rs.size()     == size_type(0) );
    EXPECT( rs.capacity() == dim(arr)     );
}

CASE( "ring_span: Allows to construct a partially filled span from iterator pair and iterator, size" )
{
    int arr[] = { 7, 7, 1, 2, 3, 7, 7, 7, };
    size_type first = 2;
    size_type count = 3;

    ring_span<int> rs( arr, arr + dim(arr), arr + first, count );

    EXPECT( rs.size()     == count    );
    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Disallows to copy-construct from a ring_span (compile-time)" )
{
#if nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS
    int arr[] = { 1, 2, 3, };
    ring_span<int> rs1( arr, arr + dim(arr) );

    ring_span<int> rs2( rs1 );
#else
    EXPECT( !!"ring_span: check for compile-time error (define nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS)" );
#endif
}

CASE( "ring_span: Disallows to copy-assign from a ring_span (compile-time)" )
{
#if nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS
    int arr[] = { 1, 2, 3, };
    ring_span<int> rs1( arr, arr + dim(arr) );
    ring_span<int> rs2( arr, arr + dim(arr) );

    rs2 = rs1;
#else
    EXPECT( !!"ring_span: check for compile-time error (define nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS)" );
#endif
}

CASE( "ring_span: Allows to move-construct from a ring_span (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    int arr[] = { 1, 2, 3, };

    ring_span<int> rs( ring_span<int>( arr, arr + dim(arr) ) );

    EXPECT( rs.size()     == size_type(0) );
    EXPECT( rs.capacity() == dim(arr)     );
#else
    EXPECT( !!"ring_span: move-construction is not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to move-assign from a ring_span (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    int arr[] = { 1, 2, 3, };
    ring_span<int> rs( arr, arr + dim(arr) );

    rs = ring_span<int>( arr, arr + dim(arr) );

    EXPECT( rs.size()     == size_type(0) );
    EXPECT( rs.capacity() == dim(arr)     );
#else
    EXPECT( !!"ring_span: move-assignment is not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to obtain the capacity of a span" )
{
    int arr[] = { 1, 2, 3, };

    ring_span<int> rs( arr, arr + dim(arr) );

    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Allows to obtain the number of elements in a span (size)" )
{
    int arr[] = { 1, 2, 3, };

    ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.size() == rs.capacity() );
}

CASE( "ring_span: Allows to check for an empty span" )
{
    int arr[] = { 1, 2, 3, };

    ring_span<int> rs( arr, arr + dim(arr) );

    EXPECT( rs.empty()                 );
    EXPECT( rs.size( ) == size_type(0) );
}

CASE( "ring_span: Allows to check for a full span" )
{
    int arr[] = { 1, 2, 3, };

    ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.full()                  );
    EXPECT( rs.size() == rs.capacity() );
}

// (const) front
// (const) back
// (const) pop_front
// (const) pop_back

// push_back( value)
// push_back( &&)
// push_back( in-place)

// swap
// global swap


// (const) iteration (for( x:span), algorithm

CASE( "ring_span: Allows to appear in range-for (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    int arr[] = { 1, 2, 3, };
    ring_span  <int> rs( arr, arr + dim(arr), arr, dim(arr) );

    std::vector<int> vec;
    for ( auto x : rs )
    {
        vec.push_back( x );
    }

    EXPECT( std::equal( vec.begin(), vec.end(), arr ) );
#else
    EXPECT( !!"range-for is not available (no C++11)" );
#endif
}

// (const) reverse iteration (algorithm)

// iteration:
// dereference: *,
// increment: pre/post ++, --, +=n, -=n, +n, -n,
// difference: it-it
// comparison: == != < <= > >=


// different poppers


// Applets:

#include <iostream>

template< class T, class Popper /*, class Pusher*/ >
void print( std::ostream & os, ring_span<T,Popper/*,Pusher*/> & rs )
{
    std::copy( rs.begin(), rs.end(), std::ostream_iterator<T>(os, ", ") );
}

CASE( "ring_span: filter" "[.app]" )
{
    double arr[]   = { 2.0 , 3.0, 5.0, };
    double coeff[] = { 0.25, 0.5, 0.25 };

    ring_span<double> buffer( arr, arr + dim(arr), arr, dim(arr) );

    std::cout << "buffer: "; print(std::cout, buffer); std::cout << "\n";

    // In an update loop, interrupt routine or the like:
    buffer.push_back( 7.0 );

    std::cout << "buffer: "; print(std::cout, buffer); std::cout << "\n";

    double result = std::inner_product( buffer.begin(), buffer.end(), coeff, 0.0 );

    std::cout << "result: " << result << "\n";

    EXPECT( result == 5.0 );
}

// end of file
