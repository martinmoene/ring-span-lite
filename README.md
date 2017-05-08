# ring-span lite - a circular buffer view for C++98 and later

(badges)

**Contents**  
- [Example usage](#example-usage)
- [In a nutshell](#in-a-nutshell)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Synopsis](#synopsis)
- [Reported to work with](#reported-to-work-with)
- [Building the tests](#building-the-tests)
- [Other ring_span implementations](#other-ring-span-implementations)
- [Notes and references](#notes-and-references)
- [Appendix](#appendix)


Example usage
-------------

```Cpp
#include "ring_span.hpp"
#include <iostream>
#include <numeric>

template< typename T, size_t N >
inline size_t dim( T (&arr)[N] ) { return N; }

template< typename T, class Popper>
inline std::ostream & operator<<( std::ostream & os, ::nonstd::ring_span<T, Popper> const & rs )
{
    os << "[ring_span: "; std::copy( rs.begin(), rs.end(), std::ostream_iterator<T>(os, ", ") ); return os << "]";
}

int main()
{
    double arr[]   = { 2.0 , 3.0, 5.0, };
    double coeff[] = { 0.25, 0.5, 0.25 };

    nonstd::ring_span<double> buffer( arr, arr + dim(arr), arr, dim(arr) );

    std::cout << buffer << "\n";

    // new sample:
    buffer.push_back( 7.0 );

    std::cout << buffer << "\n";

    double result = std::inner_product( buffer.begin(), buffer.end(), coeff, 0.0 );

    std::cout << "result: " << result << "\n";
}
```

### Compile and run

```
prompt> g++ -std=c++98 -Wall -I../include/nonstd -o 01-filter.exe 01-filter.cpp && 01-filter.exe
[ring_span: 2, 3, 5, ]
[ring_span: 3, 5, 7, ]
result: 5
```

In a nutshell
-------------
**ring-span lite** is a single-file header-only library to represent a circular buffer view on a container. The library aims to provide a [C++yy-like ring_span]() for use with C++98 and later [1][2]. Its code is inspired on Arthur O'Dwyer's reference implementation [3].

It is my intention to let the interface of this `ring_span` follow the unfolding standard one. 

**Features and properties of ring-span lite** are ease of installation (single header), freedom of dependencies other than the standard library.

**Limitations of ring-span lite** are ... .


License
-------
*ring-span lite* uses the [MIT](LICENSE) license.

 Dependencies
------------
*ring-span lite* has no other dependencies than the [C++ standard library](http://en.cppreference.com/w/cpp/header).


Installation
------------
*ring-span lite* is a single-file header-only library. Put `ring_span.hpp` in the [include](include) folder directly into the project source tree or somewhere reachable from your project.


Synopsis
--------

**Contents**  
- [Types in namespace nonstd](#types-in-namespace-nonstd)  
- [Interface of *ring-span lite*](#interface-of-ring-span-lite)  
- [Algorithms for *ring-span lite*](#algorithms-for-ring-span-lite)  
- [Configuration macros](#configuration-macros)
- [Macros to control alignment](#macros-to-control-alignment)  

### Types in namespace nonstd
(table)

### Interface of *ring-span lite*
(table)

### Algorithms for *ring-span lite*
(table)

### Configuration macros

Currently none.


Reported to work with
---------------------
The table below mentions the compiler versions *ring-span lite* is reported to work with.

(table)


Building the tests
------------------
To build the tests you need:

- [CMake](http://cmake.org), version 2.8.7 or later to be installed and in your PATH.
- A [suitable compiler](#reported-to-work-with). 

The [*lest* test framework](https://github.com/martinmoene/lest)  is included in the [test folder](test).
 
The following steps assume that the [ring-span lite source code](https://github.com/martinmoene/ring-span-lite) has been cloned into a directory named `c:\ring-span-lite`.

1. Create a directory for the build outputs for a particular architecture.  
Here we use c:\ring-span-lite\build-win-x86-vc10.

        cd c:\ring-span-lite
        md build-win-x86-vc10
        cd build-win-x86-vc10

2. Configure CMake to use the compiler of your choice (run `cmake --help` for a list).

        cmake -G "Visual Studio 10 2010" ..

3. Build the test suite in the Debug configuration (alternatively use Release).    

        cmake --build . --config Debug

4. Run the test suite.    

        ctest -V -C Debug

All tests should pass, indicating your platform is supported and you are ready to use *ring-span lite*. See the table with [supported types and functions](#features).


Other ring-span implementations
-------------------------------


Notes and references
--------------------
### References
[1] [A proposal to add a ring span to the standard library](http://wg21.link/p0059) ([latest](http://wg21.link/p0059), [r3](http://wg21.link/p0059r3), [r2](http://wg21.link/p0059r2), [r1](http://wg21.link/p0059r1), [r0](http://wg21.link/p0059r0)).  
[2] WG21-SG14/SG14. Reference implementation of [`std::ring_span`](https://github.com/WG21-SG14/SG14/blob/master/SG14/ring.h).  
[3] Arthur O'Dwyer. Reference implementation [`std::ring_span`](https://github.com/Quuxplusone/ring_view).  


Appendix
--------
### A.1 Ring-span Lite test specification

```
ring_span: Allows to construct an empty span from an iterator pair
ring_span: Allows to construct a partially filled span from an iterator pair and iterator, size
ring_span: Disallows to copy-construct from a ring_span (compile-time)
ring_span: Disallows to copy-assign from a ring_span (compile-time)
ring_span: Allows to move-construct from a ring_span (C++11)
ring_span: Allows to move-assign from a ring_span (C++11)
ring_span: Allows to obtain the capacity of a span
ring_span: Allows to obtain the number of elements in a span (size)
ring_span: Allows to check for an empty span
ring_span: Allows to check for a full span
ring_span: Allows to observe the element at the front
ring_span: Allows to observe the element at the back
ring_span: Allows to obtain and remove the element at the front
ring_span: Allows to obtain and remove the element at the back
ring_span: Allows to copy-insert an element at the front
ring_span: Allows to move-insert an element at the front (C++11)
ring_span: Allows to emplace an element at the front (C++11)
ring_span: Allows to copy-insert an element at the back
ring_span: Allows to move-insert an element at the back (C++11)
ring_span: Allows to emplace an element at the back (C++11)
ring_span: Adding an element to an empty span makes it non-empty
ring_span: Adding an element to an empty span doesn't change its capacity
ring_span: Adding an element to a full span leaves it full
ring_span: Adding an element to a full span doesn't change its capacity
ring_span: Removing an element from an empty span asserts !empty (front)[.assert]
ring_span: Removing an element from an empty span asserts !empty (back)[.assert]
ring_span: Removing an element from a full span makes it not full
ring_span: Removing an element from a full span doesn't change its capacity
ring_span: Allows to swap ring_spans (member)
ring_span: Allows to swap ring_spans (non-member)
ring_span: Allows to appear in range-for (C++11)
ring_span: Allows iteration (non-const)
ring_span: Allows iteration (const)
ring_span: Allows reverse iteration (non-const)
ring_span: Allows reverse iteration (const)
ring_span: Allows to dereference iterator
ring_span: Allows to increment iterator (prefix)
ring_span: Allows to increment iterator (postfix)
ring_span: Allows to decrement iterator (prefix)
ring_span: Allows to decrement iterator (postfix)
ring_span: Allows to advance iterator (+=)
ring_span: Allows to advance iterator (-=)
ring_span: Allows to offset iterator (+)
ring_span: Allows to offset iterator (-)
ring_span: Allows to obtain difference of iterators
ring_span: Allows to compare iterators (==)
ring_span: Allows to compare iterators (!=)
ring_span: Allows to compare iterators (<)
ring_span: Allows to compare iterators (<=)
ring_span: Allows to compare iterators (>)
ring_span: Allows to compare iterators (<=)
. . .
```
