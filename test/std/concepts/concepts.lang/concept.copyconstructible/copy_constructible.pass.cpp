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

using std::CopyConstructible;

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

class B
{
    B(const B&);
};

struct C
{
    C(C&);  // not const
    void operator=(C&);  // not const
};

template <class T>
void test_copy_constructible() {
    static_assert(CopyConstructible<T>);
    static_assert(CopyConstructible<const T>);
}

template <class T>
void test_not_copy_constructible() {
    static_assert(!CopyConstructible<T>);
    static_assert(!CopyConstructible<const T>);
    static_assert(!CopyConstructible<volatile T>);
    static_assert(!CopyConstructible<const volatile T>);
}

int main() {
    test_not_copy_constructible<void>();
    test_copy_constructible<int>();
    test_copy_constructible<const int>();
    test_copy_constructible<int&>();
    test_not_copy_constructible<int&&>();
    test_copy_constructible<const int&>();
    test_not_copy_constructible<const int&&>();
    test_not_copy_constructible<int[4]>();
    test_not_copy_constructible<void()>();

    test_copy_constructible<copyable>();
    test_not_copy_constructible<moveonly>();
    static_assert(!CopyConstructible<copyonly>);
    static_assert(CopyConstructible<const copyonly>);
    test_not_copy_constructible<nonmovable>();
    test_not_copy_constructible<explicit_move>();
    test_not_copy_constructible<explicit_copy>();
    test_copy_constructible<nonmovable &>();
    test_not_copy_constructible<nonmovable &&>();
    test_copy_constructible<const nonmovable &>();
    test_not_copy_constructible<const nonmovable &&>();

    test_copy_constructible<nondefaultconstructible>();
    test_not_copy_constructible<indestructible>();
    test_not_copy_constructible<throwing_destructor>();
    test_copy_constructible<explicit_default>();
    test_copy_constructible<deleted_default>();
    test_not_copy_constructible<explicit_move>();
    test_not_copy_constructible<explicit_copy>();

    {
        // https://github.com/ericniebler/stl2/issues/301
        struct not_mutable_ref {
            not_mutable_ref() = default;
            not_mutable_ref(const not_mutable_ref&) = default;
            not_mutable_ref(not_mutable_ref&&) = default;
            not_mutable_ref(not_mutable_ref&) = delete;
        };
        struct not_const_ref_ref {
            not_const_ref_ref() = default;
            not_const_ref_ref(const not_const_ref_ref&) = default;
            not_const_ref_ref(not_const_ref_ref&&) = default;
            not_const_ref_ref(const not_const_ref_ref&&) = delete;
        };

        static_assert(!CopyConstructible<not_mutable_ref>);
        static_assert(!CopyConstructible<not_const_ref_ref>);
    }

    test_copy_constructible<A>();
    test_copy_constructible<int&>();
    test_copy_constructible<Union>();
    test_copy_constructible<Empty>();
    test_copy_constructible<int>();
    test_copy_constructible<double>();
    test_copy_constructible<int*>();
    test_copy_constructible<const int*>();
    test_copy_constructible<NotEmpty>();
    test_copy_constructible<bit_zero>();

    test_not_copy_constructible<char[3]>();
    test_not_copy_constructible<char[]>();
    test_not_copy_constructible<void>();
    test_not_copy_constructible<Abstract>();
    test_not_copy_constructible<C>();
    test_not_copy_constructible<B>();
}
