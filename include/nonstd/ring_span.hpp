//
// Copyright (c) 2017 Martin Moene
//
// https://github.com/martinmoene/ring-span-lite
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

#pragma once

#ifndef NONSTD_RING_SPAN_LITE_HPP
#define NONSTD_RING_SPAN_LITE_HPP

//#include <typeinfo>
//#include <utility>

#define  ring_span_lite_VERSION "0.0.0"

// ring-span-lite configuration:

// ring-span-lite alignment configuration:

// Compiler detection (C++17 is speculative):

#define ring_span_CPP11_OR_GREATER  ( __cplusplus >= 201103L )
#define ring_span_CPP14_OR_GREATER  ( __cplusplus >= 201402L )
#define ring_span_CPP17_OR_GREATER  ( __cplusplus >= 201700L )

// half-open range [lo..hi):
#define ring_span_BETWEEN( v, lo, hi ) ( lo <= v && v < hi )

#if defined(_MSC_VER) && !defined(__clang__)
# define ring_span_COMPILER_MSVC_VERSION   (_MSC_VER / 100 - 5 - (_MSC_VER < 1900))
#else
# define ring_span_COMPILER_MSVC_VERSION   0
#endif

#if defined __GNUC__
# define ring_span_COMPILER_GNUC_VERSION  __GNUC__
#else
# define ring_span_COMPILER_GNUC_VERSION    0
#endif

// Presence of C++11 language features:

#if ring_span_CPP11_OR_GREATER || ring_span_COMPILER_MSVC_VERSION >= 10
# define ring_span_HAVE_NULLPTR  1
#endif

#if ring_span_CPP11_OR_GREATER || ring_span_COMPILER_MSVC_VERSION >= 12
# define ring_span_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG  1
# define ring_span_HAVE_INITIALIZER_LIST  1
#endif

#if ring_span_CPP11_OR_GREATER || ring_span_COMPILER_MSVC_VERSION >= 14
# define ring_span_HAVE_CONSTEXPR_11  1
# define ring_span_HAVE_NOEXCEPT  1
#endif

// Presence of C++14 language features:

#if ring_span_CPP14_OR_GREATER
# define ring_span_HAVE_CONSTEXPR_14  1
#endif

// Presence of C++17 language features:

// Presence of C++ library features:

#if ring_span_COMPILER_GNUC_VERSION
# define ring_span_HAVE_TR1_TYPE_TRAITS  1
# define ring_span_HAVE_TR1_ADD_CONST  1
# define ring_span_HAVE_TR1_REMOVE_REFERENCE  1
#endif

#if ring_span_CPP11_OR_GREATER || ring_span_COMPILER_MSVC_VERSION >= 9
# define ring_span_HAVE_TYPE_TRAITS  1
# define ring_span_HAVE_STD_ADD_CONST  1
# define ring_span_HAVE_STD_REMOVE_REFERENCE  1
#endif

// For the rest, consider VC14 as C++11 for ring-span-lite:

#if ring_span_COMPILER_MSVC_VERSION >= 14
# undef  ring_span_CPP11_OR_GREATER
# define ring_span_CPP11_OR_GREATER  1
#endif

// C++ feature usage:

#if ring_span_HAVE_CONSTEXPR_11
# define ring_span_constexpr constexpr
#else
# define ring_span_constexpr /*constexpr*/
#endif

#if ring_span_HAVE_CONSTEXPR_14
# define ring_span_constexpr14 constexpr
#else
# define ring_span_constexpr14 /*constexpr*/
#endif

#if ring_span_HAVE_NOEXCEPT
# define ring_span_noexcept noexcept
#else
# define ring_span_noexcept /*noexcept*/
#endif

#if ring_span_HAVE_NULLPTR
# define ring_span_nullptr nullptr
#else
# define ring_span_nullptr NULL
#endif

// additional includes:

#if ! ring_span_HAVE_NULLPTR
# include <cstddef>
#endif

#if ring_span_HAVE_INITIALIZER_LIST
# include <initializer_list>
#endif

#if ring_span_HAVE_TYPE_TRAITS
# include <type_traits>
#elif ring_span_HAVE_TR1_TYPE_TRAITS
# include <tr1/type_traits>
#endif

namespace nonstd {

class ring_span{};

} // namespace nonstd

#endif // NONSTD_RING_SPAN_LITE_HPP
