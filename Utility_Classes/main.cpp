/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Utility Classes
*/

#include <print>
#include <optional>
#include <any>

#include <string>

#include <list>
#include <vector>

using std::print, std::println;
using std::optional, std::any;

using std::string;

using std::list;
using std::vector;

void p_any(const any& a) {
	if (!a.has_value()) {
		println("None.");
	}
	else if (a.type() == typeid(int)) {
		println("int: {}", any_cast<int>(a));
	}
	else if (a.type() == typeid(string)) {
		println("string: \"{}\"", any_cast<const string&>(a));
	}
	else if (a.type() == typeid(list<int>)) {
		println("list<int>: ");
		for (auto& i : any_cast<const list<int>&>(a)) print("{} ", i);
		println();
	}
	else {
		println("someting else: {}", a.type().name());
	}
}

//{
	//println("\n---  ---\n");
	//
	//// .
	//
	//println();
//}

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

	{
		println("\n--- Use std::any for type safety ---\n");
		
		any x{};

		if (x.has_value()) println("has value");
		else println("no value");

		x = 42;

		if (x.has_value()) {
			println("x has type: {}", x.type().name());
			println("x has value: {}", any_cast<int>(x));
		}
		else {
			println("no value");
		}

		using namespace std::literals;
		x = "abc"s;
		println("x is type {} with value {}", x.type().name(), any_cast<string>(x));
		
		p_any({});
		p_any(47);
		p_any("abc"s);
		p_any(any(list{ 1, 2, 3 }));
		p_any(any(vector{ 1, 2, 3 }));

		try {
			println("{}", any_cast<int>(x));
		}
		catch (std::bad_any_cast& e) {
			println("any: {}", e.what());
		}

		println();
	}
}