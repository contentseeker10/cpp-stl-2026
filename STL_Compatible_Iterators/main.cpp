/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Compatible Iterators
*/

#include <print>

import iter_range;

using std::print, std::println;

int main() {

	{
		println("\nCreate an iterable range");

		Seq<int> r(100, 110);

		for (auto v : r) {
			print("{} ", v);
		}

		println();
	}

}