/* 
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Containers
*/

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <deque>
#include <string>
#include <ranges>
#include <algorithm>
#include <print>
#include <regex>

import uniform_erase;
import delete_from_unsorted_vector;
import keep_vector_elems_sorted;
import eff_modify_keys_map;
import use_um_custom_keys;
import rpn_calc;
import eff_todo_list_multimap;

using std::print, std::println;
using std::vector, std::map, std::unordered_map, std::set;
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
		println();
	}

	{
		println("Efficiently modify keys in a map");

		map<unsigned int, string> racers{
			{1, "Mario"}, {2, "Luigi"}, {3, "Bowser"},
			{4, "Peach"}, {5, "Donkey Kong Jr"}
		};

		printc(racers);
		emkm::node_swap(racers, 3, 5);
		printc(racers);

		println();
	}

	{
		println("Use unordered_map with custom keys");
		
		Coordmap m{
			{ {0, 0}, 1 },
			{ {0, 1}, 2 },
			{ {2, 1}, 3 }
		};
		print_Coordmap(m);

		Coord k{ 0, 1 };
		print("\n{{ ({}, {}): {} }}", k.x, k.y, m.at(k));

		println();
	}

	{
		println("Sort and filter user input with set");

		using input_it = std::istream_iterator<std::string>;

		set<string> words;

		input_it it{ std::cin };
		input_it end{};

		std::copy(it, end, std::inserter(words, words.end()));

		for (const string& w : words) {
			print("{} ", w);
		}

		println();
	}

	{
		println("Create a simple RPN calculator with deque");

		RPN rpn;
		for (string o{}; std::cin >> o; ) {
			rpn.op(o);
			auto stack_str = rpn.get_stack_string();
			println("{}: {}", o, stack_str);
		}

		println();
	}

	{
		println("Count word frequency with map");

		constexpr const char* re{ "(\\w+)" };

		map<string, int> wordmap{};
		vector <std::pair<string, int>> wordvec{};
		std::regex word_re(re);
		size_t total_words{};

		for (string s{}; std::cin >> s; ) {
			auto words_begin = std::sregex_iterator(s.begin(), s.end(), word_re);
			auto words_end = std::sregex_iterator();

			for (auto r_it = words_begin; r_it != words_end; ++r_it) {
				std::smatch match{ *r_it };
				auto word_str = match.str();

				std::ranges::transform(word_str, word_str.begin(),
					[](unsigned char c) { return tolower(c); });

				auto [map_it, result] = wordmap.try_emplace(word_str, 0);
				auto& [w, count] = *map_it;

				++total_words;
				++count;
			}
		}

		auto unique_words = wordmap.size();
		wordvec.reserve(unique_words);
		std::ranges::move(wordmap, std::back_inserter(wordvec));
		std::ranges::sort(wordvec, [](const auto& a, const auto& b) {
			if (a.second != b.second)
				return (a.second > b.second);
			return (a.first < b.first);
		});

		println("total word count: {}", total_words);
		println("unique word count: {}", unique_words);

		for (int limit{ 20 }; auto& [w, count] : wordvec) {
			println("{}: {}", count, w);
			//if (--limit == 0) break;
		}

		println();
	}

	{
		println("Create an efficient ToDo list using multimap");

		etlm::todomap todo{
			{1, "wash dishes"},
			{0, "watch tv"},
			{2, "do homework"},
			{0, "read comics"},
		};
		etlm::rprint(todo);

		println();
	}

}