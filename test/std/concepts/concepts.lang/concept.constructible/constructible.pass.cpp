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
#include <initializer_list>

using std::Constructible;
using std::initializer_list;

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

struct XXX
{
    XXX() = default;
    XXX(XXX&&) = delete;
    explicit XXX(int) {}
};

struct A
{
    explicit A(int);
    A(int, double);
    A(int, long, double);
private:
    A(char);
};

struct Base {};
struct Derived : public Base {};

class Abstract
{
    virtual void foo() = 0;
};

class AbstractDestructor
{
    virtual ~AbstractDestructor() = 0;
};

struct PrivateDtor {
  PrivateDtor(int) {}
private:
  ~PrivateDtor() {}
};

struct S {
   template <class T>
   explicit operator T () const;
};

template <class To>
struct ImplicitTo {
  operator To();
};

template <class To>
struct ExplicitTo {
   explicit operator To ();
};

template <class T, class... Args>
void test_constructible() {
    static_assert(Constructible<T, Args...>);
    static_assert(Constructible<const T, Args...>);
    static_assert(Constructible<volatile T, Args...>);
    static_assert(Constructible<const volatile T, Args...>);
}

template <class T, class... Args>
void test_not_constructible() {
    static_assert(!Constructible<T, Args...>);
    static_assert(!Constructible<const T, Args...>);
    static_assert(!Constructible<volatile T, Args...>);
    static_assert(!Constructible<const volatile T, Args...>);
}

