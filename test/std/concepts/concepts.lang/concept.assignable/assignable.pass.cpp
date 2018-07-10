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

using std::Assignable;

struct A
{
};

struct B
{
    void operator=(A);
};

struct D;

struct C
{
    template <class U>
    D operator,(U&&);
};

struct E
{
    C operator=(int);
};

struct F
{
    F& operator=(A const&);
};

struct G
{
    G(A const&);
    G& operator=(A const&) &;
};

template <typename T>
struct X { T t; };

template <class T, class U>
void test_assignable() {
    static_assert(Assignable<T, U>);
}

template <class T, class U>
void test_not_assignable() {
    static_assert(!Assignable<T, U>);
}

int main() {
    test_not_assignable<int, int>();
    test_not_assignable<int, int&>();
    test_not_assignable<int, int&&>();
    test_not_assignable<int, int const>();
    test_not_assignable<int, int const&>();
    test_not_assignable<int, int const&&>();

    test_assignable<int&, int>();
    test_assignable<int&, int&>();
    test_assignable<int&, int&&>();
    test_assignable<int&, int const>();
    test_assignable<int&, int const&>();
    test_assignable<int&, int const&&>();

    test_not_assignable<int&&, int>();
    test_not_assignable<int&&, int&>();
    test_not_assignable<int&&, int&&>();
    test_not_assignable<int&&, int const>();
    test_not_assignable<int&&, int const&>();
    test_not_assignable<int&&, int const&&>();

    test_assignable<int&, double>();
    test_assignable<void*&, void*>();

    test_not_assignable<A, A>();
    test_not_assignable<A, A&>();
    test_not_assignable<A, A&&>();
    test_not_assignable<A, A const>();
    test_not_assignable<A, A const&>();
    test_not_assignable<A, A const&&>();

    test_assignable<A&, A>();
    test_assignable<A&, A&>();
    test_assignable<A&, A&&>();
    test_assignable<A&, A const>();
    test_assignable<A&, A const&>();
    test_assignable<A&, A const&&>();

    test_not_assignable<A&&, A>();
    test_not_assignable<A&&, A&>();
    test_not_assignable<A&&, A&&>();
    test_not_assignable<A&&, A const>();
    test_not_assignable<A&&, A const&>();
    test_not_assignable<A&&, A const&&>();

    // assignment operator exists, but no CommonReference
    test_not_assignable<F, A>();
    test_not_assignable<F, A&>();
    test_not_assignable<F, A&&>();
    test_not_assignable<F, A const>();
    test_not_assignable<F, A const&>();
    test_not_assignable<F, A const&&>();

    test_not_assignable<F&, A>();
    test_not_assignable<F&, A&>();
    test_not_assignable<F&, A&&>();
    test_not_assignable<F&, A const>();
    test_not_assignable<F&, A const&>();
    test_not_assignable<F&, A const&&>();

    test_not_assignable<F&&, A>();
    test_not_assignable<F&&, A&>();
    test_not_assignable<F&&, A&&>();
    test_not_assignable<F&&, A const>();
    test_not_assignable<F&&, A const&>();
    test_not_assignable<F&&, A const&&>();

    // assignment operator exists, with CommonReference
    test_not_assignable<G, A>();
    test_not_assignable<G, A&>();
    test_not_assignable<G, A&&>();
    test_not_assignable<G, A const>();
    test_not_assignable<G, A const&>();
    test_not_assignable<G, A const&&>();

    test_assignable<G&, A>();
    test_assignable<G&, A&>();
    test_assignable<G&, A&&>();
    test_assignable<G&, A const>();
    test_assignable<G&, A const&>();
    test_assignable<G&, A const&&>();

    test_not_assignable<G&&, A>();
    test_not_assignable<G&&, A&>();
    test_not_assignable<G&&, A&&>();
    test_not_assignable<G&&, A const>();
    test_not_assignable<G&&, A const&>();
    test_not_assignable<G&&, A const&&>();

    test_not_assignable<B, A>();
    test_not_assignable<E, int>();

    test_not_assignable<A, B>();
    test_not_assignable<void, const void>();
    test_not_assignable<const void, const void>();
    test_not_assignable<int(), int>();

    // pointer to incomplete template type
    test_assignable<X<D>*&, X<D>*>();
}
