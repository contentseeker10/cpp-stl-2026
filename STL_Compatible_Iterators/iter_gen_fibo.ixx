/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Compatible Iterators
*/

module;
#include <string>
#include <print>
export module iter_gen_fibo;

export namespace igfibo {
	using fib_t = size_t;
	template<typename T>
	class fib_generator {
	public:
		using iterator_concept = std::forward_iterator_tag;
		using iterator_category = std::forward_iterator_tag;
		using value_type = std::remove_cv_t<fib_t>;
		using difference_type = std::ptrdiff_t;
		using pointer = const fib_t*;
		using reference = const fib_t&;

		explicit fib_generator(T fib_stop = 0) : stop {fib_stop} {}

		T operator*() const { return n2; }

		constexpr fib_generator& operator++() {
			do_fib();
			++count;
			return *this;
		}

		fib_generator operator++(int) {
			auto temp = *this;
			++*this;
			return temp;
		}

		bool operator!=(const fib_generator& o) const {
			return count != o.count;
		}

		bool operator==(const fib_generator& o) const {
			return count == o.count;
		}

		const fib_generator& begin() const { return *this; }
		const fib_generator end() const {
			auto sentinel = fib_generator();
			sentinel.count = stop;
			return sentinel;
		}

		fib_t size() { return stop; }
	private:
		T stop{};
		T count{};
		T n1{ 0 };
		T n2{ 1 };

		constexpr void do_fib() {
			const T prev_n2 = n2;
			n2 += n1;
			n1 = prev_n2;
		}
	};

	void printc(const auto& v, const std::string_view s = "") {
		if (!s.empty()) std::print("{}: ", s);
		for (auto e : v) std::print("{} ", e);
		std::print("\n");
	}

}