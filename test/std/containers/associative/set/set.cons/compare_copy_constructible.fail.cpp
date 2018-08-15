//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <set>

// Check that std::set fails to instantiate if the comparison predicate is
// not copy-constructible. This is LWG issue 2436

#include <set>

template <class T>
struct Comp {
    bool operator () (const T& lhs, const T& rhs) const { return lhs < rhs; }

    Comp () {}
private:
#if defined(__clang__) || !(defined(__cpp_concepts) && defined(__GNUC__))
    Comp (const Comp &); // declared but not defined
#else
    // Avoid triggering https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67225.
    Comp (const Comp &) = delete;
#endif
    };


int main() {
    std::set<int, Comp<int> > m;
}
