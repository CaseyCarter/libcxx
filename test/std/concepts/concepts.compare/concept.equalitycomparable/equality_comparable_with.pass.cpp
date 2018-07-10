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

struct A {
    friend bool operator==(const A&, const A&);
    friend bool operator!=(const A&, const A&);
};

struct B {
    friend bool operator==(const B&, const B&);
    friend bool operator!=(const B&, const B&);
};

bool operator==(const A&, const B&);
bool operator==(const B&, const A&);
bool operator!=(const A&, const B&);
bool operator!=(const B&, const A&);

struct C {
    friend bool operator==(const C&, const C&);
    friend bool operator!=(const C&, const C&);
};

struct D {
    friend bool operator==(const D&, const D&);
    friend bool operator!=(const D&, const D&);
};

bool operator==(const C&, const D&);
bool operator==(const D&, const C&);
bool operator!=(const C&, const D&);
bool operator!=(const D&, const C&);

struct E {
    E(C);
    E(D);
    friend bool operator==(const E&, const E&);
    friend bool operator!=(const E&, const E&);
};

namespace std
{
    template<>
    struct common_type<::C, ::D> { using type = ::E; };
    template<>
    struct common_type<::D, ::C> { using type = ::E; };
}

template <class T, class U = T>
void test_equality_comparable_with() {
    using std::EqualityComparableWith;

    static_assert(EqualityComparableWith<T, U>);
    static_assert(EqualityComparableWith<U, T>);
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>>) {
        static_assert(EqualityComparableWith<T&, U>);
        static_assert(EqualityComparableWith<T const&, U>);
        static_assert(EqualityComparableWith<T&&, U>);
        static_assert(EqualityComparableWith<T const&&, U>);
    }
    if constexpr (!std::is_same_v<U, std::add_lvalue_reference_t<U>>) {
        static_assert(EqualityComparableWith<T, U&>);
        static_assert(EqualityComparableWith<T, U const&>);
        static_assert(EqualityComparableWith<T, U&&>);
        static_assert(EqualityComparableWith<T, U const&&>);
    }
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>> &&
                  !std::is_same_v<U, std::add_lvalue_reference_t<U>>) {
        static_assert(EqualityComparableWith<T&, U&>);
        static_assert(EqualityComparableWith<T const&, U&>);
        static_assert(EqualityComparableWith<T&&, U&>);
        static_assert(EqualityComparableWith<T const&&, U&>);

        static_assert(EqualityComparableWith<T&, U const&>);
        static_assert(EqualityComparableWith<T const&, U const&>);
        static_assert(EqualityComparableWith<T&&, U const&>);
        static_assert(EqualityComparableWith<T const&&, U const&>);

        static_assert(EqualityComparableWith<T&, U&&>);
        static_assert(EqualityComparableWith<T const&, U&&>);
        static_assert(EqualityComparableWith<T&&, U&&>);
        static_assert(EqualityComparableWith<T const&&, U&&>);

        static_assert(EqualityComparableWith<T&, U const&&>);
        static_assert(EqualityComparableWith<T const&, U const&&>);
        static_assert(EqualityComparableWith<T&&, U const&&>);
        static_assert(EqualityComparableWith<T const&&, U const&&>);
    }
}

template <class T, class U = T>
void test_not_equality_comparable_with() {
    using std::EqualityComparableWith;

    static_assert(!EqualityComparableWith<T, U>);
    static_assert(!EqualityComparableWith<U, T>);
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>>) {
        static_assert(!EqualityComparableWith<T&, U>);
        static_assert(!EqualityComparableWith<T const&, U>);
        static_assert(!EqualityComparableWith<T&&, U>);
        static_assert(!EqualityComparableWith<T const&&, U>);
    }
    if constexpr (!std::is_same_v<U, std::add_lvalue_reference_t<U>>) {
        static_assert(!EqualityComparableWith<T, U&>);
        static_assert(!EqualityComparableWith<T, U const&>);
        static_assert(!EqualityComparableWith<T, U&&>);
        static_assert(!EqualityComparableWith<T, U const&&>);
    }
    if constexpr (!std::is_same_v<T, std::add_lvalue_reference_t<T>> &&
                  !std::is_same_v<U, std::add_lvalue_reference_t<U>>) {
        static_assert(!EqualityComparableWith<T&, U&>);
        static_assert(!EqualityComparableWith<T const&, U&>);
        static_assert(!EqualityComparableWith<T&&, U&>);
        static_assert(!EqualityComparableWith<T const&&, U&>);

        static_assert(!EqualityComparableWith<T&, U const&>);
        static_assert(!EqualityComparableWith<T const&, U const&>);
        static_assert(!EqualityComparableWith<T&&, U const&>);
        static_assert(!EqualityComparableWith<T const&&, U const&>);

        static_assert(!EqualityComparableWith<T&, U&&>);
        static_assert(!EqualityComparableWith<T const&, U&&>);
        static_assert(!EqualityComparableWith<T&&, U&&>);
        static_assert(!EqualityComparableWith<T const&&, U&&>);

        static_assert(!EqualityComparableWith<T&, U const&&>);
        static_assert(!EqualityComparableWith<T const&, U const&&>);
        static_assert(!EqualityComparableWith<T&&, U const&&>);
        static_assert(!EqualityComparableWith<T const&&, U const&&>);
    }
}

int main() {
    test_equality_comparable_with<int>();
    test_equality_comparable_with<double>();
    test_equality_comparable_with<int, double>();
    test_equality_comparable_with<std::nullptr_t>();
    test_equality_comparable_with<int(int)>();
    test_equality_comparable_with<int[42]>();

    test_equality_comparable_with<int*, void*>();

    test_not_equality_comparable_with<void>();
    test_not_equality_comparable_with<int, void>();

    test_equality_comparable_with<A, A>();
    test_equality_comparable_with<B, B>();
    test_equality_comparable_with<C, C>();
    test_equality_comparable_with<D, D>();
    test_equality_comparable_with<E, E>();

    // A and B have all the necessary operators, but fail CommonReference.
    test_not_equality_comparable_with<A, B>();

    // C and D have all the necessary operators, and model CommonReference,
    // since their common type is E.
    test_equality_comparable_with<C, D>();

    // E is also the common type of (C, E) and (D, E):
    test_equality_comparable_with<C, E>();
    test_equality_comparable_with<D, E>();
}
