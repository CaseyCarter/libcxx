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

#include <iterator>
#include <type_traits>

template <class...> class show_type;

// FIXME: farm this out into individual component tests

namespace associated_type_test {
#if 0 // FIXME: Update this to the P1037 design
	template <class T>
	struct derive_from : T {};

	template <class T, bool Derive>
	using iterator =
		meta::apply<
			meta::bind_front<
				meta::quote<std::iterator>,
				meta::if_c<Derive, derive_from<T>, T>>,
			meta::if_<
				std::is_same<T, std::output_iterator_tag>,
				meta::list<void, void, void, void>,
				meta::list<int>>>;

	template <class T, bool B, class U>
	constexpr bool test = std::is_same_v<ranges::iterator_category_t<iterator<T, B>>, U>;

	static_assert(!is_trait<ranges::iterator_category<void*>>);
	static_assert(!is_trait<ranges::iterator_category<int(*)()>>);
	static_assert(!is_trait<ranges::iterator_category<iterator<std::output_iterator_tag, false>>>);
	static_assert(!is_trait<ranges::iterator_category<iterator<std::output_iterator_tag, true>>>);

	static_assert(test<std::input_iterator_tag, false, ranges::input_iterator_tag>);
	static_assert(test<std::forward_iterator_tag, false, ranges::forward_iterator_tag>);
	static_assert(test<std::bidirectional_iterator_tag, false, ranges::bidirectional_iterator_tag>);
	static_assert(test<std::random_access_iterator_tag, false, ranges::random_access_iterator_tag>);

	static_assert(test<std::input_iterator_tag, true, ranges::input_iterator_tag>);
	static_assert(test<std::forward_iterator_tag, true, ranges::forward_iterator_tag>);
	static_assert(test<std::bidirectional_iterator_tag, true, ranges::bidirectional_iterator_tag>);
	static_assert(test<std::random_access_iterator_tag, true, ranges::random_access_iterator_tag>);

	struct foo {};
	static_assert(test<foo, false, foo>);

	// Some sanity tests
	struct my_wonky_tag : std::random_access_iterator_tag, ranges::random_access_iterator_tag {};
	struct my_wonky_tag2 : std::input_iterator_tag, ranges::random_access_iterator_tag {};
	static_assert(std::is_same<ranges::detail::stl2_to_std_iterator_category<my_wonky_tag, int>, my_wonky_tag>::value, "");
	static_assert(std::is_same<ranges::detail::stl2_to_std_iterator_category<my_wonky_tag, int&>, my_wonky_tag>::value, "");
	static_assert(std::is_same<ranges::detail::stl2_to_std_iterator_category<my_wonky_tag2, int>, my_wonky_tag2>::value, "");
	static_assert(std::is_same<ranges::detail::stl2_to_std_iterator_category<my_wonky_tag2, int&>, my_wonky_tag2>::value, "");
	struct my_wonky_tag3 : ranges::random_access_iterator_tag {};
	static_assert(std::is_same<ranges::detail::stl2_to_std_iterator_category<my_wonky_tag3, int>, std::input_iterator_tag>::value, "");
	static_assert(std::is_same<ranges::detail::stl2_to_std_iterator_category<my_wonky_tag3, int&>, std::random_access_iterator_tag>::value, "");
	static_assert(std::is_same<ranges::detail::stl2_to_std_iterator_category<ranges::input_iterator_tag, int>, std::input_iterator_tag>::value, "");
	static_assert(std::is_same<ranges::detail::stl2_to_std_iterator_category<ranges::input_iterator_tag, int&>, std::input_iterator_tag>::value, "");
#endif
} // namespace associated_type_test

namespace iterator_sentinel_test {
	struct A {
		using difference_type = signed char;
		using iterator_category = std::input_iterator_tag;
		using value_type = double;

		A& operator++();
		A operator++(int);
		double operator*() const;

		bool operator==(const A&) const;
		bool operator!=(const A&) const;
	};

	static_assert(std::Iterator<int*>);
	static_assert(std::Iterator<const int*>);
	static_assert(!std::Iterator<void*>);
	static_assert(std::Iterator<A>);
	static_assert(std::InputIterator<A>);

	static_assert(std::Iterator<int*>);
	static_assert(std::Sentinel<int*, int*>);
	static_assert(std::Sentinel<const int*, const int*>);
	static_assert(std::Sentinel<const int*, int*>);
	static_assert(!std::Sentinel<void*, void*>);
	static_assert(std::Sentinel<A, A>);
} // namespace iterator_sentinel_test

namespace indirect_result_test {
	template <class R, class... Args>
	using fn_t = R(Args...);
	static_assert(std::Same<std::indirect_result_t<fn_t<void, int>&, const int*>, void>);
}

int main() {}
