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

#ifndef  nsrs_RING_SPAN_LITE_T_HEADER
#define  nsrs_RING_SPAN_LITE_T_HEADER  "ring-span-lite.t.hpp"
#endif

#include nsrs_RING_SPAN_LITE_T_HEADER

#include <numeric>

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

#if nsrs_CPP11_OR_GREATER
struct noncopyable
{
    char c; int i;
    noncopyable() : c(), i() {}
    noncopyable( char c, int i ) : c(c), i(i) {}
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

    ring_span<int> rs( arr, arr + dim(arr) );

    EXPECT( rs.size()     == size_type(0) );
    EXPECT( rs.capacity() == dim(arr)     );
}

CASE( "ring_span: Allows to construct a partially filled span from an iterator pair and iterator, size" )
{
    int arr[] = { 7, 7, 1, 2, 3, 7, 7, 7, };
    size_type first = 2;
    size_type count = 3;

    ring_span<int> rs( arr, arr + dim(arr), arr + first, count );

    EXPECT( rs.size()     == count    );
    EXPECT( rs.capacity() == dim(arr) );
    EXPECT( std::equal( rs.begin(), rs.end(), arr + first ) );
}

CASE( "ring_span: Disallows to copy-construct from a ring_span (compile-time)" )
{
#if nsrs_CONFIG_CONFIRMS_COMPILATION_ERRORS
    int arr[] = { 1, 2, 3, }; ring_span<int> rs1( arr, arr + dim(arr) );

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

    ring_span<int> rs( ring_span<int>( arr, arr + dim(arr), arr, dim(arr) ) );

    EXPECT( rs.size()     == dim(arr) );
    EXPECT( rs.capacity() == dim(arr) );
    EXPECT( std::equal( rs.begin(), rs.end(), arr ) );
#else
    EXPECT( !!"ring_span: move-construction is not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to move-assign from a ring_span (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr) );

    rs = ring_span<int>( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.size()     == dim(arr) );
    EXPECT( rs.capacity() == dim(arr) );
    EXPECT( std::equal( rs.begin(), rs.end(), arr ) );
#else
    EXPECT( !!"ring_span: move-assignment is not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to obtain the capacity of a span" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr) );

    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Allows to obtain the number of elements in a span (size)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.size() == rs.capacity() );
}

CASE( "ring_span: Allows to check for an empty span" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr) );

    EXPECT( rs.empty() );
}

CASE( "ring_span: Allows to check for a full span" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.full() );
}

CASE( "ring_span: Allows to observe the element at the front" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.front() == arr[0] );
}

CASE( "ring_span: Allows to observe the element at the back" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.back() == arr[dim(arr) - 1] );
}

CASE( "ring_span: Allows to obtain and remove the element at the front" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.pop_front() == arr[0] );
}

CASE( "ring_span: Allows to obtain and remove the element at the back" )
{
#if nsrs_SG14
    EXPECT( !!"pop_back() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.pop_back() == arr[dim(arr) - 1] );
#endif
}

CASE( "ring_span: Allows to copy-insert an element at the front" )
{
#if nsrs_SG14
    EXPECT( !!"push_front() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    int new_element = 7;

    rs.push_front( new_element );

    EXPECT( rs.front() == new_element );
#endif
}

CASE( "ring_span: Allows to move-insert an element at the front (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
#if nsrs_SG14
    EXPECT( !!"push_front() is not available (SG14)" );
#else
    oracle arr[3]; ring_span<oracle> rs( arr, arr + dim(arr), arr, dim(arr) );
    int value = 7;

    rs.push_front( oracle(value) );

    EXPECT( oracle::state()    == oracle::moved_from );
    EXPECT( rs.front().value() == value              );
#endif
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to emplace an element at the front (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
#if nsrs_SG14
    EXPECT( !!"emplace_front() is not available (SG14)" );
#else
    noncopyable arr[3]; ring_span<noncopyable> rs( arr, arr + dim(arr), arr, dim(arr) );
    EXPECT( rs.front().c == char() );
    EXPECT( rs.front().i ==  int() );

    rs.emplace_front( 'a', 7 );

    EXPECT( rs.front().c == 'a' );
    EXPECT( rs.front().i ==  7  );
#endif
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "ring_span: Allows to copy-insert an element at the back" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    int new_element = 7;

    rs.push_back( new_element );

    EXPECT( rs.back() == new_element );
}

CASE( "ring_span: Allows to move-insert an element at the back (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    oracle arr[3]; ring_span<oracle> rs( arr, arr + dim(arr), arr, dim(arr) );
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
    noncopyable arr[3]; ring_span<noncopyable> rs( arr, arr + dim(arr), arr, dim(arr) );
    EXPECT( rs.back().c == char() );
    EXPECT( rs.back().i ==  int() );

    rs.emplace_back( 'a', 7 );

    EXPECT( rs.back().c == 'a' );
    EXPECT( rs.back().i ==  7  );
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "ring_span: Adding an element to an empty span makes it non-empty" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr) );
    EXPECT( rs.empty() );

    rs.push_back( 7 );

    EXPECT( ! rs.empty() );
}

CASE( "ring_span: Adding an element to an empty span doesn't change its capacity" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr) );
    EXPECT( rs.empty()                );
    EXPECT( rs.capacity() == dim(arr) );

    rs.push_back( 7 );

    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Adding an element to a full span leaves it full" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    EXPECT( rs.full() );

    rs.push_back( 7 );

    EXPECT( rs.full() );
}

