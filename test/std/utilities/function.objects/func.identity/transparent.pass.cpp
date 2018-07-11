//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17

#include <functional>
#include <type_traits>

template <class, class = void>
constexpr bool is_transparent = false;
template <class T>
constexpr bool is_transparent<T, std::void_t<typename T::is_transparent>> = true;

static_assert(is_transparent<std::identity>);

int main() {}
