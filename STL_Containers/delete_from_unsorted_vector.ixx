/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Containers
*/

module;
export module delete_from_unsorted_vector;
import std;

export namespace dfuv {
	template<typename T>
	void quick_delete(T& v, typename T::iterator it) {
		if (it < v.end()) {
			if (it != std::prev(v.end())) {
				*it = std::move(v.back());
			}
			v.pop_back();
		}
	}
}