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

#include <iterator>

int main() {
    static_assert(std::Incrementable<int>);
    static_assert(std::Incrementable<unsigned int>);
    static_assert(!std::Incrementable<void>);
    static_assert(std::Incrementable<int*>);
    static_assert(std::Incrementable<const int*>);
}
