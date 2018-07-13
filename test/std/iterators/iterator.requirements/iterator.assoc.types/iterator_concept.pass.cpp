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
#include <type_traits>

using std::Same;
using std::iterator_traits;
using std::contiguous_iterator_tag;

int main() {
    static_assert(Same<iterator_traits<int*>::iterator_concept, contiguous_iterator_tag>);
    static_assert(Same<iterator_traits<const int*>::iterator_concept, contiguous_iterator_tag>);
}
