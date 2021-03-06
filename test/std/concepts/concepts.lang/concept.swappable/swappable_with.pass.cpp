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

#include <cassert>
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

namespace example {
    template<class T, std::SwappableWith<T> U>
    void value_swap(T&& t, U&& u) {
        std::ranges::swap(std::forward<T>(t), std::forward<U>(u));
    }
    template<std::Swappable T>
    void lv_swap(T& t1, T& t2) {
        std::ranges::swap(t1, t2);
    }
     namespace N {
        struct A { int m; };
        struct Proxy {
            A* a;
            Proxy(A& a) : a{&a} {}
            friend void swap(Proxy&& x, Proxy&& y) {
                std::ranges::swap(*x.a, *y.a);
            }
        };
        Proxy proxy(A& a) { return Proxy(a); }
     }

     void test() {
        int i = 1, j = 2;
        lv_swap(i, j);
        assert(i == 2);
        assert(j == 1);
         N::A a1 = { 5 }, a2 = { -5 };
        value_swap(a1, proxy(a2));
        assert(a1.m == -5);
        assert(a2.m == 5);

        // Additional checks for paths not exercised by the example
        value_swap(proxy(a1), a2);
        assert(a1.m == 5);
        assert(a2.m == -5);

        value_swap(proxy(a1), proxy(a2));
        assert(a1.m == -5);
        assert(a2.m == 5);

        N::Proxy p1{a1}, p2{a2};
        std::ranges::swap(p1, p2);
        assert(a1.m == -5);
        assert(a2.m == 5);
        assert(p1.a == &a2);
        assert(p2.a == &a1);
    }
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

    example::test();
}
