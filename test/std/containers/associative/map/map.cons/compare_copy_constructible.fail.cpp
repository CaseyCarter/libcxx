//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <map>

// Check that std::map fails to instantiate if the comparison predicate is
// not copy-constructible. This is LWG issue 2436

#include <map>

template <class T>
struct Comp {
    bool operator () (const T& lhs, const T& rhs) const { return lhs < rhs; }

    Comp () {}
private:
#if !defined(__clang__) && defined(__cpp_concepts) && defined(__GNUC__)
    // Avoid triggering https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67225.
    Comp (const Comp &) = delete;
#else
    Comp (const Comp &); // declared but not defined
#endif
    };


int main() {
    std::map<int, int, Comp<int> > m;
}
