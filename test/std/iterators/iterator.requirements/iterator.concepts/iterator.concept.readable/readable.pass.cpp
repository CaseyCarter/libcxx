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
    int operator*() const;
    using value_type = int;
};

struct MoveOnlyReadable
{
    using value_type = std::unique_ptr<int>;
    value_type operator*() const;
};

struct ArrayReadable
{
    using value_type = int[2];
    value_type& operator*() const;
};

struct Abstract
{
    virtual void foo() = 0;
};

struct AbstractReadable
{
    using value_type = Abstract;
    Abstract& operator*() const;
};

int main() {
    static_assert(!std::Readable<void>);
    static_assert(!std::Readable<void*>);
    static_assert(std::Readable<int*>);
    static_assert(std::Readable<const int*>);
    static_assert(std::Readable<A>);
    static_assert(std::Readable<MoveOnlyReadable>);
    static_assert(std::Readable<ArrayReadable>);
    static_assert(std::Readable<AbstractReadable>);
}
