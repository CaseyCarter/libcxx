// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//  Copyright Eric Niebler 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17

#include <concepts>

struct copyable {};

struct moveonly {
    moveonly() = default;
    moveonly(moveonly&&) = default;
    moveonly& operator=(moveonly&&) = default;
};

struct copyonly { // Abomination!
    copyonly() = default;
    copyonly(const copyonly&) = default;
    copyonly& operator=(const copyonly&) = default;
    copyonly(copyonly&&) = delete;
    copyonly& operator=(copyonly&&) = delete;
};

struct nonmovable {
    nonmovable() = default;
    nonmovable(nonmovable&&) = delete;
};

struct nondefaultconstructible {
    nondefaultconstructible(int) {}
};

struct indestructible {
    ~indestructible() = delete;
};

struct throwing_destructor {
    ~throwing_destructor() noexcept(false);
};

struct explicit_default {
    explicit explicit_default() {}
};

struct deleted_default {
    deleted_default() = delete;
};

struct explicit_move {
    explicit_move() = default;
    explicit explicit_move(explicit_move&&) = default;
};

struct explicit_copy {
    explicit_copy() = default;
    explicit_copy(explicit_copy&&) = default;
    explicit explicit_copy(const explicit_copy&) = default;
};

struct semiregular {};

struct regular {
    friend constexpr bool operator==(const regular&, const regular&) {
        return true;
    }
    friend constexpr bool operator!=(const regular&, const regular&) {
        return false;
    }
};

struct XXX
{
    XXX() = default;
    XXX(XXX&&) = delete;
    explicit XXX(int) {}
};

template <class T>
void test_movable() {
    using std::Movable;

    static_assert(Movable<T>);
    static_assert(!Movable<const T>);
    static_assert(Movable<volatile T> == std::is_scalar_v<T>);
    static_assert(!Movable<const volatile T>);
}

template <class T>
void test_not_movable() {
    using std::Movable;

    static_assert(!Movable<T>);
    static_assert(!Movable<const T>);
    static_assert(!Movable<volatile T>);
    static_assert(!Movable<const volatile T>);
}

template <class T>
void test_copyable() {
    using std::Copyable;

    static_assert(Copyable<T>);
    static_assert(!Copyable<const T>);
    static_assert(Copyable<volatile T> == std::is_scalar_v<T>);
    static_assert(!Copyable<const volatile T>);
}

template <class T>
void test_not_copyable() {
    using std::Copyable;

    static_assert(!Copyable<T>);
    static_assert(!Copyable<const T>);
    static_assert(!Copyable<volatile T>);
    static_assert(!Copyable<const volatile T>);
}

template <class T>
void test_semiregular() {
    using std::Semiregular;

    static_assert(Semiregular<T>);
    static_assert(!Semiregular<const T>);
    static_assert(Semiregular<volatile T> == std::is_scalar_v<T>);
    static_assert(!Semiregular<const volatile T>);
}

template <class T>
void test_not_semiregular() {
    using std::Semiregular;

    static_assert(!Semiregular<T>);
    static_assert(!Semiregular<const T>);
    static_assert(!Semiregular<volatile T>);
    static_assert(!Semiregular<const volatile T>);
}

template <class T>
void test_regular() {
    using std::Regular;

    static_assert(Regular<T>);
    static_assert(!Regular<const T>);
    static_assert(Regular<volatile T> == std::is_scalar_v<T>);
    static_assert(!Regular<const volatile T>);
}

template <class T>
void test_not_regular() {
    using std::Regular;

    static_assert(!Regular<T>);
    static_assert(!Regular<const T>);
    static_assert(!Regular<volatile T>);
    static_assert(!Regular<const volatile T>);
}

int main() {
    test_movable<int>();
    test_movable<double>();
    test_movable<copyable>();
    test_movable<moveonly>();
    test_not_movable<copyonly>();
    test_not_movable<nonmovable>();

    test_not_movable<void>();
    test_not_movable<int&>();
    test_not_movable<int&&>(); // https://github.com/ericniebler/stl2/issues/310
    test_not_movable<int(int)>();
    test_not_movable<int[42]>();

    test_copyable<int>();
    test_copyable<double>();
    test_copyable<copyable>();
    test_not_copyable<moveonly>();
    test_not_copyable<copyonly>();
    test_not_copyable<nonmovable>();

    test_not_copyable<void>();
    test_not_copyable<int&>();
    test_not_copyable<int&&>();
    test_not_copyable<int(int)>();
    test_not_copyable<int[42]>();

    test_semiregular<int>();
    test_semiregular<double>();
    test_not_semiregular<void>();
    test_not_semiregular<int&>();
    test_semiregular<semiregular>(); // Whew.
    test_semiregular<regular>();
    test_semiregular<copyable>();
    test_not_semiregular<moveonly>();
    test_not_semiregular<copyonly>();
    test_not_semiregular<nonmovable>();
    test_not_semiregular<explicit_move>();
    test_not_semiregular<explicit_copy>();

    test_regular<int>();
    test_regular<double>();
    test_not_regular<void>();
    test_not_regular<int&>();
    test_not_regular<semiregular>();
    test_regular<regular>();
    test_not_regular<copyable>();
    test_not_regular<moveonly>();
    test_not_regular<copyonly>();
    test_not_regular<nonmovable>();
    test_not_regular<explicit_move>();
    test_not_regular<explicit_copy>();
}
