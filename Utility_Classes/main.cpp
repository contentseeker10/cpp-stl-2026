/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Utility Classes
*/

#include <print>
#include <optional>

using std::print, std::println;
using std::optional;

//println("\n---  ---\n");
//
//// .
//
//println();

int main() {
	{
		println("\n--- Manage optional values with std::optional ---\n");

		optional<int> a{ 42 };
		println("{}", *a);

		using oint = optional<int>;

		oint b{};

		if (b) println("{}", *b);
		else println("no value");

		a.reset();

		try
		{
			println("{}", a.value());
		}
		catch (const std::bad_optional_access& e)
		{
			println("b.value(): {}", e.what());
		}

		println();
	}
}