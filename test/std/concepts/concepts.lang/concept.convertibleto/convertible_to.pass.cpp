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

#include <cassert>
#include <concepts>

template <class From, class To>
void test_convertible_to() {
    static_assert(std::ConvertibleTo<From, To>);
    static_assert(std::ConvertibleTo<const From, To>);
    static_assert(std::ConvertibleTo<From, const To>);
    static_assert(std::ConvertibleTo<const From, const To>);
}

template <class From, class To>
void test_not_convertible_to() {
    static_assert(!std::ConvertibleTo<From, To>);
    static_assert(!std::ConvertibleTo<const From, To>);
    static_assert(!std::ConvertibleTo<From, const To>);
    static_assert(!std::ConvertibleTo<const From, const To>);
}

using Function = void();
using ConstFunction = void() const;
using Array = char[1];

struct StringType {
    StringType(const char*) {}
};

class NonCopyable {
    NonCopyable(NonCopyable&);
};

template <typename T>
class CannotInstantiate {
    enum { X = T::ThisExpressionWillBlowUp };
};

namespace X {
    struct A { A() = default; A(int) {} };

    enum class result {
        exact, convertible, unrelated
    };

    result f(A) {
        return result::exact;
    }

    template <std::ConvertibleTo<A> T>
    result f(T&&) {
        return result::convertible;
    }

    template <class T>
    result f(T) {
        return result::unrelated;
    }
} // unnamed namespace

