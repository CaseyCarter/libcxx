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

#include <concepts>

template <class Derived, class Base>
void test_derived_from() {
    static_assert(std::DerivedFrom<Derived, Base>);
    static_assert(std::DerivedFrom<const Derived, Base>);
    static_assert(std::DerivedFrom<Derived, const Base>);
    static_assert(std::DerivedFrom<const Derived, const Base>);
}

template <class Derived, class Base>
void test_not_derived_from() {
    static_assert(!std::DerivedFrom<Derived, Base>);
}

struct B {};
struct B1 : B {};
struct B2 : B {};
struct D : private B1, private B2 {};

int main() {
    test_derived_from<B1, B>();
    test_derived_from<B2, B>();
    test_derived_from<B, B>();

    test_not_derived_from<D, B>();
    test_not_derived_from<D, B1>();
    test_not_derived_from<D, B2>();
    test_not_derived_from<B, D>();
    test_not_derived_from<D&, B&>();
    test_not_derived_from<D[3], B[3]>();
    test_not_derived_from<int, int>();
}