CASE( "ring_span: Adding an element to a full span doesn't change its capacity" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    EXPECT( rs.full()                 );
    EXPECT( rs.capacity() == dim(arr) );

    rs.push_back( 7 );

    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Removing an element from an empty span asserts !empty (front)" "[.assert]" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr) );
    EXPECT( rs.empty() );

    (void) rs.pop_front();
}

CASE( "ring_span: Removing an element from an empty span asserts !empty (back)" "[.assert]" )
{
#if nsrs_SG14
    EXPECT( !!"pop_back() is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr) );
    EXPECT( rs.empty() );

    (void) rs.pop_back();
#endif
}

CASE( "ring_span: Removing an element from a full span makes it not full" )
{
    int arr[] = { 1, 2, 3, };
    ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    EXPECT( rs.full() );

    (void) rs.pop_front();

    EXPECT( ! rs.full() );
}

CASE( "ring_span: Removing an element from a full span doesn't change its capacity" )
{
    int arr[] = { 1, 2, 3, };
    ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    EXPECT( rs.full() );
    EXPECT( rs.capacity() == dim(arr) );

    (void) rs.pop_front();

    EXPECT( rs.capacity() == dim(arr) );
}

CASE( "ring_span: Allows to swap ring_spans (member)" )
{
    int arr1[] = { 1, 2, 3, }; ring_span<int> rs1( arr1, arr1 + dim(arr1), arr1, dim(arr1) );
    int arr2[] = { 9, 8, 7, }; ring_span<int> rs2( arr2, arr2 + dim(arr2), arr2, dim(arr2) );

    rs1.swap( rs2 );

    EXPECT( std::equal( rs1.begin(), rs1.end(), arr2 ) );
    EXPECT( std::equal( rs2.begin(), rs2.end(), arr1 ) );
}

CASE( "ring_span: Allows to swap ring_spans (non-member)" )
{
    int arr1[] = { 1, 2, 3, }; ring_span<int> rs1( arr1, arr1 + dim(arr1), arr1, dim(arr1) );
    int arr2[] = { 9, 8, 7, }; ring_span<int> rs2( arr2, arr2 + dim(arr2), arr2, dim(arr2) );

    swap( rs1, rs2 );

    EXPECT( std::equal( rs1.begin(), rs1.end(), arr2 ) );
    EXPECT( std::equal( rs2.begin(), rs2.end(), arr1 ) );
}

CASE( "ring_span: Allows to appear in range-for (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

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

CASE( "ring_span: Allows iteration (non-const)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( std::equal( rs.begin(), rs.end(), arr ) );
}

CASE( "ring_span: Allows iteration (const)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( std::equal( rs.cbegin(), rs.cend(), arr ) );
}

CASE( "ring_span: Allows reverse iteration (non-const)" )
{
#if nsrs_SG14
    EXPECT( !!"rbegin(), rend() are not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, };
    ring_span  <int> rs ( arr, arr + dim(arr), arr, dim(arr) );
    std::vector<int> vec( arr, arr + dim(arr) );

    EXPECT( std::equal( rs.rbegin(), rs.rend(), vec.rbegin() ) );
#endif
}

CASE( "ring_span: Allows reverse iteration (const)" )
{
#if nsrs_SG14
    EXPECT( !!"crbegin(), crend() are not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, };
    ring_span  <int> rs ( arr, arr + dim(arr), arr, dim(arr) );
    std::vector<int> vec( arr, arr + dim(arr) );

    EXPECT( std::equal( rs.crbegin(), rs.crend(), vec.rbegin() ) );
#endif
}

CASE( "ring_span: Allows to dereference iterator" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( *rs.begin() == arr[0] );
}

CASE( "ring_span: Allows to increment iterator (prefix)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    ring_span<int>::iterator pos = rs.begin();

    EXPECT( *++pos == arr[1] );
}

CASE( "ring_span: Allows to increment iterator (postfix)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    ring_span<int>::iterator pos = rs.begin();

    EXPECT( *pos++ == arr[0] );
}

CASE( "ring_span: Allows to decrement iterator (prefix)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    ring_span<int>::iterator pos = ++rs.begin();

    EXPECT( *--pos == arr[0] );
}

CASE( "ring_span: Allows to decrement iterator (postfix)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    ring_span<int>::iterator pos = ++rs.begin();

    EXPECT( *pos-- == arr[1] );
}

CASE( "ring_span: Allows to advance iterator (+=)" )
{
#if nsrs_SG14
    EXPECT( !!"iterator advancement is not implemented properly (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    ring_span<int>::iterator pos = rs.begin();

    EXPECT( *(pos += 2) == arr[2] );
#endif
}

CASE( "ring_span: Allows to advance iterator (-=)" )
{
#if nsrs_SG14
    EXPECT( !!"iterator advancement is not implemented properly (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    ring_span<int>::iterator pos = ++rs.begin(); ++pos;

    EXPECT( *(pos -= 2) == arr[0] );
#endif
}

CASE( "ring_span: Allows to offset iterator (+)" )
{
#if nsrs_SG14
    EXPECT( !!"iterator advancement is not implemented properly (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    ring_span<int>::iterator pos = rs.begin();

    EXPECT( *(pos + 2) == arr[2] );
#endif
}

CASE( "ring_span: Allows to offset iterator (-)" )
{
#if nsrs_SG14
    EXPECT( !!"iterator advancement is not implemented properly (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );
    ring_span<int>::iterator pos = ++rs.begin(); ++pos;

    EXPECT( *(pos - 2) == arr[0] );
#endif
}

CASE( "ring_span: Allows to obtain difference of iterators" )
{
#if nsrs_SG14
    EXPECT( !!"iterator difference is not available (SG14)" );
#else
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.end() - rs.begin() == difference_type( dim(arr) ) );
#endif
}

CASE( "ring_span: Allows to compare iterators (==)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.begin() == rs.begin() );
}

CASE( "ring_span: Allows to compare iterators (!=)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.begin() != rs.end() );
}

CASE( "ring_span: Allows to compare iterators (<)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.begin() < rs.end() );
}

CASE( "ring_span: Allows to compare iterators (<=)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.begin() <= rs.end()   );
    EXPECT( rs.begin() <= rs.begin() );
}

CASE( "ring_span: Allows to compare iterators (>)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.end() > rs.begin() );
}

CASE( "ring_span: Allows to compare iterators (>=)" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int> rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.end()   >= rs.begin() );
    EXPECT( rs.begin() >= rs.begin() );
}

CASE( "ring_span: A null popper returns void" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int, null_popper<int> > rs( arr, arr + dim(arr), arr, dim(arr) );

    rs.pop_front();
}

CASE( "ring_span: A null popper leaves the original element unchanged" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int, null_popper<int> > rs( arr, arr + dim(arr), arr, dim(arr) );

    rs.pop_front();

    EXPECT( arr[0] == 1 );
}

CASE( "ring_span: A default popper returns the element" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int, default_popper<int> > rs( arr, arr + dim(arr), arr, dim(arr) );

    EXPECT( rs.pop_front() == arr[0] );
}

CASE( "ring_span: A default popper moves the element (C++11)" )
{
#if nsrs_CPP11_OR_GREATER
    oracle arr[1]; ring_span<oracle, default_popper<oracle> > rs( arr, arr + dim(arr), arr, dim(arr) );
    EXPECT( oracle::state() == oracle::init );

    (void) rs.pop_front();

    EXPECT( oracle::state() == oracle::moved_from );
#else
    EXPECT( !!"move-semantics are not available (no C++11)" );
#endif
}

CASE( "ring_span: A default popper leaves the original element unchanged" )
{
    int arr[] = { 1, 2, 3, }; ring_span<int, default_popper<int> > rs( arr, arr + dim(arr), arr, dim(arr) );

    (void) rs.pop_front();

    EXPECT( arr[0] == 1 );
}

CASE( "ring_span: A copy popper returns the element" )
{
#if nsrs_SG14
    int arr[] = { 1, 2, 3, }; ring_span<int, copy_popper<int> > rs( arr, arr + dim(arr), arr, dim(arr), copy_popper<int>(7) );
#else
    int replacement = 7;
    int arr[] = { 1, 2, 3, }; ring_span<int, copy_popper<int> > rs( arr, arr + dim(arr), arr, dim(arr), copy_popper<int>(replacement) );
#endif
    int result = rs.pop_front() ;

    EXPECT( result == 1 );

}

CASE( "ring_span: A copy popper replaces the original element" )
{
#if nsrs_SG14
    int arr[] = { 1, 2, 3, }; ring_span<int, copy_popper<int> > rs( arr, arr + dim(arr), arr, dim(arr), copy_popper<int>(7) );

    (void) rs.pop_front() ;

    EXPECT( arr[0] == 7 );
#else
    int replacement = 7;
    int arr[] = { 1, 2, 3, }; ring_span<int, copy_popper<int> > rs( arr, arr + dim(arr), arr, dim(arr), copy_popper<int>(replacement) );

    (void) rs.pop_front() ;

    EXPECT( arr[0] == replacement );
#endif
}

//------------------------------------------------------------------------
// Applets:

#include <iostream>

CASE( "ring_span: filter" "[.app]" )
{
    double arr[]   = { 2.0 , 3.0, 5.0, };
    double coeff[] = { 0.25, 0.5, 0.25 };

    ring_span<double> buffer( arr, arr + dim(arr), arr, dim(arr) );

    std::cout << buffer << "\n";

    // new sample:
    buffer.push_back( 7.0 );

    std::cout << buffer << "\n";

    double result = std::inner_product( buffer.begin(), buffer.end(), coeff, 0.0 );

    std::cout << "filter result: " << result << "\n";

    EXPECT( result == 5.0 );
}

// end of file
