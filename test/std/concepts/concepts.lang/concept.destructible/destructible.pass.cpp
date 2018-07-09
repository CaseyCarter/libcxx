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

// Prevent warning when testing the Abstract test type.
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif

#include "test_macros.h"

using std::Destructible;

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

struct indestructible {
    ~indestructible() = delete;
};

struct throwing_destructor {
    ~throwing_destructor() noexcept(false);
};

class Empty {};

class NotEmpty
{
    virtual ~NotEmpty();
};

union Union {};

struct bit_zero
{
    int :  0;
};

struct A
{
    ~A();
};

typedef void (Function) ();

struct PublicAbstract                    { public:    virtual void foo() = 0; };
struct ProtectedAbstract                 { protected: virtual void foo() = 0; };
struct PrivateAbstract                   { private:   virtual void foo() = 0; };

struct PublicDestructor                  { public:    ~PublicDestructor() {}};
struct ProtectedDestructor               { protected: ~ProtectedDestructor() {}};
struct PrivateDestructor                 { private:   ~PrivateDestructor() {}};

struct VirtualPublicDestructor           { public:    virtual ~VirtualPublicDestructor() {}};
struct VirtualProtectedDestructor        { protected: virtual ~VirtualProtectedDestructor() {}};
struct VirtualPrivateDestructor          { private:   virtual ~VirtualPrivateDestructor() {}};

struct PurePublicDestructor              { public:    virtual ~PurePublicDestructor() = 0; };
struct PureProtectedDestructor           { protected: virtual ~PureProtectedDestructor() = 0; };
struct PurePrivateDestructor             { private:   virtual ~PurePrivateDestructor() = 0; };

struct DeletedPublicDestructor           { public:    ~DeletedPublicDestructor() = delete; };
struct DeletedProtectedDestructor        { protected: ~DeletedProtectedDestructor() = delete; };
struct DeletedPrivateDestructor          { private:   ~DeletedPrivateDestructor() = delete; };

struct DeletedVirtualPublicDestructor    { public:    virtual ~DeletedVirtualPublicDestructor() = delete; };
struct DeletedVirtualProtectedDestructor { protected: virtual ~DeletedVirtualProtectedDestructor() = delete; };
struct DeletedVirtualPrivateDestructor   { private:   virtual ~DeletedVirtualPrivateDestructor() = delete; };

template <class T>
void test_destructible() {
    static_assert(Destructible<T>);
    static_assert(Destructible<const T>);
    static_assert(Destructible<volatile T>);
    static_assert(Destructible<const volatile T>);
}

template <class T>
void test_not_destructible() {
    static_assert(!Destructible<T>);
    static_assert(!Destructible<const T>);
    static_assert(!Destructible<volatile T>);
    static_assert(!Destructible<const volatile T>);
}

int main() {
    test_destructible<int>();
    test_destructible<const int>();
    test_destructible<int&>();
    test_destructible<void(*)()>();
    test_destructible<void(&)()>();
    test_destructible<int[2]>();
    test_destructible<int(*)[2]>();
    test_destructible<int(&)[2]>();
    test_destructible<copyable>();
    test_destructible<moveonly>();
    test_destructible<copyonly>();
    test_destructible<nonmovable>();

    test_not_destructible<void()>();
    test_not_destructible<int[]>();
    test_not_destructible<indestructible>();
    test_not_destructible<throwing_destructor>();
    test_not_destructible<void>();

    test_destructible<A>();
    test_destructible<int&>();
    test_destructible<Union>();
    test_destructible<Empty>();
    test_destructible<int>();
    test_destructible<double>();
    test_destructible<int*>();
    test_destructible<const int*>();
    test_destructible<char[3]>();
    test_destructible<bit_zero>();
    test_destructible<int[3]>();
    test_destructible<ProtectedAbstract>();
    test_destructible<PublicAbstract>();
    test_destructible<PrivateAbstract>();
    test_destructible<PublicDestructor>();
    test_destructible<VirtualPublicDestructor>();
    test_destructible<PurePublicDestructor>();

    test_not_destructible<int[]>();
    test_not_destructible<void>();
    test_not_destructible<Function>();

    // Test access controlled destructors
    test_not_destructible<ProtectedDestructor>();
    test_not_destructible<PrivateDestructor>();
    test_not_destructible<VirtualProtectedDestructor>();
    test_not_destructible<VirtualPrivateDestructor>();
    test_not_destructible<PureProtectedDestructor>();
    test_not_destructible<PurePrivateDestructor>();

    // Test deleted constructors
    test_not_destructible<DeletedPublicDestructor>();
    test_not_destructible<DeletedProtectedDestructor>();
    test_not_destructible<DeletedPrivateDestructor>();
    test_not_destructible<DeletedVirtualPublicDestructor>(); // previously failed due to clang bug #20268
    test_not_destructible<DeletedVirtualProtectedDestructor>();
    test_not_destructible<DeletedVirtualPrivateDestructor>();

    // Test private destructors
    test_not_destructible<NotEmpty>();
}
