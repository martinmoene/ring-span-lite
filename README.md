# ring-span lite - a circular buffer view for C++98 and later

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)  [![Standard](https://img.shields.io/badge/c%2B%2B-98-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Standard](https://img.shields.io/badge/c%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Standard](https://img.shields.io/badge/c%2B%2B-14-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Standard](https://img.shields.io/badge/c%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT) [![Build Status](https://travis-ci.org/martinmoene/ring-span-lite.svg?branch=master)](https://travis-ci.org/martinmoene/ring-span-lite) [![Build status](https://ci.appveyor.com/api/projects/status/w2dgn3fxyrd6vcq8?svg=true)](https://ci.appveyor.com/project/martinmoene/ring-span-lite) [![Version](https://badge.fury.io/gh/martinmoene%2Fring-span-lite.svg)](https://github.com/martinmoene/ring-span-lite/releases) [![download](https://img.shields.io/badge/latest%20version%20%20-download-blue.svg)](https://raw.githubusercontent.com/martinmoene/ring-span-lite/master/include/nonstd/ring_span.hpp)

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
**ring-span lite** is a single-file header-only library to represent a circular buffer view on a container. The library aims to provide a [C++yy-like ring_span]() for use with C++98 and later [1][2]. Its code is inspired on Arthur O'Dwyer's reference implementation [3]. It is my intention to let the interface of this `ring_span` follow the unfolding standard one. 

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

### Types in namespace nonstd

| Purpose |[p0059](http://wg21.link/p0059)| Type | Notes |
|---------|:-----------------------------:|------|-------|
| Circular buffer view |&#10003;| template< class T, class Popper = default_popper<T> ><br>class ring_span | &nbsp; |
| Ignore element |&#10003;| template< class T ><br>class null_popper    | &nbsp; |
| Return element |&#10003;| template< class T ><br>class default_popper | &nbsp; |
| Return element, replace original |&#10003;| template< class T ><br>class copy_popper | &nbsp; |

### Interface of *ring-span lite*

| Types |[p0059](http://wg21.link/p0059)| Notes |
|-------|:--------------:|----------------------|
| type                   |&#10003;| ring_span< T, Popper > |
| size_type              |&#10003;| &nbsp; |
| value_type             |&#10003;| &nbsp; |
| pointer                |&#10003;| &nbsp; |
| reference              |&#10003;| &nbsp; |
| const_reference        |&#10003;| &nbsp; |
| iterator               |&#10003;| &nbsp; |
| const_iterator         |&#10003;| &nbsp; |
| reverse_iterator       |&ndash; | &nbsp; |
| const_reverse_iterator |&ndash; | &nbsp; |


| Kind |[p0059](http://wg21.link/p0059)| Method | Note / Result |
|-------|:--------------:|----------------------|---------------|
| Construction   |&#10003;| ring_span(<br>It begin, It end<br>, Popper popper = Popper() ) noexcept | create empty span of<br>distance(begin,end) capacity |
| &nbsp;         |&#10003;| ring_span(<br>It begin, It end<br>, It first, size_type size<br>, Popper popper = Popper() ) noexcept | create partially filled span of<br>distance(begin,end) capacity,<br>size elements |
| &nbsp;         |&#10003;| ring_span( ring_span && ) | = default (>= C++11) |
| &nbsp;         |&#10003;| ring_span& operator=( ring_span && ) | = default (>= C++11) |
| &nbsp;         |&#10003;| ring_span( ring_span const & ) | implicitly deleted (>= C++11) |
| &nbsp;         |&#10003;| ring_span & operator=( ring_span const & ); | implicitly deleted (>= C++11) |
| &nbsp;         |&ndash; | ring_span( ring_span const & ) | declared private (< C++11) |
| &nbsp;         |&ndash; | ring_span & operator=( ring_span const & ); | declared private (< C++11) |
| Observation    |&#10003;| empty()   | true if empty |
| &nbsp;         |&#10003;| full()    | true if full |
| &nbsp;         |&#10003;| size()    | current number of elements |
| &nbsp;         |&#10003;| capacity()| maximum number of elements |
| Element access |&#10003;| front()   | reference to element at front |
| &nbsp;         |&#10003;| front()   | const_reference to element at front |
| &nbsp;         |&#10003;| back()    | reference to back element at back |
| &nbsp;         |&#10003;| back()    | const_reference to element at back |
| Iteration      |&#10003;| begin()   | iterator |
| &nbsp;         |&#10003;| begin()   | const_iterator |
| &nbsp;         |&#10003;| cbegin()  | const_iterator |
| &nbsp;         |&#10003;| end()     | iterator |
| &nbsp;         |&#10003;| end()     | const_iterator |
| &nbsp;         |&#10003;| cend()    | const_iterator |
| &nbsp;         |&ndash; | rbegin()  | reverse_iterator |
| &nbsp;         |&ndash; | rbegin()  | const_reverse_iterator |
| &nbsp;         |&ndash; | crbegin() | const_reverse_iterator |
| &nbsp;         |&ndash; | rend()    | reverse_iterator |
| &nbsp;         |&ndash; | rend()    | const_reverse_iterator |
| &nbsp;         |&ndash; | crend()   | const_reverse_iterator |
| Element insertion|&#10003;| push_back( value_type const & value )   | void; SFINAE restrained for >= C++11 |
| &nbsp;         |&#10003;| push_back( value_type const & value )     | void; unrestrained for < C++11 |
| &nbsp;         |&#10003;| push_back( value_type && value )          | void; SFINAE restrained for >= C++11 |
| &nbsp;         |&#10003;| emplace_back( Args &&... args )           | void; SFINAE restrained for >= C++11 |
| &nbsp;         |&ndash;  | push_front( value_type const & value )    | void; SFINAE restrained for >= C++11 |
| &nbsp;         |&ndash;  | push_front( value_type const & value )    | void; unrestrained for < C++11 |
| &nbsp;         |&ndash;  | push_front( value_type && value )         | void; SFINAE restrained for >= C++11 |
| &nbsp;         |&ndash;  | emplace_front( Args &&... args )          | void; SFINAE restrained for >= C++11 |
| Element extraction|&#10003;| pop_front() | Popper::return_type (p0059 uses auto) |
| &nbsp;         |&ndash;    | pop_back()  | Popper::return_type |
| Swap           |&#10003;   | swap( ring_span & rhs ) noexcept | void; |

### Algorithms for *ring-span lite*

| Kind |[p0059](http://wg21.link/p0059)| Function |
|------|:--------------:|----------------------|
| Swap |&ndash;| void swap( ring_span<&hellip;> & lhs, ring_span<&hellip;> & rhs ) |

### Configuration macros

<!-- \-D<b>nsrs_STRICT_P0059</b>=0  
Define this to 1 to omit behaviour not present in proposal p0059. Default is 0.
-->
\-D<b>nsrs\_CONFIG\_CONFIRMS\_COMPILATION\_ERRORS</b>=0  
Define this to 1 to include the tests with compile-time errors. Default is 0.


Reported to work with
---------------------
The table below mentions the compiler versions *ring-span lite* is reported to work with.

OS        | Compiler   | Versions |
---------:|:-----------|:---------|
Windows   | Clang/LLVM | ? |
&nbsp;    | GCC        | 5.2.0, 6.3.0 |
&nbsp;    | Visual C++<br>(Visual Studio)| <!--8 (2005), -->10 (2010), 11 (2012),<br>12 (2013), 14 (2015, 2017) |
GNU/Linux | Clang/LLVM | 3.5.0 |
&nbsp;    | GCC        | 4.8.4 |
OS X      | ?          | ?   |


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
(TBD)

Notes and references
--------------------
### References
[1] [p0059: A proposal to add a ring span to the standard library](http://wg21.link/p0059) ([latest](http://wg21.link/p0059), [r3](http://wg21.link/p0059r3), [r2](http://wg21.link/p0059r2), [r1](http://wg21.link/p0059r1), [r0](http://wg21.link/p0059r0)).  
[2] [WG21-SG14/SG14](https://github.com/WG21-SG14/SG14/). Reference implementation of [`std::ring_span`](https://github.com/WG21-SG14/SG14/blob/master/SG14/ring.h).  
[3] [Arthur O'Dwyer](https://github.com/Quuxplusone). Reference implementation of [`std::ring_span`](https://github.com/Quuxplusone/ring_view).  


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
ring_span: Allows to compare iterators (>=)
ring_span: A null popper returns void
ring_span: A null popper leaves the original element unchanged
ring_span: A default popper returns the element
ring_span: A default popper moves the element (C++11)
ring_span: A default popper leaves the original element unchanged
ring_span: A copy popper returns the element
ring_span: A copy popper replaces the original element
```
