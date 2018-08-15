//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <memory>

// template <class Alloc>
// struct allocator_traits
// {
//     typedef Alloc::propagate_on_container_move_assignment
//           | false_type                   propagate_on_container_move_assignment;
//     ...
// };

#include <memory>
#include <type_traits>

#include "test_macros.h"

template <class T>
struct A
{
    typedef T value_type;
    typedef std::true_type propagate_on_container_move_assignment;
};

template <class T>
struct B
{
    typedef T value_type;
};


template <class T>
struct C
{
    typedef T value_type;
private:
    typedef std::true_type propagate_on_container_move_assignment;
};


int main()
{
    static_assert((std::is_same<std::allocator_traits<A<char> >::propagate_on_container_move_assignment, std::true_type>::value), "");
    static_assert((std::is_same<std::allocator_traits<B<char> >::propagate_on_container_move_assignment, std::false_type>::value), "");
#if TEST_STD_VER >= 11
#if defined(__clang__) || !(defined(__cpp_concepts) && defined(__GNUC__))
    // Avoid triggering https://gcc.gnu.org/bugzilla/shobug.cgi?id=67225.
    static_assert((std::is_same<std::allocator_traits<C<char> >::propagate_on_container_move_assignment, std::false_type>::value), "");
#endif
#endif
}
