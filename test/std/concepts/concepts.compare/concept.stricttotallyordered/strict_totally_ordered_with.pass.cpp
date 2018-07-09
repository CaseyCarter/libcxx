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
// REQUIRES: concepts

#include <concepts>
#include <cstddef>
#include <type_traits>

struct A {
    friend bool operator==(const A&, const A&);
    friend bool operator!=(const A&, const A&);
    friend bool operator< (const A&, const A&);
    friend bool operator> (const A&, const A&);
    friend bool operator<=(const A&, const A&);
    friend bool operator>=(const A&, const A&);
};

struct B {
    friend bool operator==(const B&, const B&);
    friend bool operator!=(const B&, const B&);
    friend bool operator< (const B&, const B&);
    friend bool operator> (const B&, const B&);
    friend bool operator<=(const B&, const B&);
    friend bool operator>=(const B&, const B&);
};

bool operator==(const A&, const B&);
bool operator==(const B&, const A&);
bool operator!=(const A&, const B&);
bool operator!=(const B&, const A&);
bool operator< (const A&, const B&);
bool operator< (const B&, const A&);
bool operator> (const A&, const B&);
bool operator> (const B&, const A&);
bool operator<=(const A&, const B&);
bool operator<=(const B&, const A&);
bool operator>=(const A&, const B&);
bool operator>=(const B&, const A&);

struct C {
    friend bool operator==(const C&, const C&);
    friend bool operator!=(const C&, const C&);
    friend bool operator< (const C&, const C&);
    friend bool operator> (const C&, const C&);
    friend bool operator<=(const C&, const C&);
    friend bool operator>=(const C&, const C&);
};

struct D {
    friend bool operator==(const D&, const D&);
    friend bool operator!=(const D&, const D&);
    friend bool operator< (const D&, const D&);
    friend bool operator> (const D&, const D&);
    friend bool operator<=(const D&, const D&);
    friend bool operator>=(const D&, const D&);
};

bool operator==(const C&, const D&);
bool operator==(const D&, const C&);
bool operator!=(const C&, const D&);
bool operator!=(const D&, const C&);
bool operator< (const C&, const D&);
bool operator< (const D&, const C&);
bool operator> (const C&, const D&);
bool operator> (const D&, const C&);
bool operator<=(const C&, const D&);
bool operator<=(const D&, const C&);
bool operator>=(const C&, const D&);
bool operator>=(const D&, const C&);

struct E {
    E(C);
    E(D);
    friend bool operator==(const E&, const E&);
    friend bool operator!=(const E&, const E&);
    friend bool operator< (const E&, const E&);
    friend bool operator> (const E&, const E&);
    friend bool operator<=(const E&, const E&);
    friend bool operator>=(const E&, const E&);
};

namespace std
{
    template<>
    struct common_type<::C, ::D> { using type = ::E; };
    template<>
    struct common_type<::D, ::C> { using type = ::E; };
}

template <class T, class U = T>
void test_STOW() {
    using std::StrictTotallyOrderedWith;

    static_assert(StrictTotallyOrderedWith<T, U>);
    static_assert(StrictTotallyOrderedWith<U, T>);
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>>) {
        static_assert(StrictTotallyOrderedWith<T&, U>);
        static_assert(StrictTotallyOrderedWith<T const&, U>);
        static_assert(StrictTotallyOrderedWith<T&&, U>);
        static_assert(StrictTotallyOrderedWith<T const&&, U>);
    }
    if constexpr (!std::is_same_v<U, std::add_lvalue_reference_t<U>>) {
        static_assert(StrictTotallyOrderedWith<T, U&>);
        static_assert(StrictTotallyOrderedWith<T, U const&>);
        static_assert(StrictTotallyOrderedWith<T, U&&>);
        static_assert(StrictTotallyOrderedWith<T, U const&&>);
    }
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>> &&
                  !std::is_same_v<U, std::add_lvalue_reference_t<U>>) {
        static_assert(StrictTotallyOrderedWith<T&, U&>);
        static_assert(StrictTotallyOrderedWith<T const&, U&>);
        static_assert(StrictTotallyOrderedWith<T&&, U&>);
        static_assert(StrictTotallyOrderedWith<T const&&, U&>);

        static_assert(StrictTotallyOrderedWith<T&, U const&>);
        static_assert(StrictTotallyOrderedWith<T const&, U const&>);
        static_assert(StrictTotallyOrderedWith<T&&, U const&>);
        static_assert(StrictTotallyOrderedWith<T const&&, U const&>);

        static_assert(StrictTotallyOrderedWith<T&, U&&>);
        static_assert(StrictTotallyOrderedWith<T const&, U&&>);
        static_assert(StrictTotallyOrderedWith<T&&, U&&>);
        static_assert(StrictTotallyOrderedWith<T const&&, U&&>);

        static_assert(StrictTotallyOrderedWith<T&, U const&&>);
        static_assert(StrictTotallyOrderedWith<T const&, U const&&>);
        static_assert(StrictTotallyOrderedWith<T&&, U const&&>);
        static_assert(StrictTotallyOrderedWith<T const&&, U const&&>);
    }
}

