//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This test is expected to fail due to https://gcc.gnu.org/bugzilla/shobug.cgi?id=67225.
// REQUIRES: !gcc || !concepts

// <streambuf>

// template <class charT, class traits = char_traits<charT> >
// class basic_streambuf;

// basic_streambuf();  // is protected

#include <streambuf>

int main()
{
    std::basic_streambuf<char> sb;
}
