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
#include <cstddef>

using std::EqualityComparable;

template<unsigned> // selects one requirement to violate
struct A {};

template<unsigned Select>
bool operator==(const A<Select>&, const A<Select>&);
void operator==(const A<0>&, const A<0>&);

template<unsigned Select>
bool operator!=(const A<Select>&, const A<Select>&);
void operator!=(const A<1>&, const A<1>&);

struct B {};

template <class T>
void test_equality_comparable() {
    static_assert(EqualityComparable<T>);
    static_assert(EqualityComparable<T const>);
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>>) {
        static_assert(EqualityComparable<T&>);
        static_assert(EqualityComparable<T const&>);
        static_assert(EqualityComparable<T&&>);
        static_assert(EqualityComparable<T const&&>);
    }
}

template <class T>
void test_not_equality_comparable() {
    static_assert(!EqualityComparable<T>);
    static_assert(!EqualityComparable<T const>);
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>>) {
        static_assert(!EqualityComparable<T&>);
        static_assert(!EqualityComparable<T const&>);
        static_assert(!EqualityComparable<T&&>);
        static_assert(!EqualityComparable<T const&&>);
    }
}

int main() {
    test_equality_comparable<int>();
    test_equality_comparable<double>();
    test_equality_comparable<std::nullptr_t>();
    test_equality_comparable<int(int)>();
    test_equality_comparable<int[42]>();

    test_not_equality_comparable<void>();

    test_not_equality_comparable<A<0>>();
    test_not_equality_comparable<A<1>>();
    test_equality_comparable<A<2>>();

    test_not_equality_comparable<B>();
}
