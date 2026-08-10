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
#include <map>

#include <algorithm>
#include <ranges>
#include <functional>

using std::print, std::println;
using std::vector, std::deque, std::list;
using std::function;
using std::string;

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

template <typename F, typename A, typename B>
auto combine(F binary_func, A a, B b) {
	return [=](auto param) {
		return binary_func(a(param), b(param));
	};
}

auto braces(const char a, const char b) {
	return [a, b](const auto v) {
		print("{}{}{}", a, v, b);
	};
}

const char prompt(const char* p) {
	constexpr size_t BUFLEN{ 4 };
	constexpr int NL{ '\n' };
	char bufin[BUFLEN]{};
	int bufchar{};

	auto char_upper = [](char c) -> char {
		if (c >= 'a' && c <= 'z') return c - ('a' - 'A');
		else return c;
	};

	auto flush_stdin = [] {
		int c{};
		while (c != NL && c != EOF) c = std::getchar();
	};

	print("{} > ", p);
	for (size_t i{}; i < BUFLEN && (bufchar = getchar()) != NL; ++i) {
		bufin[i] = (char)bufchar;
	}

	if (bufchar != NL) flush_stdin();
	if (bufchar == EOF) exit(0);

	const char r0 = bufin[0];
	const char r1 = bufin[1];

	if (r0 == 0 || r0 == NL) return 0;
	else if (r1 != 0 && r1 != NL) {
		println("Response is too long");
		return 0;
	}
	else return char_upper(r0);
}

const bool jump(const char select) {
	using jfunc = std::function<void()>;

	static const std::map<char, jfunc> jmap{
		{ 'A', [] { println("func A\n"); } },
		{ 'B', [] { println("func B\n"); } },
		{ 'C', [] { println("func C\n"); } },
		{ 'D', [] { println("func D\n"); } },
		{ 'X', [] { println("bye bye jump table\n"); } }
	};

	const auto it = jmap.find(select);
	if (it != jmap.end()) it->second();
	else {
		println("Invalid response");
		return false;
	}
	return true;
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

	{
		println("\n--- Combine predicates with logical conjuction ---\n");

		auto begins_with = [](const string& s) { return s.find("a") == 0; };
		auto ends_with = [](const string& s) { return s.rfind("b") == s.length() - 1; };
		auto bool_and = [](const auto& l, const auto& r) { return l & r; };

		vector<string> v{ "aabb", "bbaa", "foo", "bar", "abazb" };
		vector<string> out;
		std::copy_if(v.begin(), v.end(), std::back_inserter(out), combine(bool_and, begins_with, ends_with));
		print("{}", out);

		println();
	}

	{
		println("\n--- Pass common parameters to multiple lambdas ---\n");

		auto a = braces('(', ')');
		auto b = braces('[', ']');
		auto c = braces('{', '}');
		auto d = braces('|', '|');

		for (auto i : { 1, 2, 3, 4, 5 }) {
			for (auto x : { a, b, c, d }) x(i);
			print("\n");
		}

		println();
	}

	{
		println("\n--- Create a simple jump table with mapped lambdas ---\n");

		const char* pstr{ "What to do? (A/B/C/D/X)" };
		for (auto key = prompt(pstr); ; key = prompt(pstr)) {
			if (key) jump(key);
			if (key == 'X') break;
		}

		println();
	}


}