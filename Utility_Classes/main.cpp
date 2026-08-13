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

#include <chrono>

#include <string>

#include <list>
#include <vector>

using std::print, std::println;
using std::optional, std::any;

using std::string, std::string_view;

using std::list;
using std::vector;

using std::tuple;

using std::chrono::system_clock;
using std::chrono::steady_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using seconds = duration<double>;
using milliseconds = duration<double, std::milli>;
using microseconds = duration<double, std::micro>;
using fps24 = duration<unsigned long, std::ratio<1, 24>>;

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

constexpr uint64_t MAX_PRIME{ 0x8FFFFF };

uint64_t count_primes() {
	constexpr auto isprime = [](uint64_t n) {
		if (n == 2) return true;
		if (n < 2 || n % 2 == 0) return false;
		for (uint64_t i{ 3 }; i <= n / i; i += 2) {
			if (n % i == 0) return false;
		}
		return true;
	};

	uint64_t count{ MAX_PRIME >= 2 ? 1 : 0 };
	uint64_t start{ 3 };
	uint64_t end{ MAX_PRIME };

	for (uint64_t i{ start }; i <= end; i += 2) {
		if (isprime(i)) ++count;
	}

	return count;
}

seconds timer(uint64_t(*f)()) {
	auto t1{ steady_clock::now() };
	uint64_t count{ f() };
	auto t2{ steady_clock::now() };
	seconds secs{ t2 - t1 };
	println("there are {} primes in range", count);
	return secs;
}

template <typename... T>
void print_t(const std::tuple<T...>& tup) {
	std::apply([](const auto&... elems) {
		((print("{} ", elems)), ...);
	}, tup);
	println();
}

template <typename... T>
constexpr int sum_t(const tuple<T...>& tup) {
	return std::apply([](const auto&... elems) {
		return (elems + ...);
	}, tup);
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

	{
		println("\n--- Time events with std::chrono ---\n");
		
		auto t = system_clock::now();
		println("system_clock::now is {:%F %T %Z}", t);

		auto secs{ timer(count_primes) };
		println("time elapsed: {:.3f} sec", secs.count());
		println("time elapsed: {:.3f} ms", milliseconds(secs).count());
		println("time elapsed: {:.3e} mcs", microseconds(secs).count());
		println("time elapsed: {} frames at 24 fps", floor<fps24>(secs).count()); 

		println();
	}

	{
		println("\n--- Use fold expressions for generic tuples ---\n");
		
		tuple labels{ "ID", "Name", "Scale" };
		tuple employee{ 123456, "John Doe", 3.7 };
		tuple nums{ 1, 7, "forty-two", 47, 73L, -111.11 };

		print_t(labels);
		print_t(employee);
		print_t(nums);

		tuple ti1{ 1, 2, 3, 4, 5 };
		tuple ti2{ 9, 10, 11, 12, 13, 14, 15 };
		tuple ti3{ 47, 73, 42 };

		auto sum1{ sum_t(ti1) };
		auto sum2{ sum_t(ti2) };
		auto sum3{ sum_t(ti3) };

		println("sum of ti1: {}", sum1);
		println("sum of ti2: {}", sum2);
		println("sum of ti3: {}", sum3);

		println();
	}
}