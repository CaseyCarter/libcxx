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

#include <bitset>
#include <concepts>
#include <type_traits>
#include <utility>

using std::Boolean;

struct A {};
struct B { operator bool() const; };
struct C { explicit operator bool() const; };
struct D { operator bool(); };
enum E : bool { No, Yes };
enum class F : bool { No, Yes };

template<unsigned Select> // selects one requirement to violate
struct G {
    operator bool() const requires Select != 0;
    explicit operator bool() const requires Select == 1;
    void operator!() const requires Select ==  2;
};

void operator&&(G<3>, G<3>)  {}
void operator&&(G<4>, bool)  {}
void operator&&(bool, G<5>)  {}
void operator||(G<6>, G<6>)  {}
void operator||(G<7>, bool)  {}
void operator||(bool, G<8>)  {}
void operator==(G<9>, G<9>)  {}
void operator==(G<10>, bool) {}
void operator==(bool, G<11>) {}
void operator!=(G<12>, G<12>){}
void operator!=(G<13>, bool) {}
void operator!=(bool, G<14>) {}
C operator&&(G<15>, G<15>)   { return {}; }
C operator&&(G<16>, bool)    { return {}; }
C operator&&(bool, G<17>)    { return {}; }
C operator||(G<18>, G<18>)   { return {}; }
C operator||(G<19>, bool)    { return {}; }
C operator||(bool, G<20>)    { return {}; }
C operator==(G<21>, G<21>)   { return {}; }
C operator==(G<22>, bool)    { return {}; }
C operator==(bool, G<23>)    { return {}; }
C operator!=(G<24>, G<24>)   { return {}; }
C operator!=(G<25>, bool)    { return {}; }
C operator!=(bool, G<26>)    { return {}; }

template<>
struct G<27> {
    operator bool() const;
    G() = default;
    G(G&&) = delete;
};

static constexpr int Gmax = 28; // G<Gmax> violates no requirements

template <class T>
void test_boolean() {
    static_assert(Boolean<T>);
}

template <class T>
void test_not_boolean() {
    static_assert(!Boolean<T>);
}

template <std::size_t... Is>
void test_g(std::index_sequence<Is...>) {
    static_assert(std::is_same_v<std::index_sequence<Is...>,
        std::make_index_sequence<Gmax>>);
    (test_not_boolean<G<Is>>(), ...);
    test_boolean<G<sizeof...(Is)>>();
}

int main() {
    // Better have these three, since we use them as examples in the Standard.
    test_boolean<bool>();
    test_boolean<std::true_type>();
    test_boolean<std::bitset<42>::reference>();

    test_boolean<std::false_type>();

    test_boolean<int>();
    test_not_boolean<void*>();

    test_not_boolean<A>();
    test_boolean<B>();
    test_not_boolean<C>();
    test_not_boolean<D>();
    test_boolean<E>();
    test_not_boolean<F>();

    test_g(std::make_index_sequence<Gmax>{});
}
