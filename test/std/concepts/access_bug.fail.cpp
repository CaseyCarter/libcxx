//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// REQUIRES: concepts
// XFAIL: gcc

// Bug test for  https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67225.

#include <__config>

template <class, class>
_LIBCPP_CONCEPT_DECL C1 = true;

template <class>
_LIBCPP_CONCEPT_DECL C2 = requires { { 42 } -> C1<int>; };

int main() {
    class A { int x; } a;
    a.x = 42;
}
