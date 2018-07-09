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
#include <type_traits>

using std::SwappableWith;

namespace stl2
{
    struct A {
        A() = default;
        A(A&&) = delete;
        A& operator=(A&&) = delete;
        friend void swap(A&, A&) noexcept {}
    };

    struct B {
        friend void swap(B&, B&) {}
    };
}

namespace MyNS
{
    struct A {
        A(A const&) = delete;
        A& operator=(A const&) = delete;
    };

    struct B {
        B(B const&) = delete;
        B& operator=(B const&) = delete;
    };

    struct C {};
    struct D {};

    void swap(A&, A&) {}

    void swap(A&, B&) {}
    void swap(B&, A&) {}

    void swap(A&, C&) {} // missing swap(C, A)
    void swap(D&, C&) {} // missing swap(D, C)

    struct M {};

    void swap(M&&, M&&) {}

    struct AA {
        AA(AA const&) = delete;
        AA& operator=(AA const&) = delete;
        friend void swap(AA&, AA&) {}
    };

    struct BB {
        BB(BB const&) = delete;
        BB& operator=(BB const&) = delete;
        friend void swap(BB&, BB&) {}
    };

    struct CC {
        CC(AA const&);
        CC(BB const&);
    };

    void swap(AA&, BB&) {}
    void swap(BB&, AA&) {}
} // namespace MyNS

namespace std
{
    template<> struct common_type<::MyNS::AA, ::MyNS::BB> {
        using type = ::MyNS::CC;
    };
    template<> struct common_type<::MyNS::BB, ::MyNS::AA> {
        using type = ::MyNS::CC;
    };
}

template <class T, class U>
void test_swappable_with() {
    static_assert(SwappableWith<T, U>);
    static_assert(SwappableWith<U, T>);
}

template <class T, class U>
void test_not_swappable_with() {
    static_assert(!SwappableWith<T, U>);
    static_assert(!SwappableWith<U, T>);
}

int main() {
    {
        using namespace stl2;
        test_swappable_with<int&, int&>();
        test_swappable_with<int(&)[4], int(&)[4]>();
        test_not_swappable_with<int, int>();
        test_not_swappable_with<int&, double&>();
        test_not_swappable_with<int(&)[4], bool(&)[4]>();

        test_swappable_with<int(&)[3][4], int(&)[3][4]>();
        test_swappable_with<int(&)[3][4][1][2], int(&)[3][4][1][2]>();
        test_not_swappable_with<int(&)[3][4][1][2], int(&)[4][4][1][2]>();

        test_swappable_with<int(&)[2][2], int(&)[2][2]>();

        test_swappable_with<A&, A&>();
        test_swappable_with<B&, B&>();
    }

    {
        using namespace MyNS;

        // Test that is_swappable_with doesn't apply an lvalue reference
        // to the type. Instead it is up to the user.
        test_not_swappable_with<int, int>();
        test_swappable_with<int&, int&>();
        test_swappable_with<M, M>();
        test_swappable_with<A&, A&>();

        // test that heterogeneous swap is allowed only if both 'swap(A, B)' and
        // 'swap(B, A)' are valid.
        test_not_swappable_with<A&, B&>();
        test_swappable_with<AA&, BB&>();
        test_not_swappable_with<A&, C&>();
        test_not_swappable_with<D&, C&>();

        // test that cv void is guarded against as required.
        test_not_swappable_with<void, int>();
        test_not_swappable_with<int, void>();
        test_not_swappable_with<const void, const volatile void>();

        // test for presence of is_swappable_with_v
        test_swappable_with<int&, int&>();
        test_not_swappable_with<D&, C&>();
    }
}
