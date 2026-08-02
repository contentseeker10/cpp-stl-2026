/* 
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Containers
*/

import std;

import uniform_erase;
import delete_from_unsorted_vector;
import access_vector_elems_safely;

using std::print, std::println;
using std::vector;

void printc(const auto& r) {
	print("size: {}: ", r.size());
	for (auto& e : r) print("{} ", e);
	print("\n");
}

int main() {
	{
		println("Use Uniform Erase funcs to delete elems from a cont.\n");

		std::vector v{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		printc(v);
		// C++20 function for erase-remove idiom
		std::erase(v, 5);
		printc(v);

		std::list l{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		printc(l);
		// Custom erase-remove idiom
		ue::remove_value(l, 5);
		printc(l);
		println();
	}

	{
		println("Delete items from an unsorted vector\n");
		std::vector<int> v = { 12, 196, 47, 38, 19 };
		printc(v);
		auto it = std::ranges::find(v, 47);
		dfuv::quick_delete(v, it);
		printc(v);
		println();
	}

	{
		println("Access vector elements safely\n");
		vector v{ 19, 71, 47, 192, 4004 };
		printc(v);
		println("Func at() : {}", v.at(5)); // does check bounds (runtime)
		println("Braces [] : {}", v[5]); // does check bounds via assertion (runtime)
		// auto& i = v[5]; // breaks assertion
		// println("Ref to braces [] {}", i); // same
		// v[5] = 2001; // same
		// result is different to book
		// probably due to compiler differences (GCC in book)
	}
}