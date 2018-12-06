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

// <utility>

// namespace ranges { inline namespace unspecified {
//   inline constexpr unspecified swap = unspecified;
// }}

#include <utility>
#include <cassert>
#include <memory>

#include "test_macros.h"

namespace ranges = std::ranges;

struct CopyOnly {
    CopyOnly() {}
    CopyOnly(CopyOnly const&) noexcept {}
    CopyOnly& operator=(CopyOnly const&) { return *this; }
};

struct MoveOnly {
    MoveOnly() {}
    MoveOnly(MoveOnly&&) {}
    MoveOnly& operator=(MoveOnly&&) noexcept { return *this; }
};

struct NoexceptMoveOnly {
    NoexceptMoveOnly() {}
    NoexceptMoveOnly(NoexceptMoveOnly&&) noexcept {}
    NoexceptMoveOnly& operator=(NoexceptMoveOnly&&) noexcept { return *this; }
};

struct NotMoveConstructible {
    NotMoveConstructible() {}
    NotMoveConstructible& operator=(NotMoveConstructible&&) { return *this; }
private:
#if !defined(__clang__) && defined(__cpp_concepts) && defined(__GNUC__)
    // Avoid triggering https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67225.
    NotMoveConstructible(NotMoveConstructible&&) = delete;
#else
    NotMoveConstructible(NotMoveConstructible&&);
#endif
};

struct NotMoveAssignable {
    NotMoveAssignable(NotMoveAssignable&&);
private:
#if defined(__GNUC__) && !defined(__clang__) && defined(__cpp_concepts)
    // Avoid triggering https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67225.
    NotMoveAssignable& operator=(NotMoveAssignable&&) = delete;
#else
    NotMoveAssignable& operator=(NotMoveAssignable&&);
#endif
};

struct ImmobileNoexceptSwap {
    ImmobileNoexceptSwap() = default;
    ImmobileNoexceptSwap(ImmobileNoexceptSwap&&) = delete;
    ImmobileNoexceptSwap& operator=(ImmobileNoexceptSwap&&) = delete;
    friend void swap(ImmobileNoexceptSwap&, ImmobileNoexceptSwap&) noexcept {}
};

struct ThrowingSwap {
    friend void swap(ThrowingSwap&, ThrowingSwap&) {}
};

struct Unswappable : std::pair<int, int> { // Derives from type in std with overloaded swap
    Unswappable() = default;
    Unswappable(const Unswappable&) = delete;
    Unswappable(Unswappable&&) = delete;
};

namespace constrained_swappable_ {
    // Has a constrained swap findable via ADL:
    struct ConstrainedSwappable {
        ConstrainedSwappable() = default;
        ConstrainedSwappable(const ConstrainedSwappable&) = default;
        ConstrainedSwappable(ConstrainedSwappable&&) = default;
    };
    template <class T>
    _STL_CONCEPT_DECL C =
        std::Same<T, ConstrainedSwappable>;
    template <C T, C U>
    void swap(T&, U&) {}
    template <C T>
    void swap(T&, T&) {}
}
using constrained_swappable_::ConstrainedSwappable;

template <class>
constexpr bool can_swap = false;
template <class T>
    requires requires {
        ranges::swap(std::declval<T>(), std::declval<T>());
    }
constexpr bool can_swap<T> = true;

