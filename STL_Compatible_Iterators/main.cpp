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
#include <map>

import iter_range;
import iter_adaptors;
import iter_gen_fibo;
import zip_iterator;

using std::print, std::println;

namespace ranges = std::ranges;

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

	{
		println("Create a generator with iterators");

		igfibo::printc(igfibo::fib_generator(10));

		igfibo::fib_generator fib(10);
		auto x = fib | ranges::views::transform(
			[](unsigned long x) { return x * x; }
		);
		igfibo::printc(x, "squared:");

		println();
	}

	{
		println("Create a generator with coroutines");

		using ulong = unsigned long;
		auto gen = icorogen::gen_seq<ulong>(10);
		while (gen) {
			print("{} ", gen());
		}
		println();

		for (auto i : icorogen::gen_fib<ulong>(10)) {
			print("{} ", i);
		}
		println();

		auto gen2 = icorogen::gen_fib<ulong>(10);
		for (auto i : gen2 | ranges::views::transform([](ulong x) { return x * x; })) {
			print("{} ", i);
		}

		println();
	}

	{
		println("\nBuild a zip iterator adaptor");

		std::vector<std::string> vec_a{ "Bob", "John", "Joni" };
		std::vector<std::string> vec_b{ "Dylan", "Williams", "Mitchell" };

		print("vec_a: ");
		for (const auto& e : vec_a) print("{} ", e);
		println();

		print("vec_b: ");
		for (const auto& e : vec_b) print("{} ", e);
		println();

		print("zipped: ");
		for (const auto& [a, b] : zip_iterator(vec_a, vec_b)) {
			print("[{}, {}] ", a, b);
		}

		std::map<std::string, std::string> name_map{};

		for (const auto& [a, b] : zip_iterator(vec_a, vec_b)) {
			name_map.try_emplace(a, b);
		}

		print("name_map: ");
		for (const auto& [a, b] : name_map) {
			print("[{}, {}] ", a, b);
		}

		println();
	}

}