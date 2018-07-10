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
#include <type_traits>
#include <utility>

template<unsigned> // selects one requirement to violate
struct A {};

template<unsigned Select>
bool operator==(const A<Select>&, const A<Select>&);
void operator==(const A<0>&, const A<0>&);

template<unsigned Select>
bool operator!=(const A<Select>&, const A<Select>&);
void operator!=(const A<1>&, const A<1>&);

template<unsigned Select>
bool operator<(const A<Select>&, const A<Select>&);
void operator<(const A<2>&, const A<2>&);

template<unsigned Select>
bool operator>(const A<Select>&, const A<Select>&);
void operator>(const A<3>&, const A<3>&);

template<unsigned Select>
bool operator<=(const A<Select>&, const A<Select>&);
void operator<=(const A<4>&, const A<4>&);

template<unsigned Select>
bool operator>=(const A<Select>&, const A<Select>&);
void operator>=(const A<5>&, const A<5>&);

constexpr unsigned Amax = 6; // A<Amax> violates no requirements

struct B {};

template <class T>
void test_STO() {
    using std::StrictTotallyOrdered;

    static_assert(StrictTotallyOrdered<T>);
    static_assert(StrictTotallyOrdered<T const>);
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>>) {
        static_assert(StrictTotallyOrdered<T&>);
        static_assert(StrictTotallyOrdered<T const&>);
        static_assert(StrictTotallyOrdered<T&&>);
        static_assert(StrictTotallyOrdered<T const&&>);
    }
}

template <class T>
void test_not_STO() {
    using std::StrictTotallyOrdered;

    static_assert(!StrictTotallyOrdered<T>);
    static_assert(!StrictTotallyOrdered<T const>);
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>>) {
        static_assert(!StrictTotallyOrdered<T&>);
        static_assert(!StrictTotallyOrdered<T const&>);
        static_assert(!StrictTotallyOrdered<T&&>);
        static_assert(!StrictTotallyOrdered<T const&&>);
    }
}

template<std::size_t... Is>
void test_A(std::index_sequence<Is...>) {
    static_assert(std::is_same_v<std::index_sequence<Is...>,
        std::make_index_sequence<Amax>>);
    (test_not_STO<A<Is>>(), ...);
    test_STO<A<Amax>>();
}

int main() {
    test_STO<int>();
    test_STO<double>();
    test_STO<std::nullptr_t>();
    test_STO<int(int)>();
    test_STO<int[42]>();

    test_not_STO<void>();

    test_A(std::make_index_sequence<Amax>{});

    test_not_STO<B>();
}
