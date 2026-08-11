/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Strings and Formatting
*/

#include <iosfwd>
#include <iostream>
#include <fstream>
#include <print>
#include <format>
#include <cstdio>

#include <string>
#include <numbers>

#include <chrono>

#include <vector>

using std::print, std::println, std::format;
using std::string, std::string_view;
using std::vector;

//{
//	println("\n---  ---\n");
//
//
//
//	println();
//}

template <typename T>
struct Frac
{
	T n;
	T d;
};

template <typename T>
struct std::formatter<Frac<T>> : std::formatter<int> {
	template <typename FormatContext>
	auto format(const Frac<T>& o, FormatContext& ctx) const {
		return format_to(ctx.out(), "{}/{}", o.n, o.d);
	}
};

void timer(string(*f)()) {
	using namespace std::chrono;
	auto t1 = high_resolution_clock::now();
	string s{ f() };
	auto t2 = high_resolution_clock::now();
	duration<double, std::milli> ms = t2 - t1;
	println("{}", s);
	println("duration: {} ms", ms.count());
}

#define ITERS 1'000'000

string append_string() {
	println("\n== append_string:");
	string a{ "a" };
	string b{ "b" };
	long n{};
	while (++n) {
		string x{};
		x.append(a);
		x.append(", ");
		x.append(b);
		if (n >= ITERS) return x;
	}
	return "error";
}

string concat_string() {
	println("\n== concat_string:");
	string a{ "a" };
	string b{ "b" };
	long n{};
	while (++n) {
		string x{};
		x += a + ", " + b;
		if (n >= ITERS) return x;
	}
	return "error";
}

string concat_ostringstream() {
	println("\n== ostringstream:");
	string a{ "a" };
	string b{ "b" };
	long n{};
	while (++n) {
		std::ostringstream x{};
		x << a << ", " << b;
		if (n >= ITERS) return x.str();
	}
	return "error";
}

string concat_format() {
	println("\n== concat_format:");
	string a{ "a" };
	string b{ "b" };
	long n{};
	while (++n) {
		string x{};
		x = format("{}, {}", a, b);
		if (n >= ITERS) return x;
	}
	return "error";
}

string trimstr(const string& s) {
	constexpr const char* whitespace{ " \t\r\n\v\f" };
	if (s.empty()) return s;
	const auto first = s.find_first_not_of(whitespace);
	if (first == string::npos) return {};
	const auto last = s.find_last_not_of(whitespace);
	return s.substr(first, (last - first + 1));
}

bool prompt(const string_view s, const string_view s2 = "") {
	if (s2.size()) print("{} ({}): ", s, s2);
	else print("{}: ", s);
	fflush(stdout);
	return true;
}

void clearistream() {
	int c{};
	std::cin.clear();
	while (c != '\n' && c != EOF) c = getchar();
}

size_t wordcount(auto& is) {
	using it_t = std::istream_iterator<string>;
	return std::distance(it_t(is), it_t());
}

struct City
{
	string name;
	unsigned long population;
	double latitude;
	double longitude;
};

std::istream& operator>>(std::istream& in, City& c) {
	in >> std::ws;
	std::getline(in, c.name);
	in >> c.population >> c.latitude >> c.longitude;
	return in;
}

string make_commas(const uint64_t num) {
	auto s = std::to_string(num);
	for (auto i = s.size(); i > 3; i -= 3) {
		s.insert(i - 3, ",");
	}
	return s;
}

constexpr char char_lower(const char& c) {
	if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
	else return c;
}

class ci_traits : public std::char_traits<char> {
public:
	static constexpr bool lt(char_type a, char_type b) noexcept {
		return char_lower(a) < char_lower(b);
	}
	static constexpr bool eq(char_type a, char_type b) noexcept {
		return char_lower(a) == char_lower(b);
	}
	static constexpr int compare(const char_type* s1, const char_type* s2, size_t count) {
		for (size_t i{0}; i < count; ++i)
		{
			auto diff = char_lower(s1[i]) <=> char_lower(s2[i]);
			if (diff > 0) return 1;
			if (diff < 0) return -1;
		}
		return 0;
	}
	static constexpr const char_type* find(const char_type* p, size_t count, const char_type& ch) {
		const char_type find_c{ char_lower(ch) };
		for (size_t i{0}; i < count; ++i)
		{
			if (find_c == char_lower(p[i])) return p + i;
		}
		return nullptr;
	}
};

