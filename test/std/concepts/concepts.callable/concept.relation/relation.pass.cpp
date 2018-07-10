//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17

#include <concepts>

struct EqualTo {
    template <class T, class U>
    constexpr auto operator()(T&& t, U&& u) const ->
        decltype(static_cast<T&&>(t) == static_cast<U&&>(u))
    {
        return static_cast<T&&>(t) == static_cast<U&&>(u);
    }
};

struct Bool { operator bool() const; };

template <unsigned> struct A {};
template <unsigned U>
    requires 0 < U
Bool operator==(A<U>, A<U>);

template <unsigned> struct B {};
void operator==(B<1>, B<1>);
template <unsigned U>
bool operator==(B<U>, B<U>);

template <unsigned I>
requires 5 != I
bool operator==(A<I>, B<I>);
template <unsigned I>
requires 6 != I
bool operator==(B<I>, A<I>);

template <unsigned I> struct C
{
    C(A<I>) requires 4 != I;
    C(B<I>);
};
enum E : bool { No, Yes };
E operator==(C<2>&, C<2>&);
template <unsigned I>
    requires 2 != I
E operator==(C<I>, C<I>);

namespace std
{
    template <unsigned I>
        requires 3 != I
    struct common_type<::A<I>, ::B<I>> {
        using type = ::C<I>;
    };
    template <unsigned I>
        requires 3 != I
    struct common_type<::B<I>, ::A<I>> {
        using type = ::C<I>;
    };
}

template <class F, class T, class U = T>
void test_relation() {
    static_assert(std::Relation<F, T, U>);
    static_assert(std::StrictWeakOrder<F, T, U>);
}

template <class F, class T, class U = T>
void test_not_relation() {
    static_assert(!std::Relation<F, T, U>);
    static_assert(!std::StrictWeakOrder<F, T, U>);
}

int main() {
    test_not_relation<::EqualTo, A<0>>(); // A<0> has no ==
    test_relation<::EqualTo, A<1>>();
    test_relation<::EqualTo, B<0>>();

    test_not_relation<::EqualTo, B<1>>(); // B<1> has non-predicate ==

    test_relation<::EqualTo, C<0>>();
    test_not_relation<::EqualTo, C<2>>(); // C<2> has mutable-only ==

    test_not_relation<::EqualTo, A<0>, B<0>>(); // A<0> has no ==
    test_not_relation<::EqualTo, A<1>, B<1>>(); // B<1> has non-predicate ==
    test_not_relation<::EqualTo, A<2>, B<2>>(); // C<2> has mutable-only ==
    test_not_relation<::EqualTo, A<3>, B<3>>(); // No common_reference_t<A<3>, B<3>>
    test_not_relation<::EqualTo, A<4>, B<4>>(); // A not convertible to common_reference
    test_not_relation<::EqualTo, A<5>, B<5>>(); // No A<5> == B<5>
    test_not_relation<::EqualTo, A<6>, B<6>>(); // No B<6> == A<6>
    test_relation<::EqualTo, A<7>, B<7>>();
}
