// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17
// XFAIL: gcc

// GCC hard errors when an expression evaluated in a concept is not a constant
// expression. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=86493

#include <concepts>

template <class T>
_LIBCPP_CONCEPT_DECL Concept = T::f() == 0;

struct bad {
    static int f() { return 0; }
};

int main() {
    static_assert(!Concept<bad>);
}