int main() {
    test_constructible<int>();
    test_not_constructible<int const&>();
    test_not_constructible<int()>();
    test_not_constructible<int(&)()>();
    test_not_constructible<int[]>();
    test_constructible<int[5]>();
    test_constructible<int const(&)[5], int(&)[5]>();
    test_not_constructible<int, int(&)[3]>();

    test_constructible<int, int>();
    test_constructible<int, int&>();
    test_constructible<int, int&&>();
    test_constructible<int, const int>();
    test_constructible<int, const int&>();
    test_constructible<int, const int&&>();

    test_constructible<int*>();
    test_constructible<int*, int*>();
    test_constructible<int*, int* const>();
    test_constructible<int*, int*&>();
    test_constructible<int*, int* const&>();
    test_constructible<int*, int*&&>();
    test_constructible<int*, int* const&&>();

    test_not_constructible<int&, int>();
    test_constructible<int&, int&>();
    test_not_constructible<int&, int&&>();
    test_not_constructible<int&, const int>();
    test_not_constructible<int&, const int&>();
    test_not_constructible<int&, const int&&>();

    test_constructible<const int&, int>();
    test_constructible<const int&, int&>();
    test_constructible<const int&, int&&>();
    test_constructible<const int&, const int>();
    test_constructible<const int&, const int&>();
    test_constructible<const int&, const int&&>();

    test_constructible<int&&, int>();
    test_not_constructible<int&&, int&>();
    test_constructible<int&&, int&&>();
    test_not_constructible<int&&, const int>();
    test_not_constructible<int&&, const int&>();
    test_not_constructible<int&&, const int&&>();

    test_constructible<const int&&, int>();
    test_not_constructible<const int&&, int&>();
    test_constructible<const int&&, int&&>();
    test_constructible<const int&&, const int>();
    test_not_constructible<const int&&, const int&>();
    test_constructible<const int&&, const int&&>();

    test_constructible<copyable>();
    test_constructible<copyable, copyable>();
    test_constructible<copyable, copyable&>();
    test_constructible<copyable, copyable&&>();
    test_constructible<copyable, const copyable>();
    test_constructible<copyable, const copyable&>();
    test_constructible<copyable, const copyable&&>();

    test_constructible<moveonly>();
    test_constructible<moveonly, moveonly>();
    test_not_constructible<moveonly, moveonly&>();
    test_constructible<moveonly, moveonly&&>();
    test_not_constructible<moveonly, const moveonly>();
    test_not_constructible<moveonly, const moveonly&>();
    test_not_constructible<moveonly, const moveonly&&>();

    test_constructible<copyonly>();
    test_not_constructible<copyonly, copyonly>();
    test_constructible<copyonly, copyonly&>();
    test_not_constructible<copyonly, copyonly&&>();
    test_constructible<copyonly, const copyonly>();
    test_constructible<copyonly, const copyonly&>();
    test_constructible<copyonly, const copyonly&&>();

    test_constructible<nonmovable>();
    test_not_constructible<nonmovable, nonmovable>();
    test_not_constructible<nonmovable, nonmovable&>();
    test_not_constructible<nonmovable, nonmovable&&>();
    test_not_constructible<nonmovable, const nonmovable>();
    test_not_constructible<nonmovable, const nonmovable&>();
    test_not_constructible<nonmovable, const nonmovable&&>();

    test_not_constructible<nondefaultconstructible>();
    test_constructible<nondefaultconstructible, nondefaultconstructible>();
    test_constructible<nondefaultconstructible, nondefaultconstructible&>();
    test_constructible<nondefaultconstructible, nondefaultconstructible&&>();
    test_constructible<nondefaultconstructible, const nondefaultconstructible>();
    test_constructible<nondefaultconstructible, const nondefaultconstructible&>();
    test_constructible<nondefaultconstructible, const nondefaultconstructible&&>();

    test_not_constructible<indestructible>();
    test_not_constructible<indestructible, indestructible>();
    test_not_constructible<indestructible, indestructible&>();
    test_not_constructible<indestructible, indestructible&&>();
    test_not_constructible<indestructible, const indestructible>();
    test_not_constructible<indestructible, const indestructible&>();
    test_not_constructible<indestructible, const indestructible&&>();

    test_not_constructible<throwing_destructor>();
    test_not_constructible<throwing_destructor, throwing_destructor>();
    test_not_constructible<throwing_destructor, throwing_destructor&>();
    test_not_constructible<throwing_destructor, throwing_destructor&&>();
    test_not_constructible<throwing_destructor, const throwing_destructor>();
    test_not_constructible<throwing_destructor, const throwing_destructor&>();
    test_not_constructible<throwing_destructor, const throwing_destructor&&>();

    test_constructible<explicit_default>();
    test_constructible<explicit_default, explicit_default>();
    test_constructible<explicit_default, explicit_default&>();
    test_constructible<explicit_default, explicit_default&&>();
    test_constructible<explicit_default, const explicit_default>();
    test_constructible<explicit_default, const explicit_default&>();
    test_constructible<explicit_default, const explicit_default&&>();

    test_not_constructible<deleted_default>();
    test_constructible<deleted_default, deleted_default>();
    test_constructible<deleted_default, deleted_default&>();
    test_constructible<deleted_default, deleted_default&&>();
    test_constructible<deleted_default, const deleted_default>();
    test_constructible<deleted_default, const deleted_default&>();
    test_constructible<deleted_default, const deleted_default&&>();

    test_constructible<XXX>();
    test_constructible<XXX, int>();
    test_not_constructible<XXX, XXX>();
    test_not_constructible<XXX, XXX&>();
    test_not_constructible<XXX, XXX&&>();
    test_not_constructible<XXX, const XXX>();
    test_not_constructible<XXX, const XXX&>();
    test_not_constructible<XXX, const XXX&&>();

    test_constructible<initializer_list<int>>();

    // https://github.com/ericniebler/stl2/issues/301
    test_not_constructible<int&, long&>();

    using B = Base;
    using D = Derived;

    test_constructible<int>();
    test_constructible<int, const int>();
    test_constructible<A, int>();
    test_constructible<A, int, double>();
    test_constructible<A, int, long, double>();
    test_constructible<int&, int&>();

    test_not_constructible<A>();
    test_not_constructible<A, char>();
    test_not_constructible<A, void>();
    test_not_constructible<int, void()>();
    test_not_constructible<int, void(&)()>();
    test_not_constructible<int, void() const>();
    test_not_constructible<int&, void>();
    test_not_constructible<int&, void()>();
    test_not_constructible<int&, void() const>();
    test_not_constructible<int&, void(&)()>();

    test_not_constructible<void>();
    test_not_constructible<const void>();  // LWG 2738
    test_not_constructible<volatile void>();
    test_not_constructible<const volatile void>();
    test_not_constructible<int&>();
    test_not_constructible<Abstract>();
    test_not_constructible<AbstractDestructor>();
    test_constructible<int, S>();
    test_not_constructible<int&, S>();

    test_constructible<void(&)(), void(&)()>();
    test_constructible<void(&)(), void()>();
    test_constructible<void(&&)(), void(&&)()>();
    test_constructible<void(&&)(), void()>();
    test_constructible<void(&&)(), void(&)()>();

    test_constructible<int const&, int>();
    test_constructible<int const&, int&&>();

    test_constructible<int&&, double&>();
    test_constructible<void(&)(), void(&&)()>();

    test_not_constructible<int&, int>();
    test_not_constructible<int&, int const&>();
    test_not_constructible<int&, int&&>();

    test_constructible<int&&, int>();
    test_constructible<int&&, int&&>();
    test_not_constructible<int&&, int&>();
    test_not_constructible<int&&, int const&&>();

    test_constructible<Base, Derived>();
    test_constructible<Base&, Derived&>();
    test_not_constructible<Derived&, Base&>();
    test_constructible<Base const&, Derived const&>();
    test_not_constructible<Derived const&, Base const&>();
    test_not_constructible<Derived const&, Base>();

    test_constructible<Base&&, Derived>();
    test_constructible<Base&&, Derived&&>();
    test_not_constructible<Derived&&, Base&&>();
    test_not_constructible<Derived&&, Base>();

    // test that T must also be destructible
    test_constructible<PrivateDtor&, PrivateDtor&>();
    test_not_constructible<PrivateDtor, int>();

    test_not_constructible<void() const, void() const>();
    test_not_constructible<void() const, void*>();

    test_constructible<int&, ImplicitTo<int&>>();
    test_constructible<const int&, ImplicitTo<int&&>>();
    test_constructible<int&&, ImplicitTo<int&&>>();
    test_constructible<const int&, ImplicitTo<int>>();

    test_not_constructible<B&&, B&>();
    test_not_constructible<B&&, D&>();
    test_constructible<B&&, ImplicitTo<D&&>>();
    test_constructible<B&&, ImplicitTo<D&&>&>();
    test_constructible<int&&, double&>();
    test_constructible<const int&, ImplicitTo<int&>&>();
    test_constructible<const int&, ImplicitTo<int&>>();
    test_constructible<const int&, ExplicitTo<int&>&>();
    test_constructible<const int&, ExplicitTo<int&>>();

    test_constructible<const int&, ExplicitTo<int&>&>();
    test_constructible<const int&, ExplicitTo<int&>>();
    test_constructible<int&, ExplicitTo<int&>>();
    test_constructible<const int&, ExplicitTo<int&&>>();

    // Binding through reference-compatible type is required to perform
    // direct-initialization as described in [over.match.ref] p. 1 b. 1:
    test_constructible<int&, ExplicitTo<int&>>();
    test_constructible<const int&, ExplicitTo<int&&>>();

    test_constructible<int&&, ExplicitTo<int&&>>();

#ifdef __clang__
    // FIXME Clang and GCC disagree on the validity of this expression.
    test_constructible<const int&, ExplicitTo<int>>();
    test_constructible<int&&, ExplicitTo<int>>();
#else
    test_not_constructible<const int&, ExplicitTo<int>>();
    test_not_constructible<int&&, ExplicitTo<int>>();
#endif

    // Binding through temporary behaves like copy-initialization,
    // see [dcl.init.ref] p. 5, very last sub-bullet:
    test_not_constructible<const int&, ExplicitTo<double&&>>();
    test_not_constructible<int&&, ExplicitTo<double&&>>();

    test_not_constructible<void()>();
    test_not_constructible<void() const>();
    test_not_constructible<void() volatile>();
    test_not_constructible<void() &>();
    test_not_constructible<void() &&>();
}
