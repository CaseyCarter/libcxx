//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17

// XFAIL: gcc-7

// <functional>

// identity

// Test that identity can be constructed w/o an initializer in a constexpr
// context. This is specifically testing gcc.gnu.org/PR83921

#include <functional>

constexpr bool test_constexpr_context() {
    std::identity t;
    (void)t;
    return true;
}

static_assert(test_constexpr_context());

int main() {}
