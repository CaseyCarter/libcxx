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

#include <iterator>

template <class T>
_LIBCPP_CONCEPT_DECL has_member_value_type =
    requires { typename T::value_type; };

struct A { using value_type = int; int& operator*() const; };
struct B : A { using value_type = double; };

static_assert(std::Same<int, std::iter_value_t<int*>>);
static_assert(std::Same<int, std::iter_value_t<int[]>>);
static_assert(std::Same<int, std::iter_value_t<int[4]>>);
static_assert(std::Same<int, std::iter_value_t<A>>);
static_assert(std::Same<double, std::iter_value_t<B>>);
static_assert(std::Same<int, std::iter_value_t<const int*>>);
static_assert(!has_member_value_type<std::readable_traits<void>>);
static_assert(!has_member_value_type<std::readable_traits<void*>>);
static_assert(std::Same<int, std::iter_value_t<const int* const>>);
static_assert(std::Same<int, std::iter_value_t<const int[2]>>);

int main() {}
