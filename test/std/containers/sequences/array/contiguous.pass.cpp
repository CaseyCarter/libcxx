//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <array>

// An array is a contiguous container

#include <array>
#include <cassert>

template <class C>
void test_contiguous ( const C &c )
{
#ifdef _LIBCPP_HAS_RANGES
    static_assert(std::Same<std::contiguous_iterator_tag,
        typename std::iterator_traits<typename C::iterator>::iterator_concept>);
    static_assert(std::Same<std::contiguous_iterator_tag,
        typename std::iterator_traits<typename C::const_iterator>::iterator_concept>);
#endif

    for ( size_t i = 0; i < c.size(); ++i )
        assert ( *(c.begin() + i) == *(std::addressof(*c.begin()) + i));
}

int main()
{
    {
        typedef double T;
        typedef std::array<T, 3> C;
        test_contiguous (C());
    }
}
