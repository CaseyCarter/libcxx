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
#include <functional>
#include <type_traits>

template <class T>
struct S { explicit S(T const&) {} };

namespace std
{
    template <class T>
    struct common_type<T, ::S<T>> {
        using type = S<T>;
    };

    template <class T>
    struct common_type<::S<T>, T> {
        using type = S<T>;
    };
}

struct X {};
struct Y { explicit Y(X){} };

namespace std
{
    template <> struct common_type<::X, ::Y> { using type = ::Y; };
    template <> struct common_type<::Y, ::X> { using type = ::Y; };
}

template <class> constexpr bool always_false = false;

template <class T>
struct do_not_instantiate {
    static_assert(always_false<T>);
};

template <class T, class U>
void test_common() {
    static_assert(std::Common<T, U>);
    static_assert(std::Common<U, T>);
}

template <class T, class U>
void test_not_common() {
    static_assert(!std::Common<T, U>);
    static_assert(!std::Common<U, T>);
}

int main() {
    test_common<int, double>();
    test_common<double, int>();
    test_common<double, double>();
    test_common<void*, int*>();
    test_common<double,long long>();

    test_common<int,             int>();
    test_common<int,       const int>();
    test_common<const int, const int>();

    test_common<long,       const int>();
    test_common<const long,       int>();
    test_common<long,    volatile int>();
    test_common<volatile long,    int>();
    test_common<const long, const int>();

    test_common<double, char>();
    test_common<short , char>();

    test_common<void,             void>();
    test_common<void,       const void>();
    test_common<const void,       void>();
    test_common<void,    volatile void>();
    test_common<volatile void,    void>();
    test_common<const void, const void>();

    test_not_common<void, int>();
    test_not_common<int*, int>();

    test_common<std::reference_wrapper<int>, int>();

    test_common<S<int>, S<int>>();
    test_not_common<int, S<int>>();

    {
        struct B {};
        struct C { C() = default; C(B) {} C(int) {} };
        test_common<B, C>();
    }

    {
        struct incomplete;
        test_common<void*, incomplete*>();
        test_common<void*, do_not_instantiate<int>*>();
    }

    static_assert(std::is_same_v<std::common_type_t<X, Y>, Y>);
    static_assert(std::is_same_v<std::common_type_t<Y, X>, Y>);
    test_not_common<X, Y>();

    {
        struct B {};
        struct D : B {};
        test_common<D, B>();
    }

    {
        struct AA {
            AA() = default;
            AA(AA &&) = delete;
            AA& operator=(AA &&) = delete;
        };
        struct BB : AA { };
        static_assert(std::is_same_v<std::common_type_t<AA, BB>, AA>);
        static_assert(std::is_same_v<std::common_type_t<BB, AA>, AA>);
        test_not_common<AA, BB>();
    }
}