int main() {
    // void
    test_convertible_to<void, void>();
    test_not_convertible_to<void, Function>();
    test_not_convertible_to<void, Function&>();
    test_not_convertible_to<void, Function*>();
    test_not_convertible_to<void, Array>();
    test_not_convertible_to<void, Array&>();
    test_not_convertible_to<void, char>();
    test_not_convertible_to<void, char&>();
    test_not_convertible_to<void, char*>();
    test_not_convertible_to<char, void>();

    // Function
    test_not_convertible_to<Function, void>();
    test_not_convertible_to<Function, Function>();
    test_convertible_to<Function, Function&>();
    test_convertible_to<Function, Function*>();
    test_convertible_to<Function, Function*const>();

    static_assert(std::ConvertibleTo<Function, Function&&>);

    test_not_convertible_to<Function, Array>();
    test_not_convertible_to<Function, Array&>();
    test_not_convertible_to<Function, char>();
    test_not_convertible_to<Function, char&>();
    test_not_convertible_to<Function, char*>();

    // Function&
    test_not_convertible_to<Function&, void>();
    test_not_convertible_to<Function&, Function>();
    test_convertible_to<Function&, Function&>();

    test_convertible_to<Function&, Function*>();
    test_not_convertible_to<Function&, Array>();
    test_not_convertible_to<Function&, Array&>();
    test_not_convertible_to<Function&, char>();
    test_not_convertible_to<Function&, char&>();
    test_not_convertible_to<Function&, char*>();

    // Function*
    test_not_convertible_to<Function*, void>();
    test_not_convertible_to<Function*, Function>();
    test_not_convertible_to<Function*, Function&>();
    test_convertible_to<Function*, Function*>();

    test_not_convertible_to<Function*, Array>();
    test_not_convertible_to<Function*, Array&>();
    test_not_convertible_to<Function*, char>();
    test_not_convertible_to<Function*, char&>();
    test_not_convertible_to<Function*, char*>();

    // Non-referencable function type
    static_assert(!std::ConvertibleTo<ConstFunction, Function>);
    static_assert(!std::ConvertibleTo<ConstFunction, Function*>);
    static_assert(!std::ConvertibleTo<ConstFunction, Function&>);
    static_assert(!std::ConvertibleTo<ConstFunction, Function&&>);
    static_assert(!std::ConvertibleTo<Function*, ConstFunction>);
    static_assert(!std::ConvertibleTo<Function&, ConstFunction>);
    static_assert(!std::ConvertibleTo<ConstFunction, ConstFunction>);
    static_assert(!std::ConvertibleTo<ConstFunction, void>);

    // Array
    test_not_convertible_to<Array, void>();
    test_not_convertible_to<Array, Function>();
    test_not_convertible_to<Array, Function&>();
    test_not_convertible_to<Array, Function*>();
    test_not_convertible_to<Array, Array>();

    static_assert(!std::ConvertibleTo<Array, Array&>);
    static_assert( std::ConvertibleTo<Array, const Array&>);
    static_assert(!std::ConvertibleTo<Array, const volatile Array&>);

    static_assert(!std::ConvertibleTo<const Array, Array&>);
    static_assert( std::ConvertibleTo<const Array, const Array&>);
    static_assert(!std::ConvertibleTo<Array, volatile Array&>);
    static_assert(!std::ConvertibleTo<Array, const volatile Array&>);

    static_assert( std::ConvertibleTo<Array, Array&&>);
    static_assert( std::ConvertibleTo<Array, const Array&&>);
    static_assert( std::ConvertibleTo<Array, volatile Array&&>);
    static_assert( std::ConvertibleTo<Array, const volatile Array&&>);
    static_assert( std::ConvertibleTo<const Array, const Array&&>);
    static_assert(!std::ConvertibleTo<Array&, Array&&>);
    static_assert(!std::ConvertibleTo<Array&&, Array&>);

    test_not_convertible_to<Array, char>();
    test_not_convertible_to<Array, char&>();

    static_assert(std::ConvertibleTo<Array, char*>);
    static_assert(std::ConvertibleTo<Array, const char*>);
    static_assert(std::ConvertibleTo<Array, char* const>);
    static_assert(std::ConvertibleTo<Array, char* const volatile>);

    static_assert(!std::ConvertibleTo<const Array, char*>);
    static_assert( std::ConvertibleTo<const Array, const char*>);

    static_assert(!std::ConvertibleTo<char[42][42], char*>);
    static_assert(!std::ConvertibleTo<char[][1], char*>);

    // Array&
    test_not_convertible_to<Array&, void>();
    test_not_convertible_to<Array&, Function>();
    test_not_convertible_to<Array&, Function&>();
    test_not_convertible_to<Array&, Function*>();
    test_not_convertible_to<Array&, Array>();

    static_assert( std::ConvertibleTo<Array&, Array&>);
    static_assert( std::ConvertibleTo<Array&, const Array&>);
    static_assert(!std::ConvertibleTo<const Array&, Array&>);
    static_assert( std::ConvertibleTo<const Array&, const Array&>);

    test_not_convertible_to<Array&, char>();
    test_not_convertible_to<Array&, char&>();

    static_assert( std::ConvertibleTo<Array&, char*>);
    static_assert( std::ConvertibleTo<Array&, const char*>);
    static_assert(!std::ConvertibleTo<const Array&, char*>);
    static_assert( std::ConvertibleTo<const Array&, const char*>);

    static_assert(std::ConvertibleTo<Array, StringType>);
    static_assert(std::ConvertibleTo<char(&)[], StringType>);

    // char
    test_not_convertible_to<char, void>();
    test_not_convertible_to<char, Function>();
    test_not_convertible_to<char, Function&>();
    test_not_convertible_to<char, Function*>();
    test_not_convertible_to<char, Array>();
    test_not_convertible_to<char, Array&>();

    test_convertible_to<char, char>();

    static_assert(!std::ConvertibleTo<char, char&>);
    static_assert( std::ConvertibleTo<char, const char&>);
    static_assert(!std::ConvertibleTo<const char, char&>);
    static_assert( std::ConvertibleTo<const char, const char&>);

    test_not_convertible_to<char, char*>();

    // char&
    test_not_convertible_to<char&, void>();
    test_not_convertible_to<char&, Function>();
    test_not_convertible_to<char&, Function&>();
    test_not_convertible_to<char&, Function*>();
    test_not_convertible_to<char&, Array>();
    test_not_convertible_to<char&, Array&>();

    test_convertible_to<char&, char>();

    static_assert( std::ConvertibleTo<char&, char&>);
    static_assert( std::ConvertibleTo<char&, const char&>);
    static_assert(!std::ConvertibleTo<const char&, char&>);
    static_assert( std::ConvertibleTo<const char&, const char&>);

    test_not_convertible_to<char&, char*>();

    // char*
    test_not_convertible_to<char*, void>();
    test_not_convertible_to<char*, Function>();
    test_not_convertible_to<char*, Function&>();
    test_not_convertible_to<char*, Function*>();
    test_not_convertible_to<char*, Array>();
    test_not_convertible_to<char*, Array&>();

    test_not_convertible_to<char*, char>();
    test_not_convertible_to<char*, char&>();

    static_assert( std::ConvertibleTo<char*, char*>);
    static_assert( std::ConvertibleTo<char*, const char*>);
    static_assert(!std::ConvertibleTo<const char*, char*>);
    static_assert( std::ConvertibleTo<const char*, const char*>);

    // NonCopyable
    static_assert( std::ConvertibleTo<NonCopyable&, NonCopyable&>);
    static_assert( std::ConvertibleTo<NonCopyable&, const NonCopyable&>);
    static_assert( std::ConvertibleTo<NonCopyable&, const volatile NonCopyable&>);
    static_assert( std::ConvertibleTo<NonCopyable&, volatile NonCopyable&>);
    static_assert( std::ConvertibleTo<const NonCopyable&, const NonCopyable&>);
    static_assert( std::ConvertibleTo<const NonCopyable&, const volatile NonCopyable&>);
    static_assert( std::ConvertibleTo<volatile NonCopyable&, const volatile NonCopyable&>);
    static_assert( std::ConvertibleTo<const volatile NonCopyable&, const volatile NonCopyable&>);
    static_assert(!std::ConvertibleTo<const NonCopyable&, NonCopyable&>);

    test_not_convertible_to<NonCopyable&, NonCopyable>();

    // Ensure that CannotInstantiate is not instantiated by is_convertible when it is not needed.
    // For example CannotInstantiate is instatiated as a part of ADL lookup for arguments of type CannotInstantiate*.
    static_assert(std::ConvertibleTo<CannotInstantiate<int>*, CannotInstantiate<int>*>);

    {
        using namespace X;
        assert(f(A{}) == result::exact);
        { const A a{}; assert(f(a) == result::exact); }
        assert(f(42) == result::convertible);
        assert(f("foo") == result::unrelated);
    }

    {
        struct B {};
        struct D : B {};

        test_convertible_to<B, B>();
        test_convertible_to<D, D>();
        test_convertible_to<D, B>();
        test_not_convertible_to<B, D>();
    }

    {
        struct Z;
        struct X {
            X() = default;
            explicit X(Z) = delete;
        };
        struct Y1 {
            operator X() const;
        };
        struct Y2 {
            explicit operator X() const;
        };
        struct Z {
            operator X() const;
        };
        test_convertible_to<Y1, X>();     // Both implicitly and explicitly convertible
        test_not_convertible_to<Y2, X>(); // Only explicitly convertible
        test_not_convertible_to<Z, X>();  // Only implicitly convertible
    }
}
