//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef EMPLACEABLE_H
#define EMPLACEABLE_H

#include <utility>
#include "test_macros.h"

#if TEST_STD_VER >= 11

class Emplaceable
{
#if defined(__clang__) || !(defined(__cpp_concepts) && defined(__GNUC__))
    Emplaceable(const Emplaceable&);
    Emplaceable& operator=(const Emplaceable&);
#else
    // Avoid triggering https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67225.
    Emplaceable(const Emplaceable&) = delete;
    Emplaceable& operator=(const Emplaceable&) = delete;
#endif

    int int_;
    double double_;
public:
    Emplaceable() : int_(0), double_(0) {}
    Emplaceable(int i, double d) : int_(i), double_(d) {}
    Emplaceable(Emplaceable&& x)
        : int_(x.int_), double_(x.double_)
            {x.int_ = 0; x.double_ = 0;}
    Emplaceable& operator=(Emplaceable&& x)
        {int_ = x.int_; x.int_ = 0;
         double_ = x.double_; x.double_ = 0;
         return *this;}

    bool operator==(const Emplaceable& x) const
        {return int_ == x.int_ && double_ == x.double_;}
    bool operator<(const Emplaceable& x) const
        {return int_ < x.int_ || (int_ == x.int_ && double_ < x.double_);}

    int get() const {return int_;}
};

namespace std {

template <>
struct hash<Emplaceable>
{
    typedef Emplaceable argument_type;
    typedef std::size_t result_type;

    std::size_t operator()(const Emplaceable& x) const {return x.get();}
};

}

#endif  // TEST_STD_VER >= 11
#endif  // EMPLACEABLE_H
