//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17

// <functional>

// identity

#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>

struct S {
    int i;

    constexpr bool operator==(S const& that) {
        return i == that.i;
    }
};

constexpr bool test_constexpr() {
    constexpr std::identity f{};

    static_assert(f(42) == 42);
    static_assert(f(3.14) == 3.14);
    static_assert(f(S{42}) == S{42});

    int i = 42;
    assert(&f(i) == &i);

    double pi = 3.14;
    assert(&f(pi) == &pi);

    S s{42};
    assert(&f(s) == &s);

    auto&& i_rr = f(std::move(i));
    static_assert(std::is_same_v<decltype(i_rr), int&&>);
    assert(&i_rr == &i);

    auto&& s_rr = f(std::move(s));
    static_assert(std::is_same_v<decltype(s_rr), S&&>);
    assert(&s_rr == &s);

    auto&& pi_rr = f(std::move(pi));
    static_assert(std::is_same_v<decltype(pi_rr), double&&>);
    assert(&pi_rr == &pi);

    return true;
}

static_assert(test_constexpr());

int main() {
    std::identity f;

    assert(f(42) == 42);
    assert(f(3.14) == 3.14);
    assert(f(S{42}) == S{42});

    int i = 42;
    assert(&f(i) == &i);

    double pi = 3.14;
    assert(&f(pi) == &pi);

    S s{42};
    assert(&f(s) == &s);

    auto&& i_rr = f(std::move(i));
    static_assert(std::is_same_v<decltype(i_rr), int&&>);
    assert(&i_rr == &i);

    auto&& s_rr = f(std::move(s));
    static_assert(std::is_same_v<decltype(s_rr), S&&>);
    assert(&s_rr == &s);

    auto&& pi_rr = f(std::move(pi));
    static_assert(std::is_same_v<decltype(pi_rr), double&&>);
    assert(&pi_rr == &pi);
}
