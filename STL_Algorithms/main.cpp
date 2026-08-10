/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Algorithms
*/

#include <iostream>
#include <print>
#include <string>
#include <numbers>

#include <vector>
#include <list>

#include <ranges>
#include <algorithm>
#include <numeric>
#include <memory>

using std::print, std::println;
using std::vector, std::list;
using std::string;
using std::back_inserter;

namespace ranges = std::ranges;
namespace views = ranges::views;

//{
//	println("\n---  ---\n");
//
//
//
//	println();
//}

void printc(auto& c, std::string_view s = "") {
	if (s.size()) print("{}: ", s);
	for (auto e : c) print("[{}] ", e);
	print("\n");
}

namespace vs {
	template <typename I>
	string join(I it, I end_it, std::string_view sep = "") {
		string ostr{};
		if (it != end_it) ostr = std::format("{}", *it++);
		while (it != end_it) {
			ostr = std::format("{}{}{}", ostr, sep, *it++);
		}
		return ostr;
	}

	string join(const auto& c, std::string_view sep = "") {
		return join(c.begin(), c.end(), sep);
	}
}

int main() {
	{
		println("\n--- Copy from one iterator to another ---\n");

		vector<string> v1{ "alpha", "bravo", "charlie", "delta", "echo" };
		printc(v1, "v1");

		//vector<string> v2(v1.size()); // mem alloc
		vector<string> v2{};

		//std::copy(v1.begin(), v1.end(), v2.begin()); // needs mem alloc
		//std::copy(v1.begin(), v1.end(), back_inserter(v2)); // doesn't need mem alloc

		//std::ranges::copy(v1, v2.begin()); // needs mem alloc
		std::ranges::copy(v1, back_inserter(v2)); // doesn't need mem alloc

		printc(v2, "v2");

		vector<string> v3{};

		std::copy_n(v1.begin(), 3, back_inserter(v3));
		printc(v3, "v3");

		vector<string> v4{};

		//std::copy_if(v1.begin(), v1.end(), back_inserter(v4), [](string& s) { return s.size() > 4; });
		std::ranges::copy_if(v1, back_inserter(v4), [](string& s) { return s.size() > 4; });
		printc(v4, "v4");

		std::ostream_iterator<string> out_it(std::cout, " ");
		std::ranges::copy(v1, out_it);
		println();

		vector<string> v5(v1.size());

		//std::move(v1.begin(), v1.end(), v5.begin());
		std::ranges::move(v1, back_inserter(v5));
		printc(v1, "after move: v1");
		printc(v5, "after move: v5");

		println();
	}

	{
		println("\n--- Join container elements into a string ---\n");

		vector<string> lads{ "john", "paul", "george", "ringo", "billy" };
		println("{}", vs::join(lads, ", "));

		namespace num = std::numbers;
		list<double> constants{ num::pi, num::e, num::sqrt2 };
		println("{}", vs::join(constants, ", "));

		auto lads_view = views::join(lads);
		println("{}", vs::join(lads_view, ":"));

		println();
	}

	{
		println("\n---  ---\n");
	
		
	
		println();
	}
}