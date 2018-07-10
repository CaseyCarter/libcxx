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

struct Tag {};
struct DerFromTag : Tag {};

struct Implicit {
    Implicit(int) {}
};

struct Explicit {
    explicit Explicit(int) {}
};

struct NotCallableWithInt {
    int operator()(int) = delete;
    int operator()(Tag) { return 42; }
};

struct S
{
    typedef short (*FreeFunc)(long);
    operator FreeFunc() const;
    double operator()(char, int&);
    double const& operator()(char, int&) const;
    double volatile& operator()(char, int&) volatile;
    double const volatile& operator()(char, int&) const volatile;
};


struct SD : public S { };

struct NotDerived {};

template <class F, class... Args>
void test_invocable() {
    static_assert(std::Invocable<F, Args...>);
    static_assert(std::RegularInvocable<F, Args...>);
}

template <class F, class... Args>
void test_not_invocable() {
    static_assert(!std::Invocable<F, Args...>);
    static_assert(!std::RegularInvocable<F, Args...>);
}

int main() {
    {
        using Fn = int(Tag::*)(int);
        using RFn = int(Tag::*)(int) &&;
        // INVOKE bullet 1, 2 and 3
        {
            // Bullet 1
            test_invocable<Fn, Tag&, int>();
            test_invocable<Fn, DerFromTag&, int>();
            test_invocable<RFn, Tag&&, int>();
            test_not_invocable<RFn, Tag&, int>();
            test_not_invocable<Fn, Tag&>();
            test_not_invocable<Fn, Tag const&, int>();
        }
        {
            // Bullet 2
            using T = std::reference_wrapper<Tag>;
            using DT = std::reference_wrapper<DerFromTag>;
            using CT = std::reference_wrapper<const Tag>;
            test_invocable<Fn, T&, int>();
            test_invocable<Fn, DT&, int>();
            test_invocable<Fn, const T&, int>();
            test_invocable<Fn, T&&, int>();
            test_not_invocable<Fn, CT&, int>();
            test_not_invocable<RFn, T, int>();
        }
        {
            // Bullet 3
            using T = Tag*;
            using DT = DerFromTag*;
            using CT = const Tag*;
            using ST = std::unique_ptr<Tag>;
            test_invocable<Fn, T&, int>();
            test_invocable<Fn, DT&, int>();
            test_invocable<Fn, const T&, int>();
            test_invocable<Fn, T&&, int>();
            test_invocable<Fn, ST, int>();
            test_not_invocable<Fn, CT&, int>();
            test_not_invocable<RFn, T, int>();
        }
    }
    {
        // Bullets 4, 5 and 6
        using Fn = int (Tag::*);
        test_not_invocable<Fn>();
        {
            // Bullet 4
            test_invocable<Fn, Tag&>();
            test_invocable<Fn, DerFromTag&>();
            test_invocable<Fn, Tag&&>();
            test_invocable<Fn, Tag const&>();
        }
        {
            // Bullet 5
            using T = std::reference_wrapper<Tag>;
            using DT = std::reference_wrapper<DerFromTag>;
            using CT = std::reference_wrapper<const Tag>;
            test_invocable<Fn, T&>();
            test_invocable<Fn, DT&>();
            test_invocable<Fn, const T&>();
            test_invocable<Fn, T&&>();
            test_invocable<Fn, CT&>();
        }
        {
            // Bullet 6
            using T = Tag*;
            using DT = DerFromTag*;
            using CT = const Tag*;
            using ST = std::unique_ptr<Tag>;
            test_invocable<Fn, T&>();
            test_invocable<Fn, DT&>();
            test_invocable<Fn, const T&>();
            test_invocable<Fn, T&&>();
            test_invocable<Fn, ST>();
            test_invocable<Fn, CT&>();
        }
    }
    {
        // INVOKE bullet 7
        {
            // Function pointer
            using Fp = void(*)(Tag&, int);
            test_invocable<Fp, Tag&, int>();
            test_invocable<Fp, DerFromTag&, int>();
            test_not_invocable<Fp, const Tag&, int>();
            test_not_invocable<Fp>();
            test_not_invocable<Fp, Tag&>();
        }
        {
            // Function reference
            using Fp = void(&)(Tag&, int);
            test_invocable<Fp, Tag&, int>();
            test_invocable<Fp, DerFromTag&, int>();
            test_not_invocable<Fp, const Tag&, int>();
            test_not_invocable<Fp>();
            test_not_invocable<Fp, Tag&>();
        }
        {
            // Function object
            using Fn = NotCallableWithInt;
            test_invocable<Fn, Tag>();
            test_not_invocable<Fn, int>();
        }
    }

    typedef NotDerived ND;
    { // functor object
        test_invocable<S, int>();
        test_invocable<S&, unsigned char, int&>();
        test_invocable<S const&, unsigned char, int&>();
        test_invocable<S volatile&, unsigned char, int&>();
        test_invocable<S const volatile&, unsigned char, int&>();
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
        test_invocable<RF0>();
        test_invocable<RF1, int>();
        test_invocable<RF2, int, long>();
        test_invocable<RF3, int, long, int>();
        test_invocable<RF4, int, float, void*>();
        test_invocable<PF0>();
        test_invocable<PF1, int>();
        test_invocable<PF2, int, long>();
        test_invocable<PF3, int, long, int>();
        test_invocable<PF4, int, float, void*>();
        test_invocable<PRF0>();
        test_invocable<PRF1, int>();
        test_invocable<PRF2, int, long>();
        test_invocable<PRF3, int, long, int>();
        test_invocable<PRF4, int, float, void*>();
    }

    { // pointer to member function
        typedef int         (S::*PMS0)();
        typedef int*        (S::*PMS1)(long);
        typedef int&        (S::*PMS2)(long, int);
        typedef const int&  (S::*PMS3)(int, ...);
        test_invocable<PMS0,                              S>();
        test_invocable<PMS0,                              S&>();
        test_invocable<PMS0,                              S*>();
        test_invocable<PMS0,                              S*&>();
        test_invocable<PMS0,       std::reference_wrapper<S>>();
        test_invocable<PMS0, const std::reference_wrapper<S>&>();
        test_invocable<PMS0,       std::reference_wrapper<SD>>();
        test_invocable<PMS0, const std::reference_wrapper<SD>&>();
        test_invocable<PMS0, std::unique_ptr<S>>();
        test_invocable<PMS0, std::unique_ptr<SD>>();
        test_not_invocable<PMS0, const          S&>();
        test_not_invocable<PMS0, volatile       S&>();
        test_not_invocable<PMS0, const volatile S&>();
        test_not_invocable<PMS0, ND &                           >();
        test_not_invocable<PMS0, const ND&                      >();
        test_not_invocable<PMS0, std::unique_ptr<S const>       >();
        test_not_invocable<PMS0, std::reference_wrapper<S const>>();
        test_not_invocable<PMS0, std::reference_wrapper<ND>     >();
        test_not_invocable<PMS0, std::unique_ptr<ND>            >();

        test_invocable<PMS1,                              S,   int>();
        test_invocable<PMS1,                              S&,  int>();
        test_invocable<PMS1,                              S*,  int>();
        test_invocable<PMS1,                              S*&, int>();
        test_invocable<PMS1, std::unique_ptr<S>,               int>();
        test_invocable<PMS1, std::unique_ptr<SD>,              int>();
        test_invocable<PMS1, std::reference_wrapper<S>,        int>();
        test_invocable<PMS1, const std::reference_wrapper<S>&, int>();
        test_invocable<PMS1, std::reference_wrapper<SD>,        int>();
        test_invocable<PMS1, const std::reference_wrapper<SD>&, int>();
        test_not_invocable<PMS1, const          S&, int>();
        test_not_invocable<PMS1, volatile       S&, int>();
        test_not_invocable<PMS1, const volatile S&, int>();
        test_not_invocable<PMS1, ND &,                            int>();
        test_not_invocable<PMS1, const ND&,                       int>();
        test_not_invocable<PMS1, std::unique_ptr<S const>,        int>();
        test_not_invocable<PMS1, std::reference_wrapper<S const>, int>();
        test_not_invocable<PMS1, std::reference_wrapper<ND>,      int>();
        test_not_invocable<PMS1, std::unique_ptr<ND>,             int>();

        test_invocable<PMS2,                S,   int, int>();
        test_invocable<PMS2,                S&,  int, int>();
        test_invocable<PMS2,                S*,  int, int>();
        test_invocable<PMS2,                S*&, int, int>();
        test_invocable<PMS2, std::unique_ptr<S>, int, int>();
        test_invocable<PMS2, std::unique_ptr<SD>, int, int>();
        test_invocable<PMS2, std::reference_wrapper<S>,         int, int>();
        test_invocable<PMS2, const std::reference_wrapper<S>&,  int, int>();
        test_invocable<PMS2, std::reference_wrapper<SD>,        int, int>();
        test_invocable<PMS2, const std::reference_wrapper<SD>&, int, int>();
        test_not_invocable<PMS2, const          S&, int, int>();
        test_not_invocable<PMS2, volatile       S&, int, int>();
        test_not_invocable<PMS2, const volatile S&, int, int>();
        test_not_invocable<PMS2, std::unique_ptr<S const>,   int, int>();
        test_not_invocable<PMS2, std::reference_wrapper<S const>, int, int>();
        test_not_invocable<PMS2, const ND&,                  int, int>();
        test_not_invocable<PMS2, std::reference_wrapper<ND>, int, int>();
        test_not_invocable<PMS2, std::unique_ptr<ND>,        int, int>();

        test_invocable<PMS3, S&, int>();
        test_invocable<PMS3, S&, int, long>();

        typedef int        (S::*PMS0C)() const;
        typedef int*       (S::*PMS1C)(long) const;
        typedef int&       (S::*PMS2C)(long, int) const;
        typedef const int& (S::*PMS3C)(int, ...) const;
        test_invocable<PMS0C,                S>();
        test_invocable<PMS0C,                S&>();
        test_invocable<PMS0C, const          S&>();
        test_invocable<PMS0C,                S*>();
        test_invocable<PMS0C, const          S*>();
        test_invocable<PMS0C,                S*&>();
        test_invocable<PMS0C, const          S*&>();
        test_invocable<PMS0C, std::unique_ptr<S>>();
        test_invocable<PMS0C, std::unique_ptr<SD>>();
        test_invocable<PMS0C, std::reference_wrapper<S>              >();
        test_invocable<PMS0C, std::reference_wrapper<const S>        >();
        test_invocable<PMS0C, const std::reference_wrapper<S> &      >();
        test_invocable<PMS0C, const std::reference_wrapper<const S> &>();
        test_invocable<PMS0C, std::reference_wrapper<SD>             >();
        test_invocable<PMS0C, std::reference_wrapper<const SD>       >();
        test_invocable<PMS0C, const std::reference_wrapper<SD> &     >();
        test_invocable<PMS0C, const std::reference_wrapper<const SD> &>();
        test_not_invocable<PMS0C, volatile       S&>();
        test_not_invocable<PMS0C, const volatile S&>();

        test_invocable<PMS1C,                S,   int>();
        test_invocable<PMS1C,                S&,  int>();
        test_invocable<PMS1C, const          S&,  int>();
        test_invocable<PMS1C,                S*,  int>();
        test_invocable<PMS1C, const          S*,  int>();
        test_invocable<PMS1C,                S*&, int>();
        test_invocable<PMS1C, const          S*&, int>();
        test_invocable<PMS1C, std::unique_ptr<S>, int>();
        test_not_invocable<PMS1C, volatile       S&, int>();
        test_not_invocable<PMS1C, const volatile S&, int>();

        test_invocable<PMS2C,                S,   int, int>();
        test_invocable<PMS2C,                S&,  int, int>();
        test_invocable<PMS2C, const          S&,  int, int>();
        test_invocable<PMS2C,                S*,  int, int>();
        test_invocable<PMS2C, const          S*,  int, int>();
        test_invocable<PMS2C,                S*&, int, int>();
        test_invocable<PMS2C, const          S*&, int, int>();
        test_invocable<PMS2C, std::unique_ptr<S>, int, int>();
        test_not_invocable<PMS2C, volatile       S&, int, int>();
        test_not_invocable<PMS2C, const volatile S&, int, int>();

        test_invocable<PMS3C, S&, int>();
        test_invocable<PMS3C, S&, int, long>();

        typedef int       (S::*PMS0V)() volatile;
        typedef int*       (S::*PMS1V)(long) volatile;
        typedef int&       (S::*PMS2V)(long, int) volatile;
        typedef const int& (S::*PMS3V)(int, ...) volatile;
        test_invocable<PMS0V,                S>();
        test_invocable<PMS0V,                S&>();
        test_invocable<PMS0V, volatile       S&>();
        test_invocable<PMS0V,                S*>();
        test_invocable<PMS0V, volatile       S*>();
        test_invocable<PMS0V,                S*&>();
        test_invocable<PMS0V, volatile       S*&>();
        test_invocable<PMS0V, std::unique_ptr<S>>();
        test_not_invocable<PMS0V, const          S&>();
        test_not_invocable<PMS0V, const volatile S&>();

        test_invocable<PMS1V,                S,   int>();
        test_invocable<PMS1V,                S&,  int>();
        test_invocable<PMS1V, volatile       S&,  int>();
        test_invocable<PMS1V,                S*,  int>();
        test_invocable<PMS1V, volatile       S*,  int>();
        test_invocable<PMS1V,                S*&, int>();
        test_invocable<PMS1V, volatile       S*&, int>();
        test_invocable<PMS1V, std::unique_ptr<S>, int>();
        test_not_invocable<PMS1V, const          S&, int>();
        test_not_invocable<PMS1V, const volatile S&, int>();

        test_invocable<PMS2V,                S,   int, int>();
        test_invocable<PMS2V,                S&,  int, int>();
        test_invocable<PMS2V, volatile       S&,  int, int>();
        test_invocable<PMS2V,                S*,  int, int>();
        test_invocable<PMS2V, volatile       S*,  int, int>();
        test_invocable<PMS2V,                S*&, int, int>();
        test_invocable<PMS2V, volatile       S*&, int, int>();
        test_invocable<PMS2V, std::unique_ptr<S>, int, int>();
        test_not_invocable<PMS2V, const          S&, int, int>();
        test_not_invocable<PMS2V, const volatile S&, int, int>();

        test_invocable<PMS3V, S&, int>();
        test_invocable<PMS3V, S&, int, long>();

        typedef int        (S::*PMS0CV)() const volatile;
        typedef int*       (S::*PMS1CV)(long) const volatile;
        typedef int&       (S::*PMS2CV)(long, int) const volatile;
        typedef const int& (S::*PMS3CV)(int, ...) const volatile;
        test_invocable<PMS0CV,                S>();
        test_invocable<PMS0CV,                S&>();
        test_invocable<PMS0CV, const          S&>();
        test_invocable<PMS0CV, volatile       S&>();
        test_invocable<PMS0CV, const volatile S&>();
        test_invocable<PMS0CV,                S*>();
        test_invocable<PMS0CV, const          S*>();
        test_invocable<PMS0CV, volatile       S*>();
        test_invocable<PMS0CV, const volatile S*>();
        test_invocable<PMS0CV,                S*&>();
        test_invocable<PMS0CV, const          S*&>();
        test_invocable<PMS0CV, volatile       S*&>();
        test_invocable<PMS0CV, const volatile S*&>();
        test_invocable<PMS0CV, std::unique_ptr<S>>();

        test_invocable<PMS1CV,                S,   int>();
        test_invocable<PMS1CV,                S&,  int>();
        test_invocable<PMS1CV, const          S&,  int>();
        test_invocable<PMS1CV, volatile       S&,  int>();
        test_invocable<PMS1CV, const volatile S&,  int>();
        test_invocable<PMS1CV,                S*,  int>();
        test_invocable<PMS1CV, const          S*,  int>();
        test_invocable<PMS1CV, volatile       S*,  int>();
        test_invocable<PMS1CV, const volatile S*,  int>();
        test_invocable<PMS1CV,                S*&, int>();
        test_invocable<PMS1CV, const          S*&, int>();
        test_invocable<PMS1CV, volatile       S*&, int>();
        test_invocable<PMS1CV, const volatile S*&, int>();
        test_invocable<PMS1CV, std::unique_ptr<S>, int>();

        test_invocable<PMS2CV,                S,   int, int>();
        test_invocable<PMS2CV,                S&,  int, int>();
        test_invocable<PMS2CV, const          S&,  int, int>();
        test_invocable<PMS2CV, volatile       S&,  int, int>();
        test_invocable<PMS2CV, const volatile S&,  int, int>();
        test_invocable<PMS2CV,                S*,  int, int>();
        test_invocable<PMS2CV, const          S*,  int, int>();
        test_invocable<PMS2CV, volatile       S*,  int, int>();
        test_invocable<PMS2CV, const volatile S*,  int, int>();
        test_invocable<PMS2CV,                S*&, int, int>();
        test_invocable<PMS2CV, const          S*&, int, int>();
        test_invocable<PMS2CV, volatile       S*&, int, int>();
        test_invocable<PMS2CV, const volatile S*&, int, int>();
        test_invocable<PMS2CV, std::unique_ptr<S>, int, int>();

        test_invocable<PMS3CV, S&, int>();
        test_invocable<PMS3CV, S&, int, long>();
    }

    { // pointer to member data
        typedef char S::*PMD;
        test_invocable<PMD, S&>();
        test_invocable<PMD, S*>();
        test_invocable<PMD, S* const>();
        test_invocable<PMD, const S&>();
        test_invocable<PMD, const S*>();
        test_invocable<PMD, volatile S&>();
        test_invocable<PMD, volatile S*>();
        test_invocable<PMD, const volatile S&>();
        test_invocable<PMD, const volatile S*>();
        test_invocable<PMD, SD&>();
        test_invocable<PMD, SD const&>();
        test_invocable<PMD, SD*>();
        test_invocable<PMD, const SD*>();
        test_invocable<PMD, std::unique_ptr<S>>();
        test_invocable<PMD, std::unique_ptr<S const>>();
        test_invocable<PMD, std::reference_wrapper<S>>();
        test_invocable<PMD, std::reference_wrapper<S const>>();
        test_not_invocable<PMD, ND&>();
    }
}
