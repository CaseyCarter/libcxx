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

#include <random>

#include "test_macros.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

struct no_call {
    static constexpr unsigned min() { return 0; }
    static constexpr unsigned max() { return 42; }
};

struct no_lvalue_call {
    static constexpr unsigned min() { return 0; }
    static constexpr unsigned max() { return 42; }
    unsigned operator()() &&;
};

struct signed_value {
    static constexpr int min() { return 0; }
    static constexpr int max() { return 42; }
    int operator()();
};

struct no_min {
    static constexpr unsigned max() { return 42; }
    unsigned operator()();
};

struct non_constexpr_min {
    static unsigned min() { return 0; }
    static constexpr unsigned max() { return 42; }
    unsigned operator()();
};

struct bad_min {
    static constexpr int min() { return 0; }
    static constexpr unsigned max() { return 42; }
    unsigned operator()();
};

struct no_max {
    static constexpr unsigned min() { return 0; }
    unsigned operator()();
};

struct non_constexpr_max {
    static constexpr unsigned min() { return 0; }
    static unsigned max() { return 42; }
    unsigned operator()();
};

struct bad_max {
    static constexpr unsigned min() { return 0; }
    static constexpr int max() { return 42; }
    unsigned operator()();
};

struct empty_range {
    static constexpr unsigned min() { return 0; }
    static constexpr unsigned max() { return 0; }
    unsigned operator()();
};

struct reversed_range {
    static constexpr unsigned min() { return 42; }
    static constexpr unsigned max() { return 0; }
    unsigned operator()();
};

struct urbg {
    static constexpr unsigned min() { return 0; }
    static constexpr unsigned max() { return 42; }
    unsigned operator()();
};

struct const_urbg {
    static constexpr unsigned min() { return 0; }
    static constexpr unsigned max() { return 42; }
    unsigned operator()() const;
};

template <class T>
void test_URBG() {
    static_assert(std::UniformRandomBitGenerator<T>);
}

template <class T>
void test_not_URBG() {
    static_assert(!std::UniformRandomBitGenerator<T>);
}

int main() {
    test_not_URBG<no_call>();
    test_not_URBG<no_lvalue_call>();
    test_not_URBG<signed_value>();
    test_not_URBG<no_min>();
    test_not_URBG<no_max>();
#ifndef TEST_COMPILER_GCC
    // Disabled due to GCC PR 86493 as validated by non_constexpr_eval.pass.cpp
    test_not_URBG<non_constexpr_min>();
    test_not_URBG<non_constexpr_max>();
#endif
    test_not_URBG<bad_min>();
    test_not_URBG<bad_max>();
    test_not_URBG<empty_range>();
    test_not_URBG<reversed_range>();

    test_URBG<urbg>();
    test_URBG<const_urbg>();
    test_URBG<const const_urbg>();

    test_URBG<std::minstd_rand0>();
    test_URBG<std::minstd_rand>();
    test_URBG<std::mt19937>();
    test_URBG<std::mt19937_64>();
    test_URBG<std::ranlux24_base>();
    test_URBG<std::ranlux48_base>();
    test_URBG<std::ranlux24>();
    test_URBG<std::ranlux48>();
    test_URBG<std::knuth_b>();
    test_URBG<std::default_random_engine>();
    test_URBG<std::random_device>();
}
