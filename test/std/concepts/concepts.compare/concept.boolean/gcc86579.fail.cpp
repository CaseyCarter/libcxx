// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03
// XFAIL: gcc

// Bug test for https://gcc.gnu.org/bugzilla/show_bug.cgi?id=86579
// GCC incorrectly allows relational comparisons with nullptr_t

int main() {
    void(nullptr < nullptr);
}
