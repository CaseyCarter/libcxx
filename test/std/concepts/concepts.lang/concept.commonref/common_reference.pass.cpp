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
#include <functional>
#include <type_traits>

using std::CommonReference;
using std::void_t;

template <class, class = void>
constexpr bool is_trait = false;
template <class T>
constexpr bool is_trait<T, std::void_t<typename T::type>> = true;

struct X {};
struct Y { explicit Y(X){} };

namespace std
{
    template <> struct common_type<X, Y> { using type = Y; };
    template <> struct common_type<Y, X> { using type = Y; };
}

static_assert(std::is_same<std::common_reference_t<int&&, int const&, int volatile&>, int const volatile&>());
static_assert(std::is_same<std::common_reference_t<int&&, int const&, float&>, float>());
static_assert(!is_trait<std::common_reference<int, short, int, char*>>);

static_assert(std::is_same<std::common_reference_t<int, short>, int>::value);
static_assert(std::is_same<std::common_reference_t<int, short&>, int>::value);
static_assert(std::is_same<std::common_reference_t<int&, short&>, int>::value);
static_assert(std::is_same<std::common_reference_t<int&, short>, int>::value);

// tricky volatile reference case
static_assert(std::is_same<std::common_reference_t<int&&, int volatile&>, int>::value);
static_assert(std::is_same<std::common_reference_t<int volatile&, int&&>, int>::value);
static_assert(std::is_same<std::common_reference_t<int const volatile&&, int volatile&&>, int const volatile&&>::value);
static_assert(std::is_same<std::common_reference_t<int&&, int const&, int volatile&>, int const volatile&>());

// Array types?? Yup!
static_assert(std::is_same<std::common_reference_t<int (&)[10], int (&&)[10]>, int const(&)[10]>::value);
static_assert(std::is_same<std::common_reference_t<int const (&)[10], int volatile (&)[10]>, int const volatile(&)[10]>::value);
static_assert(std::is_same<std::common_reference_t<int (&)[10], int (&)[11]>, int *>::value);

struct X2 {};
struct Y2 {};
struct Z2 {};

namespace std
{
    template <>
    struct common_type<X2, Y2>
    {
        using type = Z2;
    };
    template <>
    struct common_type<Y2, X2>
    {
        using type = Z2;
    };
} // namespace std

template <class T, class U>
void test_common_ref() {
    static_assert(CommonReference<T, U>);
    static_assert(CommonReference<U, T>);
}

template <class T, class U>
void test_not_common_ref() {
    static_assert(!CommonReference<T, U>);
    static_assert(!CommonReference<U, T>);
}

int main() {
    test_common_ref<int, int>();
    test_common_ref<int, double>();
    test_common_ref<double, int>();
    test_common_ref<double, double>();
    test_not_common_ref<void, int>();
    test_not_common_ref<int*, int>();
    test_common_ref<void*, int*>();
    test_common_ref<double,long long>();
    test_common_ref<void, void>();

    test_not_common_ref<X, Y>();

    {
        struct AA {
            AA() = default;
            AA(AA&&) = delete;
            AA(AA const&) = delete;
        };
        struct BB : AA { };

        test_common_ref<AA&, BB&>();
        test_common_ref<AA&&, BB&&>();
        test_not_common_ref<AA, BB>();
    }

    {
        struct B {};
        struct D : B {};

        test_common_ref<B&, D&>();
        test_common_ref<B&, D const&>();
        test_common_ref<B const&, D&>();
        test_common_ref<B const&, D const&>();

        test_common_ref<B&&, D&&>();
        test_common_ref<B&&, D const&&>();
        test_common_ref<B const&&, D&&>();
        test_common_ref<B const&&, D const&&>();

        test_common_ref<B&, D&&>();
        test_common_ref<B&, D const&&>();
        test_common_ref<B const&, D&&>();
        test_common_ref<B const&, D const&&>();

        test_common_ref<B&&, D&>();
        test_common_ref<B&&, D const&>();
        test_common_ref<B const&&, D&>();
        test_common_ref<B const&&, D const&>();

        test_common_ref<B&&, D&&>();
        test_common_ref<B&&, D const&&>();
        test_common_ref<B const&&, D&&>();
        test_common_ref<B const&&, D const&&>();
    }

    {
        // https://github.com/ericniebler/stl2/issues/338
        struct MyIntRef { MyIntRef(int&); };
        test_common_ref<int&, MyIntRef>();
    }

    {
        struct noncopyable {
            noncopyable() = default;
            noncopyable(noncopyable&&) = default;
            noncopyable& operator=(noncopyable&&) = default;
        };
        struct noncopyable2 : noncopyable {};

        test_not_common_ref<noncopyable const&, noncopyable>();
        test_common_ref<noncopyable const&, noncopyable&>();
        test_not_common_ref<noncopyable2 const&, noncopyable>();
        test_common_ref<noncopyable2 const&, noncopyable&&>();
        test_not_common_ref<noncopyable const&, noncopyable2>();
        test_common_ref<noncopyable const&, noncopyable2 const&>();
    }

    {
        static_assert(std::is_same_v<Z2,
            std::common_reference_t<X2&, Y2 const&>>);
        test_not_common_ref<X2&, Y2 const&>();
    }

    {
        struct B {};
        struct C { C() = default; C(B); C(int); };
        test_common_ref<B, C>();
    }
}