using ci_string = std::basic_string<char, ci_traits>;

int main() {
	{
		println("\n--- Use string_view as a lightweight string object ---\n");

		char text[]{ "hello" };
		string_view greeting{ text };
		text[0] = 'J';
		println("{} {}", text, greeting);

		using namespace std::literals;
		auto sv = "hello"sv;
		println("{}", sv.substr(1, 4));

		auto str_data = sv.data();
		for (auto i = 0; i < sv.size(); ++i)
		{
			print("{} ", str_data[i]);
		}

		println();
	}

	{
		println("\n--- Format text with the format library ---\n");
	
		string who{ "everyone" };
		int ival{ 42 };
		double pi{ std::numbers::pi };
		auto s1 = format("Hello, {}!", who);
		auto s2 = format("Integer: {}", ival);
		auto s3 = format("pi: {}", pi);

		for (auto s : { s1, s2, s3 })
		{
			println("{}", s);
		}

		auto s4 = format("Hello {1} {0}", ival, who);
		auto s5 = format("Hello {0} {1}", ival, who);
	
		println("{}\n{}", s4, s5);

		auto s6 = format("{:.<10}", ival);
		auto s7 = format("{:.>10}", ival);
		auto s8 = format("{:.^10}", ival);
		
		println("{}\n{}\n{}", s6, s7, s8);

		Frac f{ 5, 3 };
		println("Frac f {{5, 3}}: {}", f);

		println();
	}

	{
		println("\n--- Concatenate strings and test efficiency ---\n");
	
		string a{ "a" };
		string b{ "b" };

		string x{};

		//x += a + ", " + b;

		//x.append(a);
		//x.append(", ");
		//x.append(b);

		//std::ostringstream xs{};
		//xs << a << ", " << b;
		
		x = format("{}, {}", a, b);

		//println("{}", xs.str());
		println("{}", x);

		timer(append_string);
		timer(concat_string);
		timer(concat_ostringstream);
		timer(concat_format);
		
	
		println();
	}

	{
		println("\n--- Trim whitespace from strings ---\n");
	
		string s{ " \t  ten-thumbed input   \t   \n \t " };
		println("[{}]", s);

		s = trimstr(s);
		println("[{}]", s);
	
		println();
	}

	{
		println("\n--- Read strings from user input ---\n");
	
		constexpr size_t MAXLINE{ 1024 * 10 };
		char s[MAXLINE]{};
		const char* p1{ "Words here" };
		prompt(p1);
		std::cin.getline(s, MAXLINE, '\n');
		println("{}", s);

		string line{};
		const char* p1a{ "More words here" };
		prompt(p1a, "p1a");
		getline(std::cin, line, '\n');
		println("{}", line);
		
		const char* p2{ "Doubles here" };
		double a{};
		double b{};
		for (prompt(p2); !(std::cin >> a >> b); prompt(p2))
		{
			println("not numeric");
			clearistream();
		}
		println("You entered {} and {}", a, b);
		clearistream();

		println();
	}

	{
		println("\n--- Count words in a file ---\n");
	
		const char* fn{ "testdata.txt" };
		auto infile = std::ifstream(fn, std::ios_base::in);
		println("There are {} words in the file.", wordcount(infile));
	
		println();
	}

	{
		println("\n--- Initialize complex structures from file input ---\n");

		constexpr const char* fn{ "cities.txt" };

		vector<City> cities;

		std::ifstream infile(fn, std::ios_base::in);
		if (!infile.is_open()) {
			println("failed to open file {}", fn);
			return 1;
		}
		for (City c{}; infile >> c;)
		{
			cities.emplace_back(c);
		}

		for (const auto& [name, pop, lat, lon] : cities)
		{
			println("{:.<15} pop {:.10} coords {}, {}", name, make_commas(pop), lat, lon);
		}

		println();
	}

	{
		println("\n--- Customize a string class with char_traits ---\n");

		ci_string cmp1{ "CoMpArE StRiNg" };
		ci_string cmp2{ "compare string" };

		if (cmp1 == cmp2) {
			println("Match! {} == {}", cmp1, cmp2);
		}
		else {
			println("no match {} != {}", cmp1, cmp2);
		}

		println();
	}
}