/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Compatible Iterators
*/

#include <print>
#include <ranges>
#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

import iter_range;
import iter_adaptors;

using std::print, std::println;

int main() {

	{
		println("\nCreate an iterable range & Use traits with your iterators for STL compatibility");

		irange::Seq<int> r(100, 110);
		for (auto v : r) {
			print("{} ", v);
		}

		auto [min_it, max_it] = std::minmax_element(r.begin(), r.end());
		//irange::printc<Seq<int>>(r); // idk, requirement is not met

		println();
		println();
	}

	{
		println("Use iterator adaptors to populate STL containers");

		std::deque<int> d1{ 1, 2, 3, 4, 5 };
		std::deque<int> d2(d1.size());
		std::copy(d1.begin(), d1.end(), d2.begin());
		iadapt::printc(d1, "d1");
		iadapt::printc(d2, "d2 after copy");

		std::copy(d1.begin(), d1.end(), std::back_inserter(d2));
		iadapt::printc(d2, "d2 after back_inserter");

		std::deque<int> d3{ 47, 73, 114, 138, 54 };
		std::copy(d3.begin(), d3.end(), std::front_inserter(d2));
		iadapt::printc(d2, "d2 after front_inserter");

		auto it2 = d2.begin() + 2;
		std::copy(d1.begin(), d1.end(), std::inserter(d2, it2));
		iadapt::printc(d2, "d2 after middle insert");

		print("ostream_iterator: ");
		std::copy(d1.begin(), d1.end(), std::ostream_iterator<int>(std::cout));
		println();

		std::vector<std::string> vs{};
		std::copy(std::istream_iterator<std::string>(std::cin), 
			std::istream_iterator<std::string>(), 
			std::back_inserter(vs));
		iadapt::printc(vs, "vs from istream");

		for (auto it = d1.rbegin(); it != d1.rend(); ++it) {
			print("{} ", *it);
		}
		println();

		println();
	}

}