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
#include <type_traits>

static_assert(std::Same<int, int>);
static_assert(std::Same<double, double>);
static_assert(!std::Same<double, int>);
static_assert(!std::Same<int, double>);

// Test that `Same<A,B>` subsumes `Same<B,A>` (with reversed args).
template <class A, class B>
    requires std::Same<B, A>
constexpr bool foo() {
    return false;
}

template <class A, class B>
    requires std::Same<A, B> && std::is_integral_v<A>
constexpr bool foo() {
    return true;
}

static_assert(!foo<int*, int*>());
static_assert(foo<int, int>());

template <class T, class U>
void test_same() {
    static_assert( std::Same<T, U>);
    static_assert(!std::Same<const T, U>);
    static_assert(!std::Same<T, const U>);
    static_assert( std::Same<const T, const U>);
}

template <class T, class U>
void test_same_ref() {
    static_assert(std::Same<T, U>);
    static_assert(std::Same<const T, U>);
    static_assert(std::Same<T, const U>);
    static_assert(std::Same<const T, const U>);
}

template <class T, class U>
void test_not_same() {
    static_assert(!std::Same<T, U>);
}

struct Class
{
    ~Class();
};

int main() {
    test_same<int, int>();
    test_same<void, void>();
    test_same<Class, Class>();
    test_same<int*, int*>();
    test_same_ref<int&, int&>();

    test_not_same<int, void>();
    test_not_same<void, Class>();
    test_not_same<Class, int*>();
    test_not_same<int*, int&>();
    test_not_same<int&, int>();
}