int main()
{
    // TODO: constexpr, program-defined types, cross-type

    // Non-array swap
    {
        int i = 1;
        int j = 2;
        ranges::swap(i, j);
        assert(i == 2);
        assert(j == 1);
    }
    {

        std::unique_ptr<int> i(new int(1));
        std::unique_ptr<int> j(new int(2));
        ranges::swap(i, j);
        assert(*i == 2);
        assert(*j == 1);
    }
    {
        static_assert(can_swap<CopyOnly&>);
        static_assert(can_swap<MoveOnly&>);
        static_assert(can_swap<NoexceptMoveOnly&>);

        static_assert(!can_swap<NotMoveConstructible&>);
        static_assert(!can_swap<NotMoveAssignable&>);

        CopyOnly c;
        MoveOnly m;
        NoexceptMoveOnly nm;
        static_assert(!noexcept(ranges::swap(c, c)));
        static_assert(!noexcept(ranges::swap(m, m)));
        static_assert(noexcept(ranges::swap(nm, nm)));
    }

    // Array swap
    {
        int i[3] = {1, 2, 3};
        int j[3] = {4, 5, 6};
        static_assert(noexcept(ranges::swap(i, j)));
        ranges::swap(i, j);
        assert(i[0] == 4);
        assert(i[1] == 5);
        assert(i[2] == 6);
        assert(j[0] == 1);
        assert(j[1] == 2);
        assert(j[2] == 3);
    }
    {
        std::unique_ptr<int> i[3];
        for (int k = 0; k < 3; ++k)
            i[k].reset(new int(k+1));
        std::unique_ptr<int> j[3];
        for (int k = 0; k < 3; ++k)
            j[k].reset(new int(k+4));
        static_assert(noexcept(ranges::swap(i, j)) == noexcept(ranges::swap(i[0], j[0])));
        ranges::swap(i, j);
        assert(*i[0] == 4);
        assert(*i[1] == 5);
        assert(*i[2] == 6);
        assert(*j[0] == 1);
        assert(*j[1] == 2);
        assert(*j[2] == 3);
    }
    {
        using CA = CopyOnly[42];
        using MA = NoexceptMoveOnly[42];
        using NA = NotMoveConstructible[42];
        static_assert(can_swap<CA&>);
        static_assert(can_swap<MA&>);
        static_assert(!can_swap<NA&>);

        CopyOnly c;
        CA ca;
        NoexceptMoveOnly m;
        MA ma;
        static_assert(!noexcept(ranges::swap(ca, ca)));
        static_assert(noexcept(ranges::swap(ma, ma)));

        static_assert(noexcept(ranges::swap(ca, ca)) == noexcept(ranges::swap(c, c)));
        static_assert(noexcept(ranges::swap(ma, ma)) == noexcept(ranges::swap(m, m)));
    }
    {
        // Multidimensional arrays? Sure.
        int i[2][3][2] = {
            { {0, 1}, {2, 3}, { 4,  5} },
            { {6, 7}, {8, 9}, {10, 11} },
        };
        int j[2][3][2] = {
            { {12, 13}, {14, 15}, {16, 17} },
            { {18, 19}, {20, 21}, {22, 23} },
        };
        static_assert(noexcept(ranges::swap(i, j)));
        ranges::swap(i, j);
        assert(i[0][0][0] == 12);
        assert(i[0][0][1] == 13);
        assert(i[0][1][0] == 14);
        assert(i[0][1][1] == 15);
        assert(i[0][2][0] == 16);
        assert(i[0][2][1] == 17);
        assert(i[1][0][0] == 18);
        assert(i[1][0][1] == 19);
        assert(i[1][1][0] == 20);
        assert(i[1][1][1] == 21);
        assert(i[1][2][0] == 22);
        assert(i[1][2][1] == 23);
        assert(j[0][0][0] == 0);
        assert(j[0][0][1] == 1);
        assert(j[0][1][0] == 2);
        assert(j[0][1][1] == 3);
        assert(j[0][2][0] == 4);
        assert(j[0][2][1] == 5);
        assert(j[1][0][0] == 6);
        assert(j[1][0][1] == 7);
        assert(j[1][1][0] == 8);
        assert(j[1][1][1] == 9);
        assert(j[1][2][0] == 10);
        assert(j[1][2][1] == 11);
    }

    {
        ImmobileNoexceptSwap x;
        static_assert(noexcept(ranges::swap(x, x)));
    }
    {
        ThrowingSwap x;
        static_assert(!noexcept(ranges::swap(x, x)));
    }

    static_assert(!can_swap<Unswappable&>);
    static_assert(can_swap<ConstrainedSwappable&>);
    static_assert(!can_swap<const volatile ConstrainedSwappable&>);
}
