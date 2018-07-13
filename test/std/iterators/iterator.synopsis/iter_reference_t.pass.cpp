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

struct A { using value_type = int; int& operator*() const; };
struct B : A { using value_type = double; };

static_assert(std::Same<int&, std::iter_reference_t<int*>>);
static_assert(std::Same<int&, std::iter_reference_t<int[]>>);
static_assert(std::Same<int&, std::iter_reference_t<int[4]>>);
static_assert(std::Same<int&, std::iter_reference_t<A>>);
static_assert(std::Same<int&, std::iter_reference_t<B>>);
static_assert(std::Same<const int&, std::iter_reference_t<const int*>>);

int main() {}