template <class T, class U = T>
void test_not_STOW() {
    using std::StrictTotallyOrderedWith;

    static_assert(!StrictTotallyOrderedWith<T, U>);
    static_assert(!StrictTotallyOrderedWith<U, T>);
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>>) {
        static_assert(!StrictTotallyOrderedWith<T&, U>);
        static_assert(!StrictTotallyOrderedWith<T const&, U>);
        static_assert(!StrictTotallyOrderedWith<T&&, U>);
        static_assert(!StrictTotallyOrderedWith<T const&&, U>);
    }
    if constexpr (!std::is_same_v<U, std::add_lvalue_reference_t<U>>) {
        static_assert(!StrictTotallyOrderedWith<T, U&>);
        static_assert(!StrictTotallyOrderedWith<T, U const&>);
        static_assert(!StrictTotallyOrderedWith<T, U&&>);
        static_assert(!StrictTotallyOrderedWith<T, U const&&>);
    }
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>> &&
                  !std::is_same_v<U, std::add_lvalue_reference_t<U>>) {
        static_assert(!StrictTotallyOrderedWith<T&, U&>);
        static_assert(!StrictTotallyOrderedWith<T const&, U&>);
        static_assert(!StrictTotallyOrderedWith<T&&, U&>);
        static_assert(!StrictTotallyOrderedWith<T const&&, U&>);

        static_assert(!StrictTotallyOrderedWith<T&, U const&>);
        static_assert(!StrictTotallyOrderedWith<T const&, U const&>);
        static_assert(!StrictTotallyOrderedWith<T&&, U const&>);
        static_assert(!StrictTotallyOrderedWith<T const&&, U const&>);

        static_assert(!StrictTotallyOrderedWith<T&, U&&>);
        static_assert(!StrictTotallyOrderedWith<T const&, U&&>);
        static_assert(!StrictTotallyOrderedWith<T&&, U&&>);
        static_assert(!StrictTotallyOrderedWith<T const&&, U&&>);

        static_assert(!StrictTotallyOrderedWith<T&, U const&&>);
        static_assert(!StrictTotallyOrderedWith<T const&, U const&&>);
        static_assert(!StrictTotallyOrderedWith<T&&, U const&&>);
        static_assert(!StrictTotallyOrderedWith<T const&&, U const&&>);
    }
}

int main() {
    test_STOW<int>();
    test_STOW<double>();
    test_STOW<int, double>();
    test_STOW<std::nullptr_t>();
    test_STOW<int(int)>();
    test_STOW<int[42]>();

    test_STOW<int*, void*>();

    test_not_STOW<void>();
    test_not_STOW<int, void>();

    test_STOW<A, A>();
    test_STOW<B, B>();
    test_STOW<C, C>();
    test_STOW<D, D>();
    test_STOW<E, E>();

    // A and B have all the necessary operators, but fail CommonReference.
    test_not_STOW<A, B>();

    // C and D have all the necessary operators, and model CommonReference,
    // since their common type is E.
    test_STOW<C, D>();

    // E is also the common type of (C, E) and (D, E):
    test_STOW<C, E>();
    test_STOW<D, E>();
}
