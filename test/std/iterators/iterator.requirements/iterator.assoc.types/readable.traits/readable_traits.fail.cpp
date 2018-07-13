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

struct S { using value_type = int; using element_type = int const; };
using T = std::readable_traits<S>::value_type; // expected-error {{ambiguous*instantiation*readable_traits<S>}}

int main() {}
