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

using std::Swappable;

struct A {
    friend void swap(A&, A&) {}
};

struct B {
    B() = default;
    B(B&&) = delete;
    B& operator=(B&&) = delete;
    friend void swap(B&, B&) noexcept {}
};

namespace MyNS {
    // Make the test types non-copyable so that generic std::swap is not valid.
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
    void swap(D&, C&) {}

    struct M {
    M(M const&) = delete;
    M& operator=(M const&) = delete;
    };

    void swap(M&&, M&&) {}

    struct DeletedSwap {
    friend void swap(DeletedSwap&, DeletedSwap&) = delete;
    };
} // namespace MyNS

namespace MyNS2 {
    struct AmbiguousSwap {};

    template <class T>
    void swap(T&, T&) {}
} // end namespace MyNS2

template <class T>
void test_swappable() {
    static_assert(Swappable<T>);
}

template <class T>
void test_not_swappable() {
    static_assert(!Swappable<T>);
}

int main() {
    test_swappable<int>();
    test_swappable<int[4]>();
    test_not_swappable<int[]>();
    test_not_swappable<int[][4]>();
    test_swappable<int[3][4]>();
    test_swappable<int[3][4][1][2]>();

    test_swappable<A>();
    test_swappable<B>();

    {
        // Test that is_swappable applies an lvalue reference to the type.
        test_swappable<MyNS::A>();
        test_swappable<MyNS::A&>();
        test_not_swappable<MyNS::M>();
        test_not_swappable<MyNS::M&&>();
    }
    test_not_swappable<MyNS::B>();
    test_swappable<MyNS::C>();
    {
        // test non-referencable types
        test_not_swappable<void>();
        test_not_swappable<int() const>();
        test_not_swappable<int() &>();
    }
    {
        // test that a deleted swap is correctly handled.
        test_not_swappable<MyNS::DeletedSwap>();
    }
    {
        // test that a swap with ambiguous overloads is handled correctly.
        test_not_swappable<MyNS2::AmbiguousSwap>();
    }
}
