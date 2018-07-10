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
#include <functional>
#include <memory>
#include <type_traits>

struct Bool { operator bool() const; };

struct Tag {};
struct DerFromTag : Tag {};

struct Implicit {
    Implicit(int) {}
};

struct Explicit {
    explicit Explicit(int);
};

struct NotCallableWithInt {
    int operator()(int) = delete;
    Bool operator()(Tag);
};

struct S
{
    typedef Bool (*FreeFunc)(long);
    operator FreeFunc() const;
    Bool operator()(char, int&);
    Bool const& operator()(char, int&) const;
    Bool volatile& operator()(char, int&) volatile;
    Bool const volatile& operator()(char, int&) const volatile;
};


struct SD : public S { };

struct NotDerived {};

template <class F, class... Args>
void test_predicate() {
    static_assert(std::Predicate<F, Args...>);
}

template <class F, class... Args>
void test_not_predicate() {
    static_assert(!std::Predicate<F, Args...>);
}

int main() {
    {
        using Fn = Bool(Tag::*)(int);
        using RFn = Bool(Tag::*)(int) &&;
        // INVOKE bullet 1, 2 and 3
        {
            // Bullet 1
            test_predicate<Fn, Tag&, int>();
            test_predicate<Fn, DerFromTag&, int>();
            test_predicate<RFn, Tag&&, int>();
            test_not_predicate<RFn, Tag&, int>();
            test_not_predicate<Fn, Tag&>();
            test_not_predicate<Fn, Tag const&, int>();
        }
        {
            // Bullet 2
            using T = std::reference_wrapper<Tag>;
            using DT = std::reference_wrapper<DerFromTag>;
            using CT = std::reference_wrapper<const Tag>;
            test_predicate<Fn, T&, int>();
            test_predicate<Fn, DT&, int>();
            test_predicate<Fn, const T&, int>();
            test_predicate<Fn, T&&, int>();
            test_not_predicate<Fn, CT&, int>();
            test_not_predicate<RFn, T, int>();
        }
        {
            // Bullet 3
            using T = Tag*;
            using DT = DerFromTag*;
            using CT = const Tag*;
            using ST = std::unique_ptr<Tag>;
            test_predicate<Fn, T&, int>();
            test_predicate<Fn, DT&, int>();
            test_predicate<Fn, const T&, int>();
            test_predicate<Fn, T&&, int>();
            test_predicate<Fn, ST, int>();
            test_not_predicate<Fn, CT&, int>();
            test_not_predicate<RFn, T, int>();
        }
    }
    {
        // Bullets 4, 5 and 6
        using Fn = Bool (Tag::*);
        test_not_predicate<Fn>();
        {
            // Bullet 4
            test_predicate<Fn, Tag&>();
            test_predicate<Fn, DerFromTag&>();
            test_predicate<Fn, Tag&&>();
            test_predicate<Fn, Tag const&>();
        }
        {
            // Bullet 5
            using T = std::reference_wrapper<Tag>;
            using DT = std::reference_wrapper<DerFromTag>;
            using CT = std::reference_wrapper<const Tag>;
            test_predicate<Fn, T&>();
            test_predicate<Fn, DT&>();
            test_predicate<Fn, const T&>();
            test_predicate<Fn, T&&>();
            test_predicate<Fn, CT&>();
        }
        {
            // Bullet 6
            using T = Tag*;
            using DT = DerFromTag*;
            using CT = const Tag*;
            using ST = std::unique_ptr<Tag>;
            test_predicate<Fn, T&>();
            test_predicate<Fn, DT&>();
            test_predicate<Fn, const T&>();
            test_predicate<Fn, T&&>();
            test_predicate<Fn, ST>();
            test_predicate<Fn, CT&>();
        }
    }
    {
        // INVOKE bullet 7
        {
            // Function pointer
            using Fp = Bool(*)(Tag&, int);
            test_predicate<Fp, Tag&, int>();
            test_predicate<Fp, DerFromTag&, int>();
            test_not_predicate<Fp, const Tag&, int>();
            test_not_predicate<Fp>();
            test_not_predicate<Fp, Tag&>();
        }
        {
            // Function reference
            using Fp = Bool(&)(Tag&, int);
            test_predicate<Fp, Tag&, int>();
            test_predicate<Fp, DerFromTag&, int>();
            test_not_predicate<Fp, const Tag&, int>();
            test_not_predicate<Fp>();
            test_not_predicate<Fp, Tag&>();
        }
        {
            // Function object
            using Fn = NotCallableWithInt;
            test_predicate<Fn, Tag>();
            test_not_predicate<Fn, int>();
        }
    }

    typedef NotDerived ND;
    { // functor object
        test_predicate<S, int>();
        test_predicate<S&, unsigned char, int&>();
        test_predicate<S const&, unsigned char, int&>();
        test_not_predicate<S volatile&, unsigned char, int&>();
        test_not_predicate<S const volatile&, unsigned char, int&>();
    }

    { // pointer to function
        typedef bool        (&RF0)();
        typedef bool*       (&RF1)(int);
        typedef bool&       (&RF2)(int, int);
        typedef bool const& (&RF3)(int, int, int);
        typedef bool        (&RF4)(int, ...);
        typedef bool        (*PF0)();
        typedef bool*       (*PF1)(int);
        typedef bool&       (*PF2)(int, int);
        typedef bool const& (*PF3)(int, int, int);
        typedef bool        (*PF4)(int, ...);
        typedef bool        (*&PRF0)();
        typedef bool*       (*&PRF1)(int);
        typedef bool&       (*&PRF2)(int, int);
        typedef bool const& (*&PRF3)(int, int, int);
        typedef bool        (*&PRF4)(int, ...);
        test_predicate<RF0>();
        test_not_predicate<RF1, int>();
        test_predicate<RF2, int, long>();
        test_predicate<RF3, int, long, int>();
        test_predicate<RF4, int, float, void*>();
        test_predicate<PF0>();
        test_not_predicate<PF1, int>();
        test_predicate<PF2, int, long>();
        test_predicate<PF3, int, long, int>();
        test_predicate<PF4, int, float, void*>();
        test_predicate<PRF0>();
        test_not_predicate<PRF1, int>();
        test_predicate<PRF2, int, long>();
        test_predicate<PRF3, int, long, int>();
        test_predicate<PRF4, int, float, void*>();
    }

    { // pointer to member function
        typedef Bool         (S::*PMS0)();
        typedef Bool         (S::*PMS1)(long);
        typedef Bool&        (S::*PMS2)(long, int);
        typedef Bool const&  (S::*PMS3)(int, ...);
        test_predicate<PMS0,                              S>();
        test_predicate<PMS0,                              S&>();
        test_predicate<PMS0,                              S*>();
        test_predicate<PMS0,                              S*&>();
        test_predicate<PMS0,       std::reference_wrapper<S>>();
        test_predicate<PMS0, const std::reference_wrapper<S>&>();
        test_predicate<PMS0,       std::reference_wrapper<SD>>();
        test_predicate<PMS0, const std::reference_wrapper<SD>&>();
        test_predicate<PMS0, std::unique_ptr<S>>();
        test_predicate<PMS0, std::unique_ptr<SD>>();
        test_not_predicate<PMS0, const          S&>();
        test_not_predicate<PMS0, volatile       S&>();
        test_not_predicate<PMS0, const volatile S&>();
        test_not_predicate<PMS0, ND &                           >();
        test_not_predicate<PMS0, const ND&                      >();
        test_not_predicate<PMS0, std::unique_ptr<S const>       >();
        test_not_predicate<PMS0, std::reference_wrapper<S const>>();
        test_not_predicate<PMS0, std::reference_wrapper<ND>     >();
        test_not_predicate<PMS0, std::unique_ptr<ND>            >();

        test_predicate<PMS1,                              S,   int>();
        test_predicate<PMS1,                              S&,  int>();
        test_predicate<PMS1,                              S*,  int>();
        test_predicate<PMS1,                              S*&, int>();
        test_predicate<PMS1, std::unique_ptr<S>,               int>();
        test_predicate<PMS1, std::unique_ptr<SD>,              int>();
        test_predicate<PMS1, std::reference_wrapper<S>,        int>();
        test_predicate<PMS1, const std::reference_wrapper<S>&, int>();
        test_predicate<PMS1, std::reference_wrapper<SD>,        int>();
        test_predicate<PMS1, const std::reference_wrapper<SD>&, int>();
        test_not_predicate<PMS1, const          S&, int>();
        test_not_predicate<PMS1, volatile       S&, int>();
        test_not_predicate<PMS1, const volatile S&, int>();
        test_not_predicate<PMS1, ND &,                            int>();
        test_not_predicate<PMS1, const ND&,                       int>();
        test_not_predicate<PMS1, std::unique_ptr<S const>,        int>();
        test_not_predicate<PMS1, std::reference_wrapper<S const>, int>();
        test_not_predicate<PMS1, std::reference_wrapper<ND>,      int>();
        test_not_predicate<PMS1, std::unique_ptr<ND>,             int>();

        test_predicate<PMS2,                S,   int, int>();
        test_predicate<PMS2,                S&,  int, int>();
        test_predicate<PMS2,                S*,  int, int>();
        test_predicate<PMS2,                S*&, int, int>();
        test_predicate<PMS2, std::unique_ptr<S>, int, int>();
        test_predicate<PMS2, std::unique_ptr<SD>, int, int>();
        test_predicate<PMS2, std::reference_wrapper<S>,         int, int>();
        test_predicate<PMS2, const std::reference_wrapper<S>&,  int, int>();
        test_predicate<PMS2, std::reference_wrapper<SD>,        int, int>();
        test_predicate<PMS2, const std::reference_wrapper<SD>&, int, int>();
        test_not_predicate<PMS2, const          S&, int, int>();
        test_not_predicate<PMS2, volatile       S&, int, int>();
        test_not_predicate<PMS2, const volatile S&, int, int>();
        test_not_predicate<PMS2, std::unique_ptr<S const>,   int, int>();
        test_not_predicate<PMS2, std::reference_wrapper<S const>, int, int>();
        test_not_predicate<PMS2, const ND&,                  int, int>();
        test_not_predicate<PMS2, std::reference_wrapper<ND>, int, int>();
        test_not_predicate<PMS2, std::unique_ptr<ND>,        int, int>();

        test_predicate<PMS3, S&, int>();
        test_predicate<PMS3, S&, int, long>();

        typedef bool        (S::*PMS0C)() const;
        typedef bool        (S::*PMS1C)(long) const;
        typedef bool        (S::*PMS2C)(long, int) const;
        typedef const bool& (S::*PMS3C)(int, ...) const;
        test_predicate<PMS0C,                S>();
        test_predicate<PMS0C,                S&>();
        test_predicate<PMS0C, const          S&>();
        test_predicate<PMS0C,                S*>();
        test_predicate<PMS0C, const          S*>();
        test_predicate<PMS0C,                S*&>();
        test_predicate<PMS0C, const          S*&>();
        test_predicate<PMS0C, std::unique_ptr<S>>();
        test_predicate<PMS0C, std::unique_ptr<SD>>();
        test_predicate<PMS0C, std::reference_wrapper<S>              >();
        test_predicate<PMS0C, std::reference_wrapper<const S>        >();
        test_predicate<PMS0C, const std::reference_wrapper<S> &      >();
        test_predicate<PMS0C, const std::reference_wrapper<const S> &>();
        test_predicate<PMS0C, std::reference_wrapper<SD>             >();
        test_predicate<PMS0C, std::reference_wrapper<const SD>       >();
        test_predicate<PMS0C, const std::reference_wrapper<SD> &     >();
        test_predicate<PMS0C, const std::reference_wrapper<const SD> &>();
        test_not_predicate<PMS0C, volatile       S&>();
        test_not_predicate<PMS0C, const volatile S&>();

        test_predicate<PMS1C,                S,   int>();
        test_predicate<PMS1C,                S&,  int>();
        test_predicate<PMS1C, const          S&,  int>();
        test_predicate<PMS1C,                S*,  int>();
        test_predicate<PMS1C, const          S*,  int>();
        test_predicate<PMS1C,                S*&, int>();
        test_predicate<PMS1C, const          S*&, int>();
        test_predicate<PMS1C, std::unique_ptr<S>, int>();
        test_not_predicate<PMS1C, volatile       S&, int>();
        test_not_predicate<PMS1C, const volatile S&, int>();

        test_predicate<PMS2C,                S,   int, int>();
        test_predicate<PMS2C,                S&,  int, int>();
        test_predicate<PMS2C, const          S&,  int, int>();
        test_predicate<PMS2C,                S*,  int, int>();
        test_predicate<PMS2C, const          S*,  int, int>();
        test_predicate<PMS2C,                S*&, int, int>();
        test_predicate<PMS2C, const          S*&, int, int>();
        test_predicate<PMS2C, std::unique_ptr<S>, int, int>();
        test_not_predicate<PMS2C, volatile       S&, int, int>();
        test_not_predicate<PMS2C, const volatile S&, int, int>();

        test_predicate<PMS3C, S&, int>();
        test_predicate<PMS3C, S&, int, long>();

        typedef bool        (S::*PMS0V)() volatile;
        typedef bool        (S::*PMS1V)(long) volatile;
        typedef bool        (S::*PMS2V)(long, int) volatile;
        typedef const bool& (S::*PMS3V)(int, ...) volatile;
        test_predicate<PMS0V,                S>();
        test_predicate<PMS0V,                S&>();
        test_predicate<PMS0V, volatile       S&>();
        test_predicate<PMS0V,                S*>();
        test_predicate<PMS0V, volatile       S*>();
        test_predicate<PMS0V,                S*&>();
        test_predicate<PMS0V, volatile       S*&>();
        test_predicate<PMS0V, std::unique_ptr<S>>();
        test_not_predicate<PMS0V, const          S&>();
        test_not_predicate<PMS0V, const volatile S&>();

        test_predicate<PMS1V,                S,   int>();
        test_predicate<PMS1V,                S&,  int>();
        test_predicate<PMS1V, volatile       S&,  int>();
        test_predicate<PMS1V,                S*,  int>();
        test_predicate<PMS1V, volatile       S*,  int>();
        test_predicate<PMS1V,                S*&, int>();
        test_predicate<PMS1V, volatile       S*&, int>();
        test_predicate<PMS1V, std::unique_ptr<S>, int>();
        test_not_predicate<PMS1V, const          S&, int>();
        test_not_predicate<PMS1V, const volatile S&, int>();

        test_predicate<PMS2V,                S,   int, int>();
        test_predicate<PMS2V,                S&,  int, int>();
        test_predicate<PMS2V, volatile       S&,  int, int>();
        test_predicate<PMS2V,                S*,  int, int>();
        test_predicate<PMS2V, volatile       S*,  int, int>();
        test_predicate<PMS2V,                S*&, int, int>();
        test_predicate<PMS2V, volatile       S*&, int, int>();
        test_predicate<PMS2V, std::unique_ptr<S>, int, int>();
        test_not_predicate<PMS2V, const          S&, int, int>();
        test_not_predicate<PMS2V, const volatile S&, int, int>();

        test_predicate<PMS3V, S&, int>();
        test_predicate<PMS3V, S&, int, long>();

        typedef Bool        (S::*PMS0CV)() const volatile;
        typedef Bool        (S::*PMS1CV)(long) const volatile;
        typedef Bool        (S::*PMS2CV)(long, int) const volatile;
        typedef const Bool& (S::*PMS3CV)(int, ...) const volatile;
        test_predicate<PMS0CV,                S>();
        test_predicate<PMS0CV,                S&>();
        test_predicate<PMS0CV, const          S&>();
        test_predicate<PMS0CV, volatile       S&>();
        test_predicate<PMS0CV, const volatile S&>();
        test_predicate<PMS0CV,                S*>();
        test_predicate<PMS0CV, const          S*>();
        test_predicate<PMS0CV, volatile       S*>();
        test_predicate<PMS0CV, const volatile S*>();
        test_predicate<PMS0CV,                S*&>();
        test_predicate<PMS0CV, const          S*&>();
        test_predicate<PMS0CV, volatile       S*&>();
        test_predicate<PMS0CV, const volatile S*&>();
        test_predicate<PMS0CV, std::unique_ptr<S>>();

        test_predicate<PMS1CV,                S,   int>();
        test_predicate<PMS1CV,                S&,  int>();
        test_predicate<PMS1CV, const          S&,  int>();
        test_predicate<PMS1CV, volatile       S&,  int>();
        test_predicate<PMS1CV, const volatile S&,  int>();
        test_predicate<PMS1CV,                S*,  int>();
        test_predicate<PMS1CV, const          S*,  int>();
        test_predicate<PMS1CV, volatile       S*,  int>();
        test_predicate<PMS1CV, const volatile S*,  int>();
        test_predicate<PMS1CV,                S*&, int>();
        test_predicate<PMS1CV, const          S*&, int>();
        test_predicate<PMS1CV, volatile       S*&, int>();
        test_predicate<PMS1CV, const volatile S*&, int>();
        test_predicate<PMS1CV, std::unique_ptr<S>, int>();

        test_predicate<PMS2CV,                S,   int, int>();
        test_predicate<PMS2CV,                S&,  int, int>();
        test_predicate<PMS2CV, const          S&,  int, int>();
        test_predicate<PMS2CV, volatile       S&,  int, int>();
        test_predicate<PMS2CV, const volatile S&,  int, int>();
        test_predicate<PMS2CV,                S*,  int, int>();
        test_predicate<PMS2CV, const          S*,  int, int>();
        test_predicate<PMS2CV, volatile       S*,  int, int>();
        test_predicate<PMS2CV, const volatile S*,  int, int>();
        test_predicate<PMS2CV,                S*&, int, int>();
        test_predicate<PMS2CV, const          S*&, int, int>();
        test_predicate<PMS2CV, volatile       S*&, int, int>();
        test_predicate<PMS2CV, const volatile S*&, int, int>();
        test_predicate<PMS2CV, std::unique_ptr<S>, int, int>();

        test_predicate<PMS3CV, S&, int>();
        test_predicate<PMS3CV, S&, int, long>();
    }

    { // pointer to member data
        typedef bool S::*PMD;
        test_predicate<PMD, S&>();
        test_predicate<PMD, S*>();
        test_predicate<PMD, S* const>();
        test_predicate<PMD, const S&>();
        test_predicate<PMD, const S*>();
        test_predicate<PMD, volatile S&>();
        test_predicate<PMD, volatile S*>();
        test_predicate<PMD, const volatile S&>();
        test_predicate<PMD, const volatile S*>();
        test_predicate<PMD, SD&>();
        test_predicate<PMD, SD const&>();
        test_predicate<PMD, SD*>();
        test_predicate<PMD, const SD*>();
        test_predicate<PMD, std::unique_ptr<S>>();
        test_predicate<PMD, std::unique_ptr<S const>>();
        test_predicate<PMD, std::reference_wrapper<S>>();
        test_predicate<PMD, std::reference_wrapper<S const>>();
        test_not_predicate<PMD, ND&>();
    }
}
