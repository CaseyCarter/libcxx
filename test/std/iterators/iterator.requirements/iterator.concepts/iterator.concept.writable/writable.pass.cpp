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
#include <memory>

struct A
{
    int& operator*() const;
};

int main() {
    static_assert(std::Writable<std::unique_ptr<int>*, std::unique_ptr<int>&&>);
    static_assert(!std::Writable<std::unique_ptr<int>*, std::unique_ptr<int>&>);
    static_assert(!std::Writable<void, int>);
    static_assert(!std::Writable<void*, void>);
    static_assert(std::Writable<int*, int>);
    static_assert(std::Writable<int*, int&>);
    static_assert(std::Writable<int*, const int&>);
    static_assert(std::Writable<int*, const int>);
    static_assert(!std::Writable<const int*, int>);
    static_assert(std::Writable<A, int>);
    static_assert(std::Writable<A, const int&>);
    static_assert(std::Writable<A, double>);
    static_assert(std::Writable<A, const double&>);
}
