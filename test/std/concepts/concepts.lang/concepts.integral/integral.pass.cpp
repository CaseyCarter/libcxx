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
#include <cstddef>

template <class> constexpr bool always_false = false;

template <class T>
struct do_not_instantiate {
    static_assert(always_false<T>);
};

namespace X {
    enum class is {
        not_integral, integral, signed_integral, unsigned_integral, ull
    };

    template <class T>
    constexpr is f(T&&) { return is::not_integral; }
    constexpr is f(std::Integral) { return is::integral; }
    constexpr is f(std::SignedIntegral) { return is::signed_integral; }
    constexpr is f(std::UnsignedIntegral) { return is::unsigned_integral; }
    constexpr is f(unsigned long long) { return is::ull; }
}

void test_subsumption() {
    using namespace X;

    static_assert(f(0.0f) == is::not_integral);
    static_assert(f(0.0) == is::not_integral);
    static_assert(f(0) == is::signed_integral);
    static_assert(f(0u) == is::unsigned_integral);
    static_assert(f(nullptr) == is::not_integral);
    static_assert(f(0ull) == is::ull);
    static_assert((f('a') == is::signed_integral || f('a') == is::unsigned_integral));
    {
        static int i;
        static_assert(f(&i) == is::not_integral);
    }
    {
        struct A { void foo() {} };
        static_assert(f(&A::foo) == is::not_integral);
    }
}

template <class T>
void test_integral() {
    static_assert(std::Integral<T>);
    static_assert(std::Integral<const T>);
    static_assert(std::Integral<volatile T>);
    static_assert(std::Integral<const volatile T>);
}

template <class T>
void test_not_integral() {
    static_assert(!std::Integral<T>);
    static_assert(!std::Integral<const T>);
    static_assert(!std::Integral<volatile T>);
    static_assert(!std::Integral<const volatile T>);
    static_assert(!std::SignedIntegral<T>);
    static_assert(!std::SignedIntegral<const T>);
    static_assert(!std::SignedIntegral<volatile T>);
    static_assert(!std::SignedIntegral<const volatile T>);
    static_assert(!std::UnsignedIntegral<T>);
    static_assert(!std::UnsignedIntegral<const T>);
    static_assert(!std::UnsignedIntegral<volatile T>);
    static_assert(!std::UnsignedIntegral<const volatile T>);
}

template <class T>
void test_signed_integral() {
    test_integral<T>();

    static_assert(std::SignedIntegral<T>);
    static_assert(std::SignedIntegral<const T>);
    static_assert(std::SignedIntegral<volatile T>);
    static_assert(std::SignedIntegral<const volatile T>);
    static_assert(!std::UnsignedIntegral<T>);
    static_assert(!std::UnsignedIntegral<const T>);
    static_assert(!std::UnsignedIntegral<volatile T>);
    static_assert(!std::UnsignedIntegral<const volatile T>);
}

template <class T>
void test_unsigned_integral() {
    test_integral<T>();

    static_assert(std::UnsignedIntegral<T>);
    static_assert(std::UnsignedIntegral<const T>);
    static_assert(std::UnsignedIntegral<volatile T>);
    static_assert(std::UnsignedIntegral<const volatile T>);
    static_assert(!std::SignedIntegral<T>);
    static_assert(!std::SignedIntegral<const T>);
    static_assert(!std::SignedIntegral<volatile T>);
    static_assert(!std::SignedIntegral<const volatile T>);
}

template <class T>
void test_some_integral() {
    if constexpr (std::SignedIntegral<T>)
        test_signed_integral<T>();
    else
        test_unsigned_integral<T>();
}

int main() {
    test_signed_integral<signed char>();
    test_signed_integral<short int>();
    test_signed_integral<int>();
    test_signed_integral<long int>();
    test_signed_integral<long long int>();

    test_unsigned_integral<unsigned char>();
    test_unsigned_integral<unsigned short int>();
    test_unsigned_integral<unsigned int>();
    test_unsigned_integral<unsigned long int>();
    test_unsigned_integral<unsigned long long int>();

    test_unsigned_integral<bool>();
    test_some_integral<char>();
    test_some_integral<wchar_t>();
    test_unsigned_integral<char16_t>();
    test_unsigned_integral<char32_t>();

    test_not_integral<float>();
    test_not_integral<double>();
    test_not_integral<long double>();

    test_not_integral<void>();
    test_not_integral<std::nullptr_t>();

    test_not_integral<int[42]>();
    test_not_integral<int(int)>();
    test_not_integral<int*>();
    test_not_integral<int&>();

    {
        struct S;
        union U;
        enum E {};
        test_not_integral<S>();
        test_not_integral<U>();
        test_not_integral<do_not_instantiate<void>>();
        test_not_integral<E>();
        test_not_integral<void (S::*)()>();
    }

    test_signed_integral<std::ptrdiff_t>();
    test_unsigned_integral<std::size_t>();

    test_subsumption();
}
