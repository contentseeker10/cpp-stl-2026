/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Strings and Formatting
*/

#include <print>
#include <string>

using std::print, std::println;
using std::string, std::string_view;

//{
//	println("\n---  ---\n");
//
//
//
//	println();
//}

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
}