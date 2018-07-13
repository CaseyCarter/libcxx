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
// XFAIL: gcc

#include <__config>

// Bug test for https://gcc.gnu.org/bugzilla/show_bug.cgi?id=78173

template <class _Tp>
_LIBCPP_CONCEPT_DECL CanDifference =
    requires(const _Tp& __x, const _Tp& __y) {
        __x - __y;
    };

static_assert(!CanDifference<void*>);
static_assert(!CanDifference<void const*>);
static_assert(!CanDifference<void volatile*>);
static_assert(!CanDifference<void const volatile*>);
