/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Lambda Expressions
*/

#include <print>
#include <vector>
#include <algorithm>
#include <ranges>

using std::print, std::println;
using std::vector;

/*bool is_div4(const int i) {
	return i % 4 == 0;
}*/

// functor (functional object)
//struct is_div4 {
//	bool operator()(const int i) const {
//		return i % 4 == 0;
//	}
//};

auto is_div_by(const int divisor) {
	return [divisor](const int i) { return i % divisor == 0; };
}

int main() {
	{
		println("Use lambdas as predicates with algorithms");

		vector<int> v{ 1, 7, 12, 20, 4, 9, 4, 8 };

		auto is_div4 = [](int i) { return i % 4 == 0; }; // lambda as an unary predicate

		auto count4 = std::count_if(v.begin(), v.end(), is_div4);
		println("numbers divisible by 4 (is_div4): {}", count4);

		auto count = std::count_if(v.begin(), v.end(), [](const int i) { return i % 4 == 0; }); // anonymous lambda as an unary predicate
		println("numbers divisible by 4 (anon lambda): {}", count);

		for (int i : { 3, 4, 5 }) {
			auto pred = is_div_by(i);
			auto count = std::count_if(v.begin(), v.end(), pred);
			println("numbers divisible by {}: {} (is_div_by)", i, count);
		}

		auto comp = [](int a, int b) { return a < b; }; // lambda as a binary predicate

		println("unsorted: {}", v);

		std::ranges::sort(v, comp);

		println("sorted: {}", v);

		println();
	}


}