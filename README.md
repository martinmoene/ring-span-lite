# ring-span lite: A circular buffer view for C++98 and later

[![Language](https://img.shields.io/badge/C%2B%2B-98/11/14/17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)  [![License](https://img.shields.io/badge/license-BSL-blue.svg)](https://opensource.org/licenses/BSL-1.0) [![Build Status](https://github.com/martinmoene/ring-span-lite/actions/workflows/ci.yml/badge.svg)](https://github.com/martinmoene/ring-span-lite/actions/workflows/ci.yml) [![Version](https://badge.fury.io/gh/martinmoene%2Fring-span-lite.svg)](https://github.com/martinmoene/ring-span-lite/releases) [![download](https://img.shields.io/badge/latest-download-blue.svg)](https://raw.githubusercontent.com/martinmoene/ring-span-lite/master/include/nonstd/ring_span.hpp) [![Conan](https://img.shields.io/badge/on-conan-blue.svg)](https://conan.io/center/ring-span-lite) [![Vcpkg](https://img.shields.io/badge/on-vcpkg-blue.svg)](https://vcpkg.link/ports/ring-span-lite) [![Try it on wandbox](https://img.shields.io/badge/on-wandbox-blue.svg)](https://wandbox.org/permlink/GHo8T1PIo7TV7eoG) [![Try it on godbolt online](https://img.shields.io/badge/on-godbolt-blue.svg)](https://godbolt.org/z/7n4Byc)

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
#include "nonstd/ring_span.hpp"
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

    std::cout << "filter result: " << result << "\n";
}
```

### Compile and run

```Text
prompt> g++ -std=c++98 -Wall -I../include -o 01-filter.exe 01-filter.cpp && 01-filter.exe
[ring_span: 2, 3, 5, ]
[ring_span: 3, 5, 7, ]
filter result: 5
```

Or to run with [Buck](https://buckbuild.com/):

```Text
prompt> buck run example/:01-filter 
```

In a nutshell
-------------

**ring-span lite** is a single-file header-only library to represent a circular buffer view on a container. The library aims to provide a [C++yy-like ring_span]() for use with C++98 and later [1][2]. Its initial code is inspired on the reference implementation by Arthur O'Dwyer [3]. It is my intention to let the interface of this `ring_span` follow the unfolding standard one.

This library also includes header `<ring.hpp>` to provide a data-owning ring buffer.

**Features and properties of ring-span lite** are ease of installation (single header), freedom of dependencies other than the standard library.

**Limitations of ring-span lite** are ... .

License
-------

*ring-span lite* is distributed under the [Boost Software License](LICENSE.txt).

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
- [Non-member functions for *ring-span lite*](#non-member-functions-for-ring-span-lite)
- [Configuration macros](#configuration-macros)

### Types in namespace nonstd

| Purpose |[p0059](http://wg21.link/p0059)| Type | Notes |
|---------|:-----------------------------:|------|-------|
| Circular buffer view |&#10003;/&ndash;| template<<br>&emsp;class T<br>&emsp;, class Popper = default_popper&lt;T><br>&emsp;, bool `CapacityIsPowerOf2` = false<br>><br>class **ring_span** | See Note 1 below. |
| Ignore element |&#10003;| template< class T ><br>class **null_popper**    | &nbsp; |
| Return element |&#10003;| template< class T ><br>class **default_popper** | &nbsp; |
| Return element, replace original |&#10003;| template< class T ><br>class **copy_popper** | &nbsp; |

Note 1: `CapacityIsPowerOf2` is an extension (`nsrs_CONFIG_STRICT_P0059=0`).With `CapacityIsPowerOf2` being `true`, method `normalize_()` is optimized to use bitwise and instead of modulo division.

### Interface of *ring-span lite*

#### Class `ring_span`

| Kind |[p0059](http://wg21.link/p0059)| Type / Method | Note / Result |
|-------|:--------------:|-----------------------------|---------------|
| Various types  |&#10003;| **type**                   |ring_span&lt;T, Popper\[, CapacityIsPowerOf2\]> |
| &nbsp;         |&#10003;| **size_type**              |&nbsp; |
| Value types    |&#10003;| **value_type**             |&nbsp; |
| &nbsp;         |&#10003;| **pointer**                |&nbsp; |
| &nbsp;         |&#10003;| **reference**              |&nbsp; |
| &nbsp;         |&#10003;| **const_reference**        |&nbsp; |
| Iterator types |&#10003;| **iterator**               |&nbsp; |
| &nbsp;         |&#10003;| **const_iterator**         |&nbsp; |
| &nbsp;         |&ndash; | **reverse_iterator**       |&nbsp; |
| &nbsp;         |&ndash; | **const_reverse_iterator** |&nbsp; |
| Construction   |&#10003;| **ring_span**(<br>It begin, It end<br>, Popper popper = Popper() ) noexcept | create empty span of<br>distance(begin,end) capacity |
| &nbsp;         |&#10003;| **ring_span**(<br>It begin, It end<br>, It first, size_type size<br>, Popper popper = Popper() ) noexcept | create partially filled span of<br>distance(begin,end) capacity,<br>size elements |
| &nbsp;         |&#10003;| **ring_span**( ring_span && ) | = default (>= C++11) |
| &nbsp;         |&#10003;| ring_span& **operator=**( ring_span && ) | = default (>= C++11) |
| &nbsp;         |&#10003;| **ring_span**( ring_span const & ) | implicitly deleted (>= C++11) |
| &nbsp;         |&#10003;| ring_span & **operator=**( ring_span const & ); | implicitly deleted (>= C++11) |
| &nbsp;         |&ndash; | **ring_span**( ring_span const & ) | declared private (< C++11) |
| &nbsp;         |&ndash; | ring_span & **operator=**( ring_span const & ); | declared private (< C++11) |
| Iteration      |&#10003;| **begin**() noexcept   | iterator |
| &nbsp;         |&#10003;| **begin**() noexcept   | const_iterator |
| &nbsp;         |&#10003;| **cbegin**() noexcept  | const_iterator |
| &nbsp;         |&#10003;| **end**() noexcept     | iterator |
| &nbsp;         |&#10003;| **end**() noexcept     | const_iterator |
| &nbsp;         |&#10003;| **cend**() noexcept    | const_iterator |
| Reverse iter.  |&ndash; | **rbegin**() noexcept  | reverse_iterator |
| &nbsp;         |&ndash; | **rbegin**() noexcept  | const_reverse_iterator |
| &nbsp;         |&ndash; | **crbegin**() noexcept | const_reverse_iterator |
| &nbsp;         |&ndash; | **rend**() noexcept    | reverse_iterator |
| &nbsp;         |&ndash; | **rend**() noexcept    | const_reverse_iterator |
| &nbsp;         |&ndash; | **crend**() noexcept   | const_reverse_iterator |
| Observation    |&#10003;| **empty**() noexcept   | true if empty |
| &nbsp;         |&#10003;| **full**() noexcept    | true if full |
| &nbsp;         |&#10003;| **size**() noexcept    | current number of elements |
| &nbsp;         |&#10003;| **capacity**() noexcept| maximum number of elements |
| Element access |&#10003;| **front**() noexcept   | reference to element at front |
| &nbsp;         |&#10003;| **front**() noexcept   | const_reference to element at front |
| &nbsp;         |&#10003;| **back**() noexcept    | reference to back element at back |
| &nbsp;         |&#10003;| **back**() noexcept    | const_reference to element at back |
| &nbsp;         |&ndash; | **operator[]**( size_type idx ) noexcept | reference to element at specified index |
| &nbsp;         |&ndash; | **operator[]**( size_type idx ) noexcept | const_reference to element at specified index |
| Elem.extraction|&#10003;| **pop_front**() | Popper::return_type (p0059: auto) |
| &nbsp;         |&ndash; | **pop_back**()  | Popper::return_type |
| Elem.insertion|&#10003; | **push_back**( value_type const & value ) noexcept(&hellip;) | void; restrained (>= C++11) |
| &nbsp;         |&ndash; | **push_back**( value_type const & value )     | void; unrestrained (< C++11) |
| &nbsp;         |&#10003;| **push_back**( value_type && value ) noexcept(&hellip;) | void; restrained (>= C++11) |
| &nbsp;         |&#10003;| **emplace_back**( Args &&... args ) noexcept(&hellip;)| void; restrained (>= C++11) |
| &nbsp;         |&ndash; | **push_front**( value_type const & value ) noexcept(&hellip;) | void; restrained (>= C++11) |
| &nbsp;         |&ndash; | **push_front**( value_type const & value )    | void; unrestrained (< C++11) |
| &nbsp;         |&ndash; | **push_front**( value_type && value ) noexcept(&hellip;)| void; restrained (>= C++11) |
| &nbsp;         |&ndash; | **emplace_front**( Args &&... args ) noexcept(&hellip;) | void; restrained (>= C++11) |
| Swap           |&#10003;| **swap**( ring_span & rhs ) noexcept | void; |

#### Class `ring_iterator`

| Kind |[p0059](http://wg21.link/p0059)| Type / Method  | Note / Result |
|-------|:--------------:|------------------------------|---------------|
| Various types |&#10003;| **type**                         |ring_span< T, Popper > |
| &nbsp;        |&#10003;| **difference_type**              |&nbsp; |
| Value types   |&#10003;| **value_type**                   |&nbsp; |
| &nbsp;        |&#10003;| **pointer**                      |&nbsp; |
| &nbsp;        |&#10003;| **reference**                    |&nbsp; |
| Category      |&#10003;| **iterator_category**            |&nbsp; |
| Construction  |&#10003;| **ring_iterator**()              | = default (>= C++11) |
| &nbsp;        |&ndash; | **ring_iterator**()              | (< C++11) |
| Conversion    |&ndash; | **operator ring_iterator<&hellip;,true>**() const noexcept| const ring_iterator |
| Element access|&#10003;| **operator\***() const noexcept  |reference |
| Increment     |&#10003;| **operator++**() noexcept        |ring_iterator<&hellip;> & |
| &nbsp;        |&#10003;| **operator++**( int ) noexcept   |ring_iterator<&hellip;> |
| Decrement     |&#10003;| **operator--**() noexcept        |ring_iterator<&hellip;> & |
| &nbsp;        |&#10003;| **operator--**( int ) noexcept   |ring_iterator<&hellip;> |
| Addition      |&#10003;| **operator+=**( int i ) noexcept |ring_iterator<&hellip;> & |
| &nbsp;        |&#10003;| **operator-=**( int i ) noexcept |ring_iterator<&hellip;> & |
| Difference    |&ndash; | **operator-**( ring_iterator<&hellip;> const & rhs ) | difference_type, Note 1 |
| Comparison    |&#10003;| **operator==**( ring_iterator<&hellip;> const & rhs ) const noexcept |bool, Note 1 |
| &nbsp;        |&#10003;| **operator!=**( ring_iterator<&hellip;> const & rhs ) const noexcept |bool, Note 1 |
| &nbsp;        |&#10003;| **operator<**( ring_iterator<&hellip;> const & rhs ) const noexcept  |bool, Note 1 |
| &nbsp;        |&#10003;| **operator<=**( ring_iterator<&hellip;> const & rhs ) const noexcept |bool, Note 1 |
| &nbsp;        |&#10003;| **operator>**( ring_iterator<&hellip;> const & rhs ) const noexcept  |bool, Note 1 |
| &nbsp;        |&#10003;| **operator>=**( ring_iterator<&hellip;> const & rhs ) const noexcept |bool, Note 1 |

Note 1: accepts lhs and rhs of different const-ness.

### Non-member functions for *ring-span lite*

| Kind            |[p0059](http://wg21.link/p0059)| Function | Note / Result |
|-----------------|:-----------------------------:|----------|--------|
| Swap            |&ndash;/&#10003;| **swap**( ring_span<&hellip;> & lhs, ring_span<&hellip;> & rhs ) |void |
| Iterator offset |&#10003;| **operator+**( ring_iterator<&hellip;> it, int i ) noexcept | ring_iterator<&hellip;> |
| &nbsp;          |&ndash; | **operator+**( int i, ring_iterator<&hellip;> it ) noexcept | ring_iterator<&hellip;> |
| &nbsp;          |&#10003;| **operator-**( ring_iterator<&hellip;> it, int i ) noexcept | ring_iterator<&hellip;> |
| &nbsp;          |&ndash; | **operator-**( int i, ring_iterator<&hellip;> it ) noexcept | ring_iterator<&hellip;> |

Legenda:&ensp;&ndash; not in proposal&ensp;&middot;&ensp;&#10003; in proposal&ensp;&middot;&ensp;&ndash;/&#10003; not in proposal/in sg14 code

#### Class `ring`

| Kind | Type / Method | Note / Result |
|------|---------------|---------------|
| Circular buffer | template<<br>&emsp;class Container<br>&emsp;, bool `CapacityIsPowerOf2` = false<br>><br>class **ring** | See Note 1 below. |
| Various types  | **size_type**              |&nbsp; |
| Value types    | **value_type**             |&nbsp; |
| &nbsp;         | **reference**              |&nbsp; |
| &nbsp;         | **const_reference**        |&nbsp; |
| Iterator types | **iterator**               |&nbsp; |
| &nbsp;         | **const_iterator**         |&nbsp; |
| &nbsp;         | **reverse_iterator**       |&nbsp; |
| &nbsp;         | **const_reverse_iterator** |&nbsp; |
| Construction   | **ring**()                 | create empty ring,<br>C-array, `std::array` |
| &nbsp;         | **ring**(size_type size)   | create empty ring of capacity `size`,<br>dynamic container |
| Iteration      | **begin**() noexcept       | iterator |
| &nbsp;         | **begin**() noexcept       | const_iterator |
| &nbsp;         | **cbegin**() noexcept      | const_iterator |
| &nbsp;         | **end**() noexcept         | iterator |
| &nbsp;         | **end**() noexcept         | const_iterator |
| &nbsp;         | **cend**() noexcept        | const_iterator |
| Reverse iter.  | **rbegin**() noexcept      | reverse_iterator |
| &nbsp;         | **rbegin**() noexcept      | const_reverse_iterator |
| &nbsp;         | **crbegin**() noexcept     | const_reverse_iterator |
| &nbsp;         | **rend**() noexcept        | reverse_iterator |
| &nbsp;         | **rend**() noexcept        | const_reverse_iterator |
| &nbsp;         | **crend**() noexcept       | const_reverse_iterator |
| Observation    | **empty**() noexcept       | true if empty |
| &nbsp;         | **full**() noexcept        | true if full |
| &nbsp;         | **size**() noexcept        | current number of elements |
| &nbsp;         | **capacity**() noexcept    | maximum number of elements |
| Element access | **front**() noexcept       | reference to element at front |
| &nbsp;         | **front**() noexcept       | const_reference to element at front |
| &nbsp;         | **back**() noexcept        | reference to back element at back |
| &nbsp;         | **back**() noexcept        | const_reference to element at back |
| Element access | **front**() noexcept       | reference to element at front |
| &nbsp;         | **front**() noexcept       | const_reference to element at front |
| &nbsp;         | **back**() noexcept        | reference to back element at back |
| &nbsp;         | **back**() noexcept        | const_reference to element at back |
| &nbsp;         | **operator[]**( size_type idx ) noexcept | reference to element at specified index |
| &nbsp;         | **operator[]**( size_type idx ) noexcept | const_reference to element at specified index |
| Elem.extraction| **pop_front**() | Popper::return_type |
| &nbsp;         | **pop_back**()  | Popper::return_type |
| Elem.insertion|& **push_back**( value_type const & value ) noexcept(&hellip;) | void; restrained (>= C++11) |
| &nbsp;         | **push_back**( value_type const & value )     | void; unrestrained (< C++11) |
| &nbsp;         | **push_back**( value_type && value ) noexcept(&hellip;) | void; restrained (>= C++11) |
| &nbsp;         | **emplace_back**( Args &&... args ) noexcept(&hellip;)| void; restrained (>= C++11) |
| &nbsp;         | **push_front**( value_type const & value ) noexcept(&hellip;) | void; restrained (>= C++11) |
| &nbsp;         | **push_front**( value_type const & value )    | void; unrestrained (< C++11) |
| &nbsp;         | **push_front**( value_type && value ) noexcept(&hellip;)| void; restrained (>= C++11) |
| &nbsp;         | **emplace_front**( Args &&... args ) noexcept(&hellip;) | void; restrained (>= C++11) |
| Swap           | **swap**( ring_span & rhs ) noexcept | void; |

Note 1: `CapacityIsPowerOf2` is an extension (`nsrs_CONFIG_STRICT_P0059=0`).With `CapacityIsPowerOf2` being `true`, method `normalize_()` is optimized to use bitwise and instead of modulo division. Class `default_popper` is used as popper.

### Configuration macros

#### Tweak header

If the compiler supports [`__has_include()`](https://en.cppreference.com/w/cpp/preprocessor/include), *ring-span lite* supports the [tweak header](https://vector-of-bool.github.io/2020/10/04/lib-configuration.html) mechanism. Provide your *tweak header* as `nonstd/ring_span.tweak.hpp` in a folder in the include-search-path. In the tweak header, provide definitions as documented below, like `#define nsrs_CPLUSPLUS 201103L`.

#### Standard selection macro

\-D<b>nsrs\_CPLUSPLUS</b>=199711L  
Define this macro to override the auto-detection of the supported C++ standard, if your compiler does not set the `__cpluplus` macro correctly.

#### Select `std::ring_span` or `nonstd::ring_span`

At default, *ring_span lite* uses `std::ring_span` if it is available and lets you use it via namespace `nonstd`. You can however override this default and explicitly request to use `std::ring_span` or ring_span lite's `nonstd::ring_span` as `nonstd::ring_span` via the following macros.

-D<b>nsrs\_CONFIG\_SELECT\_RING\_SPAN</b>=nsrs_RING_SPAN_DEFAULT  
Define this to `nsrs_RING_SPAN_STD` to select `std::ring_span` as `nonstd::ring_span`. Define this to `nsrs_RING_SPAN_NONSTD` to select `nonstd::ring_span` as `nonstd::ring_span`. Default is undefined, which has the same effect as defining to `nsrs_RING_SPAN_DEFAULT`.

#### Disable extensions

\-D<b>nsrs\_CONFIG\_STRICT\_P0059</b>=0  
Define this to 1 to omit behaviour not present in proposal [p0059](http://wg21.link/p0059). Default is undefined (same effect as 0).

#### Enable popper empty base class optimization

\-D<b>nsrs\_CONFIG\_POPPER\_EMPTY\_BASE\_CLASS</b>=0  
Poppers are often stateless. To prevent they take up space C++20 attribute `[[no_unique_address]]` is used when available. Another way to prevent up taking space is to make the popper a base class of `class ring_span`. This is what occurs with macro `nsrs_CONFIG_POPPER_EMPTY_BASE_CLASS` defined to 1. This is an extension to proposal p0059. Disabling extensions via macro `nsrs_CONFIG_STRICT_P0059` also disables this extension. Default is undefined (same effect as 0).

#### Enable compilation errors

\-D<b>nsrs\_CONFIG\_CONFIRMS\_COMPILATION\_ERRORS</b>=0  
Define this to 1 to include the tests with compile-time errors. Default is undefined (same effect as 0).

Reported to work with
---------------------

The table below mentions the compiler versions *ring-span lite* is reported to work with.

OS        | Compiler   | Versions |
---------:|:-----------|:---------|
Windows   | Clang/LLVM | ? |
&nbsp;    | GCC        | 5.2.0, 6.3.0 |
&nbsp;    | Visual C++<br>(Visual Studio)| 8 (2005), 10 (2010), 11 (2012),<br>12 (2013), 14 (2015, 2017) |
GNU/Linux | Clang/LLVM | 3.5.0 |
&nbsp;    | GCC        | 4.8.4 |
OS X      | ?          | ?   |

Building the tests
------------------

To build the tests you need:
- [Buck](https://buckbuild.com/) or [CMake](http://cmake.org) version 2.8.7 or later to be installed and in your PATH.
- A [suitable compiler](#reported-to-work-with).

The [*lest* test framework](https://github.com/martinmoene/lest)  is included in the [test folder](test).

### Buck

To run the tests:

```Text
prompt> buck run test/
```

### CMake

The following steps assume that the [ring-span lite source code](https://github.com/martinmoene/ring-span-lite) has been cloned into a directory named `c:\ring-span-lite`.

1. Create a directory for the build outputs for a particular architecture.  
Here we use c:\ring-span-lite\build-win-x86-vc10.

        cd c:\ring-span-lite
        md build-win-x86-vc10
        cd build-win-x86-vc10

2. Configure CMake to use the compiler of your choice (run `cmake --help` for a list).

        cmake -G "Visual Studio 10 2010" [see 3. below] ..

3. Optional. You can control above configuration through the following options:

   - `-DRING_SPAN_LITE_COLOURISE_TEST=ON`: use colour for pass, fail, default off

4. Build the test suite in the Debug configuration (alternatively use Release).    

        cmake --build . --config Debug

5. Run the test suite.    

        ctest -V -C Debug

All tests should pass, indicating your platform is supported and you are ready to use *ring-span lite*. See the table with [supported types and functions](#features).

Other ring-span implementations
-------------------------------

- Bjørn Reese. [Circular span](https://github.com/breese/trial.circular).
- Jan Wilmans. [ring_span](https://github.com/janwilmans/ring_span), based on code by Björn Fahller.

Notes and references
--------------------

### References

[1] [p0059: A proposal to add a ring span to the standard library](http://wg21.link/p0059) ([latest](http://wg21.link/p0059), [r4](http://wg21.link/p0059r4), [r3](http://wg21.link/p0059r3), [r2](http://wg21.link/p0059r2), [r1](http://wg21.link/p0059r1), [r0](http://wg21.link/p0059r0)).  
[2] [WG21-SG14/SG14](https://github.com/WG21-SG14/SG14/). Reference implementation of [`std::ring_span`](https://github.com/WG21-SG14/SG14/blob/master/SG14/ring.h) by [Guy Davidson](https://github.com/hatcat) and [Arthur O'Dwyer](https://github.com/Quuxplusone).  
[3] [Arthur O'Dwyer](https://github.com/Quuxplusone). Reference implementation of [`std::ring_span`](https://github.com/Quuxplusone/ring_view).  
[4] Arthur O’Dwyer. [Reference types with metadata cause problems](https://quuxplusone.github.io/blog/2018/05/30/reference-types-with-metadata-cause-problems/). 30 May 2018.  
[5] Phillip Johnston. [Creating a Circular Buffer in C and C++](https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc). 17 May 2017.  
[6] Jan Gaspar. [Boost.Circular Buffer](http://www.boost.org/libs/circular_buffer).  

Appendix
--------

**Contents**

- [A.1 Applets](#a1)
- [A.2 Compile-time information](#a2)
- [A.3 Ring-span lite test specification](#a3)

<a id="a1"></a>
### A.1 Applets

Applets demonstrate a specific use case. They are available via tag `[.applet]`.

```Text
> ring-span-main.t.exe -l .applet
ring_span: filter[.applet]
```

<a id="a2"></a>
### A.2 Compile-time information

The version of *ring-span lite* is available via tag `[.version]`. The following tags are available for information on the compiler and on the C++ standard library used: `[.compiler]`, `[.stdc++]`, `[.stdlanguage]` and `[.stdlibrary]`.

<a id="a3"></a>
### A.3 Ring-span lite test specification

Note: test cases that assert are tagged with `[.assert]` and only run when [.assert] is included on the command line, like: `test [.assert] partial-test-name`.

<details>
<summary>click to expand</summary>
<p>

```Text
ring_span: Allows to construct an empty span from an iterator pair
ring_span: Allows to construct an empty span from an iterator pair - capacity is power of 2
ring_span: Allows to construct a partially filled span from an iterator pair and iterator, size
ring_span: Allows to construct a partially filled span from an iterator pair and iterator, size - capacity is power of 2
ring_span: Disallows to copy-construct from a ring_span (compile-time)
ring_span: Disallows to copy-assign from a ring_span (compile-time)
ring_span: Allows to move-construct from a ring_span (C++11)
ring_span: Allows to move-assign from a ring_span (C++11)
ring_span: Allows to obtain the capacity of a span
ring_span: Allows to obtain the number of elements in a span (size)
ring_span: Allows to check for an empty span
ring_span: Allows to check for a full span
ring_span: Allows to observe the element at the specified index [extension]
ring_span: Allows to observe the element at the front
ring_span: Allows to observe the element at the back
ring_span: Allows to obtain and remove the element at the front
ring_span: Allows to obtain and remove the element at the back [extension]
ring_span: Allows to copy-insert an element at the front [extension]
ring_span: Allows to move-insert an element at the front (C++11) [extension]
ring_span: Allows to emplace an element at the front (C++11) [extension]
ring_span: Allows to copy-insert an element at the back
ring_span: Allows to move-insert an element at the back (C++11)
ring_span: Allows to emplace an element at the back (C++11)
ring_span: Adding an element to an empty span makes it non-empty (front) [extension]
ring_span: Adding an element to an empty span makes it non-empty (back)
ring_span: Adding an element to an empty span doesn't change its capacity (front) [extension]
ring_span: Adding an element to an empty span doesn't change its capacity (back)
ring_span: Adding an element to a full span leaves it full (front) [extension]
ring_span: Adding an element to a full span leaves it full (back)
ring_span: Adding an element to a full span doesn't change its capacity (front) [extension]
ring_span: Adding an element to a full span doesn't change its capacity (back)
ring_span: Removing an element from a span with one element makes it empty (front)
ring_span: Removing an element from a span with one element makes it empty (back) [extension]
ring_span: Removing an element from a span with one element doesn't change its capacity (front)
ring_span: Removing an element from a span with one element doesn't change its capacity (back) [extension]
ring_span: Removing an element from a full span makes it not full (front)
ring_span: Removing an element from a full span makes it not full (back) [extension]
ring_span: Removing an element from a full span doesn't change its capacity (front)
ring_span: Removing an element from a full span doesn't change its capacity (back) [extension]
ring_span: Allows to swap spans (member)
ring_span: Allows to swap spans (non-member)
ring_span: Allows to appear in range-for (C++11)
ring_span: Allows iteration (non-const)
ring_span: Allows iteration (const)
ring_span: Allows iteration (mixed const-non-const)
ring_span: Allows reverse iteration (non-const) [extension]
ring_span: Allows reverse iteration (const) [extension]
ring_span: Allows reverse iteration (mixed const-non-const) [extension]
ring_span: A span with capacity zero is both empty and full
ring_span: A full span is a delay-line of capacity elements (back-front)
ring_span: A full span is a delay-line of capacity elements (front-back) [extension]
ring_span: A non-full span is a stack of capacity elements (back) [extension]
ring_span: A non-full span is a stack of capacity elements (front) [extension]
ring_span: A non-full span behaves like an harmonica (back-front)
ring_span: A non-full span behaves like an harmonica (front-back) [extension]
ring_iterator: Allows conversion to const ring_iterator [extension]
ring_iterator: Allows to dereference iterator (operator*())
ring_iterator: Allows to dereference iterator (operator->())
ring_iterator: Allows to index from iterator (operator[](size_t))
ring_iterator: Allows to increment iterator (prefix)
ring_iterator: Allows to increment iterator (postfix)
ring_iterator: Allows to decrement iterator (prefix)
ring_iterator: Allows to decrement iterator (postfix)
ring_iterator: Allows to advance iterator (+=) [extension]
ring_iterator: Allows to advance iterator (-=) [extension]
ring_iterator: Allows to offset iterator (+) [extension]
ring_iterator: Allows to offset iterator (-) [extension]
ring_iterator: Allows to obtain difference of iterators [extension]
ring_iterator: Allows to compare iterators (==)
ring_iterator: Allows to compare iterators (!=)
ring_iterator: Allows to compare iterators (<)
ring_iterator: Allows to compare iterators (<=)
ring_iterator: Allows to compare iterators (>)
ring_iterator: Allows to compare iterators (>=)
ring_iterator: Allows to compare iterators (mixed const-non-const)
null_popper: A null popper returns void
null_popper: A null popper leaves the original element unchanged
default_popper: A default popper returns the element
default_popper: A default popper moves the element (C++11)
default_popper: A default popper leaves the original element unchanged
copy_popper: A copy popper returns the element
copy_popper: A copy popper replaces the original element
ring: Allows to create data owning ring from container
ring: Allows to create data owning ring from container - capacity is power of 2
ring: Allows to create data owning ring from std::array (C++11)
ring: Allows to create data owning ring from C-array
tweak header: reads tweak header if supported [tweak]
```

</p>
</details>
