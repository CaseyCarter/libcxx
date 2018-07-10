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

using std::MoveConstructible;

struct copyable {};

struct moveonly {
    moveonly() = default;
    moveonly(moveonly&&) = default;
    moveonly& operator=(moveonly&&) = default;
};

struct copyonly { // Abomination!
    copyonly() = default;
    copyonly(const copyonly&) = default;
    copyonly& operator=(const copyonly&) = default;
    copyonly(copyonly&&) = delete;
    copyonly& operator=(copyonly&&) = delete;
};

struct nonmovable {
    nonmovable() = default;
    nonmovable(nonmovable&&) = delete;
};

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
    A(const A&);
};

struct B
{
#if TEST_STD_VER >= 11
    B(B&&);
#endif
};

template <class T>
void test_move_constructible() {
    static_assert(MoveConstructible<T>);
}

template <class T>
void test_not_move_constructible() {
    static_assert(!MoveConstructible<T>);
}

int main() {
    test_not_move_constructible<void>();
    test_move_constructible<int>();
    test_move_constructible<const int>();
    test_not_move_constructible<int[4]>();
    test_not_move_constructible<void()>();
    test_move_constructible<int &>();
    test_move_constructible<int &&>();
    test_move_constructible<const int &>();
    test_move_constructible<const int &&>();

    test_move_constructible<copyable>();
    test_move_constructible<moveonly>();
    test_not_move_constructible<copyonly>();
    test_not_move_constructible<nonmovable>();
    test_not_move_constructible<explicit_move>();
    test_move_constructible<explicit_copy>();

    test_move_constructible<nondefaultconstructible>();
    test_not_move_constructible<indestructible>();
    test_not_move_constructible<throwing_destructor>();
    test_move_constructible<explicit_default>();
    test_move_constructible<deleted_default>();

    test_move_constructible<nonmovable &>();
    test_move_constructible<nonmovable &&>();
    test_move_constructible<const nonmovable &>();
    test_move_constructible<const nonmovable &&>();

    test_not_move_constructible<char[3]>();
    test_not_move_constructible<char[]>();
    test_not_move_constructible<void>();
    test_not_move_constructible<Abstract>();

    test_move_constructible<A>();
    test_move_constructible<int&>();
    test_move_constructible<Union>();
    test_move_constructible<Empty>();
    test_move_constructible<int>();
    test_move_constructible<double>();
    test_move_constructible<int*>();
    test_move_constructible<const int*>();
    test_move_constructible<NotEmpty>();
    test_move_constructible<bit_zero>();
    test_move_constructible<B>();
}
