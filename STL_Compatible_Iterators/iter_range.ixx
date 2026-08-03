/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Compatible Iterators
*/

module;
export module iter_range;

export template<typename T>
class Seq {
public:
	Seq(T start, T end) : seq_start {start}, seq_end {end} {}

	class iterator {
	public:
		explicit iterator(T position = 0) : it_value {position} {}

		T operator*() const { return it_value; }

		iterator& operator++() {
			++it_value;
			return *this;
		}

		bool operator !=(const Seq::iterator& other) const {
			return it_value != other.it_value;
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