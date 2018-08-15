//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This test fails due to https://gcc.gnu.org/bugzilla/shobug.cgi?id=67225.
// REQUIRES: !gcc || !concepts

// <memory>

// shared_ptr

// template<class T, class... Args> shared_ptr<T> make_shared(Args&&... args);

#include <memory>
#include <cassert>

#include "test_macros.h"

struct S {
private:
   S () {};  // ctor is private
};

int main()
{
    std::shared_ptr<S> p = std::make_shared<S>();
}
