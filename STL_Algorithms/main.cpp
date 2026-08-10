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
#include <random>

using std::print, std::println;
using std::vector, std::list;
using std::string;
using std::back_inserter, std::transform;

namespace ranges = std::ranges;
namespace views = ranges::views;

//{
//	println("\n---  ---\n");
//
//
//
//	println();
//}

void check_sorted(const auto& c) {
	if (!ranges::is_sorted(c)) print("un");
	print("sorted: ");
}

void printc(auto& c, std::string_view s = "") {
	check_sorted(c);
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

void randomize(auto& c) {
	static std::random_device rd;
	static std::default_random_engine rng(rd());
	ranges::shuffle(c, rng);
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
		println("\n--- Sort containers with std::sort ---\n");
	
		vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		printc(v);

		randomize(v);
		printc(v);

		//std::sort(v.begin(), v.end());
		ranges::sort(v);
		printc(v);

		randomize(v);

		auto middle{ v.begin() + (v.size() / 2) };
		std::partial_sort(v.begin(), middle, v.end());
		printc(v);

		randomize(v);
		printc(v);
		std::partition(v.begin(), v.end(), [](int i) { return i > 5; });
		printc(v);
	
		println();
	}

	{
		println("\n--- Modify containers with std::transform ---\n");
	
		vector<int> v1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		vector<int> v2{};

		printc(v1, "v1");

		println("squares:");
		// does not guarantee order of transform operation
		transform(v1.begin(), v1.end(), back_inserter(v2), [](int x) { return x * x; });
		printc(v2);

		vector<string> vstr1{ "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto" };
		vector<string> vstr2{};

		printc(vstr1, "vstr1");
		println("str_lower:");
		transform(vstr1.begin(), vstr1.end(), back_inserter(vstr2), [](string& s) {
			auto char_lower = [](char c) -> char {
				if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
				else return c;
			};
			for (auto& c : s) c = char_lower(c);
			return s;
		});
		printc(vstr2, "vstr2");

		println("ranges squares:");
		auto view = views::transform(v1, [](int x) { return x * x; });
		printc(view, "view");

		println();
	}


}