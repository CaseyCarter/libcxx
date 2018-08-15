//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This test is expected to fail due to https://gcc.gnu.org/bugzilla/shobug.cgi?id=67225.
// REQUIRES: !gcc || !concepts

// <random>

// class seed_seq;

// seed_seq();

#include <random>

int main()
{
    std::seed_seq s0;
    std::seed_seq s(s0);
}
