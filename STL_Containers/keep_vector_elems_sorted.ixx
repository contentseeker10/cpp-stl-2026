/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Containers
*/

module;

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <print>

export module keep_vector_elems_sorted;

export namespace kves {

	void printv(const auto& v) {
		for (const auto& e : v) {
			std::print("{} ", e);
		}
		std::print("\n");
	}

	void psorted(const auto& v) {
		if (std::ranges::is_sorted(v)) std::print("sorted: ");
		else std::print("unsorted: ");
		printv(v);
	}

	template <typename C, typename E>
	void insert_sorted(C& c, const E& e) {
		const auto pos{ std::ranges::lower_bound(c, e) };
		c.insert(pos, e);
	}
}