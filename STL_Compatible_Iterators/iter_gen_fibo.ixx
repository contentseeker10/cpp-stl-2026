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
#include <coroutine>

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

export namespace icorogen {

	template<typename T>
	class co_gen {
	public:
		struct promise_type {
			T value;

			co_gen get_return_object() {
				return { handle_type::from_promise(*this) };
			}

			std::suspend_always initial_suspend() {
				return {};
			}

			std::suspend_always final_suspend() noexcept {
				return {};
			}

			void return_void() {}

			std::suspend_always yield_value(T v) {
				value = v;
				return {};
			}

			void unhandled_exception() {
				std::exit(1);
			}
		};
		using handle_type = std::coroutine_handle<promise_type>;
		handle_type coro{};

		co_gen(handle_type h) : coro(h) {}

		~co_gen() {
			if (coro) coro.destroy();
		}

		operator bool() {
			coro.resume();
			return !coro.done();
		}

		T operator()() {
			return coro.promise().value;
		}

		class iterator {
		public:
			using iterator_concept = std::forward_iterator_tag;
			using iterator_category = std::forward_iterator_tag;
			using value_type = std::remove_cv_t<T>;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;

			iterator() : coro(nullptr) {}
			iterator(handle_type h) : coro(h) {}

			T& operator*() const {
				return coro.promise().value;
			}

			T* operator->() const {
				return &coro.promise().value;
			}

			iterator& operator++() {
				coro.resume();
				if (coro.done()) coro = nullptr;
				return *this;
			}

			iterator operator++(int) {
				iterator temp = *this;
				++*this;
				return temp;
			}

			bool operator==(const iterator& other) const {
				return coro == other.coro;
			}

			bool operator!=(const iterator& other) const {
				return !(*this == other);
			}
		private:
			handle_type coro{};
		};

		iterator begin() {
			if (coro) coro.resume();
			if (coro.done()) return iterator{ nullptr };
			return iterator{ coro };
		}

		iterator end() {
			return iterator{ nullptr };
		}
	};

	template<typename T>
	co_gen<T> gen_seq(T count) {
		for (T i{}; i < count; ++i) {
			co_yield i;
		}
	}

	template<typename T>
	co_gen<T> gen_fib(T count) {
		T n1{}, n2{ 1 };
		auto do_fib = [&n1, &n2] {
			const T prev_n2 = n2;
			n2 += n1;
			n1 = prev_n2;
			return n1;
		};

		for (T i = 0; i < count; ++i) {
			co_yield do_fib();
		}
	}

}