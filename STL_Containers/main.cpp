/* 
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Containers
*/

#include <vector>
#include <list>
#include <map>
#include <string>
#include <ranges>
#include <algorithm>
#include <print>


import uniform_erase;
import delete_from_unsorted_vector;
import keep_vector_elems_sorted;

using std::print, std::println;
using std::vector, std::map;
using std::string;

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
		println("Func at() : {}", v.at(4)); // does check bounds (runtime)
		println("Braces [] : {}", v[4]); // does check bounds via assertion (runtime)
		// auto& i = v[5]; // breaks assertion
		// println("Ref to braces [] {}", i); // same
		// v[5] = 2001; // same
		// result is different to book
		// probably due to compiler differences (GCC in book)
		println();
	}

	{
		println("Keep vector elements sorted");

		

		vector<std::string> v{
			"Miles",
			"Hendrix",
			"Beatles",
			"Zappa",
			"Shostakovich"
		};
		kves::psorted(v);

		std::ranges::sort(v);
		kves::psorted(v);

		vector<int> v2{ 1, 7, 3, 5, 2 };
		kves::psorted(v2);
		std::ranges::sort(v2);
		kves::psorted(v2);

		kves::insert_sorted(v2, 4);
		kves::psorted(v2);

		println();
	}

	{
		println("Efficiently insert elements into a map");
		map<string, string> m;
		m["Miles"] = "Trumpet";
		m.insert({ "Hendrix", "Guitar" });
		m.emplace("Krupa", "Drums");
		printc(m);
		println();

		map<string, string> m2;
		m2.try_emplace("Miles", "Trumpet");
		m2.try_emplace("Hendrix", "Guitar");
		m2.try_emplace("Krupa", "Drums");
		m2.try_emplace("Zappa", "Guitar");
		m2.try_emplace("Liszt", "Piano");
		printc(m2);
	}
}