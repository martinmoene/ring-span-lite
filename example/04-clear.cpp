#include "nonstd/ring_span.hpp"
#include <memory>
#include <vector>

struct Widget{};

int main()
{
    std::vector< std::unique_ptr<Widget>> v( 10 );
    nonstd::ring_span< std::unique_ptr<Widget> > r( v.begin(), v.end() );

    r.push_back( std::make_unique<Widget>() );
    r.push_back( std::make_unique<Widget>() );

    r = nonstd::ring_span< std::unique_ptr<Widget> >( v.begin(), v.end() );
}

// g++ -std=c++14 -Wall -I../include -o 04-clear.exe 04-clear.cpp && 04-clear.exe
// cl -EHsc -I../include 04-clear.cpp && 04-clear.exe
