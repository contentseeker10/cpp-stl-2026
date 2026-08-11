/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Strings and Formatting
*/

#include <print>
#include <format>

#include <string>
#include <numbers>

using std::print, std::println, std::format;
using std::string, std::string_view;

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
		println("\n---  ---\n");
	
		
	
		println();
	}
}