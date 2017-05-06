#include "ring_span.hpp"
#include <iostream>
#include <numeric>

template< typename T, size_t N >
inline size_t dim( T (&arr)[N] ) { return N; }

template< class T, class Popper >
void print( std::ostream & os, nonstd::ring_span<T,Popper> & rs )
{
    std::copy( rs.begin(), rs.end(), std::ostream_iterator<T>(os, ", ") );
}

int main()
{
    double arr[]   = { 2.0 , 3.0, 5.0, };
    double coeff[] = { 0.25, 0.5, 0.25 };

    nonstd::ring_span<double> buffer( arr, arr + dim(arr), arr, dim(arr) );

    std::cout << "buffer: "; print(std::cout, buffer); std::cout << "\n";

    // new sample:
    buffer.push_back( 7.0 );

    std::cout << "buffer: "; print(std::cout, buffer); std::cout << "\n";

    double result = std::inner_product( buffer.begin(), buffer.end(), coeff, 0.0 );
    
    std::cout << "result: " << result << "\n";
}

// g++ -std=c++98 -Wall -I../include/nonstd -o 01-filter.exe 01-filter.cpp && 01-filter.exe
