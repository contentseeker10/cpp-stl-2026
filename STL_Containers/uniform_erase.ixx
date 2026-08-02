/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Containers
*/

module;
export module uniform_erase;
import std;

using std::print;

export namespace ue {
	// Erase-Remove Idiom
	template<typename Tc, typename Tv>
	void remove_value(Tc& c, const Tv v) {
		auto remove_it = std::remove(c.begin(), c.end(), v);
		c.erase(remove_it, c.end());
	}

	void printc(const auto& r) {
		print("size: {}: ", r.size());
		for (auto& e : r) print("{} ", e);
		print("\n");
	}
}