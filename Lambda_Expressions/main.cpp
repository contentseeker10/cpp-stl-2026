/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Lambda Expressions
*/

#include <print>

#include <vector>
#include <deque>
#include <list>

#include <algorithm>
#include <ranges>
#include <functional>

using std::print, std::println;
using std::vector, std::deque, std::list;
using std::function;

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

template <typename T, typename... Ts>
auto concat(const T t, const Ts... ts) {
	if constexpr (sizeof...(ts) > 0) {
		return [=](const auto... parameters) {
			return t(concat(ts...)(parameters...));
		};
	}
	else {
		return t;
	}
}

int main() {
	{
		println("\n--- Use lambdas as predicates with algorithms ---\n");

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

	{
		println("\n--- Create a polymorhic wrapper with std::function ---\n");

		deque<int> d;
		list<int> l;
		vector<int> v;

		auto print_c = [](const auto& c) {
			print("container: {}: ", typeid(c).name());
			for (const auto i : c) print("{} ", i);
			print("\n");
		};

		auto push_c = [](auto& container) {
			return [&container](const auto value) {
				container.push_back(value);
			};
		};

		const vector<function<void(int)>> consumers{ push_c(d), push_c(l), push_c(v) };

		for (auto& c : consumers) {
			for (int i{ 0 }; i < 10; ++i) {
				c(i);
			}
		}

		print_c(d);
		print_c(l);
		print_c(v);

		println();
	}

	{
		println("\n--- Concatenate lambdas with recursion ---\n");

		auto twice = [](const auto i) { return i * 2; };
		auto thrice = [](const auto i) { return i * 3; };

		auto combined = concat(thrice, twice, std::plus<int>{});
		println("{}", combined(2, 3));

		println();
	}


}