/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Compatible Iterators
*/

module;
#include <iterator>
#include <type_traits>
#include <print>
#include <ranges>
export module iter_range;

export namespace irange {

	template<typename T>
	class Seq {
	public:
		Seq(T start, T end) : seq_start{ start }, seq_end{ end } {}

		class iterator {
		public:
			using iterator_concept = std::forward_iterator_tag;
			using iterator_category = std::forward_iterator_tag;
			using value_type = std::remove_cv_t<T>;
			using difference_type = std::ptrdiff_t;
			using pointer = const T*;
			using reference = const T&;

			explicit iterator(T position = 0) : it_value{ position } {}

			const T& operator*() const { return it_value; }

			iterator& operator++() {
				++it_value;
				return *this;
			}

			bool operator==(const iterator& other) const noexcept {
				return it_value == other.it_value;
			}

		private:
			T it_value{};
		};

		iterator begin() const { return iterator{ seq_start }; }
		iterator end() const { return iterator{ seq_end }; }
	private:
		T seq_start{};
		T seq_end{};
	};

	export template<typename T>
		requires std::forward_iterator<typename T::iterator> // idk, requirement is not met
	void printc(const T& c) {
		for (const auto& v : c) {
			std::print("{} ", v);
		}
		std::print("\n");
	}

}