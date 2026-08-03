/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Compatible Iterators
*/

#include <print>
#include <ranges>
#include <algorithm>

import iter_range;

using std::print, std::println;

int main() {

	{
		println("\nCreate an iterable range & Use traits with your iterators for STL compatibility");

		Seq<int> r(100, 110);

		auto [min_it, max_it] = std::minmax_element(r.begin(), r.end());
		//printc<Seq<int>>(r); // idk, requirement is not met

		println();
	}

}