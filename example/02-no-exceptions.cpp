#include "nonstd/ring_span.hpp"

using nonstd::ring_span;

template< typename T, size_t N >
inline size_t dim( T (&arr)[N] ) { return N; }

int main()
{
    double arr[] = { 0 , 0, 0, };

    ring_span<double> spn( arr, arr + dim(arr) );
}

// cl -nologo -I../include 02-no-exceptions.cpp && 02-no-exceptions
// g++ -Wall -fno-exceptions -I../include -o 02-no-exceptions 02-no-exceptions.cpp && 02-no-exceptions
