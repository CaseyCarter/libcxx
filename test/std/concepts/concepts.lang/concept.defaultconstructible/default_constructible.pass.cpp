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
#include <initializer_list>

using std::DefaultConstructible;
using std::initializer_list;

struct nondefaultconstructible {
    nondefaultconstructible(int) {}
};

struct indestructible {
    ~indestructible() = delete;
};

struct throwing_destructor {
    ~throwing_destructor() noexcept(false);
};

struct explicit_default {
    explicit explicit_default() {}
};

struct deleted_default {
    deleted_default() = delete;
};

struct explicit_move {
    explicit_move() = default;
    explicit explicit_move(explicit_move&&) = default;
};

struct explicit_copy {
    explicit_copy() = default;
    explicit_copy(explicit_copy&&) = default;
    explicit explicit_copy(const explicit_copy&) = default;
};

class Empty
{
};

class NoDefaultConstructor
{
    NoDefaultConstructor(int) {}
};

class NotEmpty
{
public:
    virtual ~NotEmpty();
};

union Union {};

struct bit_zero
{
    int :  0;
};

class Abstract
{
public:
    virtual ~Abstract() = 0;
};

struct A
{
    A();
};

class B
{
    B();
};

template <class T>
void test_default_constructible() {
    static_assert(DefaultConstructible<T>);
    static_assert(DefaultConstructible<const T>);
    static_assert(DefaultConstructible<volatile T>);
    static_assert(DefaultConstructible<const volatile T>);
}

template <class T>
void test_not_default_constructible() {
    static_assert(!DefaultConstructible<T>);
    static_assert(!DefaultConstructible<const T>);
    static_assert(!DefaultConstructible<volatile T>);
    static_assert(!DefaultConstructible<const volatile T>);
}

int main() {
    test_default_constructible<int>();
    test_default_constructible<int const>();
    test_not_default_constructible<int&>();
    test_not_default_constructible<int const&>();
    test_not_default_constructible<int()>();
    test_not_default_constructible<int(&)()>();
    test_default_constructible<double>();
    test_not_default_constructible<void>();
    test_not_default_constructible<int[]>();
    test_default_constructible<int[2]>();
    test_default_constructible<int*>();

    test_default_constructible<explicit_default>();
    test_default_constructible<explicit_move>();
    test_default_constructible<explicit_copy>();

    test_not_default_constructible<nondefaultconstructible>();
    test_not_default_constructible<deleted_default>();

    test_default_constructible<std::initializer_list<int>>();

    test_default_constructible<A>();
    test_default_constructible<Union>();
    test_default_constructible<Empty>();
    test_default_constructible<int>();
    test_default_constructible<double>();
    test_default_constructible<int*>();
    test_default_constructible<const int*>();
    test_default_constructible<char[3]>();
    test_default_constructible<char[5][3]>();

    test_default_constructible<NotEmpty>();
    test_default_constructible<bit_zero>();

    test_not_default_constructible<void>();
    test_not_default_constructible<int&>();
    test_not_default_constructible<char[]>();
    test_not_default_constructible<char[][3]>();

    test_not_default_constructible<Abstract>();
    test_not_default_constructible<NoDefaultConstructor>();
    test_not_default_constructible<B>();
    test_not_default_constructible<int&&>();

    test_not_default_constructible<void()>();
    test_not_default_constructible<void() const> ();
    test_not_default_constructible<void() volatile> ();
    test_not_default_constructible<void() &> ();
    test_not_default_constructible<void() &&> ();
}
