//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17
// REQUIRES: concepts

#include <cstddef>
#include <iterator>

template <class T>
_STL_CONCEPT_DECL has_member_difference_type =
    requires { typename T::difference_type; };

static_assert(std::Same<std::ptrdiff_t, std::iter_difference_t<int*>>);
static_assert(std::Same<std::ptrdiff_t, std::iter_difference_t<int[]>>);
static_assert(std::Same<std::ptrdiff_t, std::iter_difference_t<int[4]>>);
static_assert(std::Same<int, std::iter_difference_t<int>>);

static_assert(!has_member_difference_type<std::incrementable_traits<void>>);
static_assert(!has_member_difference_type<std::incrementable_traits<void*>>);

int main() {}
