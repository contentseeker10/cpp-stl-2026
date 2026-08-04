/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Compatible Iterators
*/

module;
#include <iterator>
#include <type_traits>
export module zip_iterator;

export template<typename T>
class zip_iterator {
private:
	using val_t = typename T::value_type;
	using ret_t = std::pair<val_t, val_t>;
	using it_t = typename T::iterator;

	it_t ita{};
	it_t itb{};
	it_t ita_begin{};
	it_t itb_begin{};
	it_t ita_end{};
	it_t itb_end{};

	zip_iterator(it_t ita, it_t itb) : ita{ ita }, itb{ itb } {}
public:
	using iterator_concept = std::forward_iterator_tag;
	using iterator_category = std::forward_iterator_tag;
	using value_type = std::pair<val_t, val_t>;
	using difference_type = long int;
	using pointer = const val_t*;
	using reference = const val_t&;

	zip_iterator(T& a, T& b) :
		ita {a.begin()},
		itb {b.begin()},
		ita_begin {ita},
		itb_begin {itb},
		ita_end {a.end()},
		itb_end {b.end()}
	{}

	zip_iterator& operator++() {
		++ita;
		++itb;
		return *this;
	}

	bool operator==(const zip_iterator& o) const {
		return ita == o.ita || itb == o.itb;
	}

	bool operator!=(const zip_iterator& o) const {
		return !operator==(o);
	}

	ret_t operator*() const {
		return { *ita, *itb };
	}

	zip_iterator begin() const {
		return zip_iterator(ita_begin, itb_begin);
	}

	zip_iterator end() const {
		return zip_iterator(ita_end, itb_end);
	}
};