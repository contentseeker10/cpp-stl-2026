/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Utility Classes
*/

#include <print>
#include <optional>
#include <any>
#include <variant>

#include <string>

#include <list>
#include <vector>

using std::print, std::println;
using std::optional, std::any;

using std::string, std::string_view;

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

class Animal {
public:
	Animal(string_view n, string_view s) : _name{n}, _sound{s} {}
	void speak() const {
		println("{} says {}", _name, _sound);
	}
	void sound(string_view s) {
		_sound = s;
	}
private:
	string_view _name;
	string_view _sound;
	Animal();
};

class Cat : public Animal {
public:
	Cat(string_view n) : Animal(n, "meow") {};
};

class Dog : public Animal {
public:
	Dog(string_view n) : Animal(n, "arf!") {};
};

class Wookie : public Animal {
public:
	Wookie(string_view n) : Animal(n, "grraargghh!") {};
};


using v_animal = std::variant<Cat, Dog, Wookie>;

struct animal_speaks {
	void operator()(const Dog& d) const { d.speak(); }
	void operator()(const Cat& c) const { c.speak(); }
	void operator()(const Wookie& w) const { w.speak(); }
};

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

	{
		println("\n--- Store different types with std::variant ---\n");
		
		list<v_animal> pets{
			Cat{"Hobbes"}, Dog{"Fido"}, Cat{"Max"}, Wookie{"Chewie"}
		};

		for (const v_animal& a : pets) {
			std::visit(animal_speaks{}, a);
		}

		println();
		
		for (const v_animal& a : pets) {
			auto idx{ a.index() };
			if (idx == 0) std::get<Cat>(a).speak();
			if (idx == 1) std::get<Dog>(a).speak();
			if (idx == 2) std::get<Wookie>(a).speak();
		}

		println();

		for (const v_animal& a : pets) {
			if (const auto c{ std::get_if<Cat>(&a) }; c) {
				c->speak();
			}
			else if (const auto d{ std::get_if<Dog>(&a) }; d) {
				d->speak();
			}
			else if (const auto w{ std::get_if<Wookie>(&a) }; w) {
				w->speak();
			}
		}

		println();

		size_t n_cats{}, n_dogs{}, n_wookies{};

		for (const v_animal& a : pets) {
			if (std::holds_alternative<Cat>(a)) ++n_cats;
			if (std::holds_alternative<Dog>(a)) ++n_dogs;
			if (std::holds_alternative<Wookie>(a)) ++n_wookies;
		}

		println("there are {} cats, {} dogs and {} wookies", n_cats, n_dogs, n_wookies);

		println();
	}
}