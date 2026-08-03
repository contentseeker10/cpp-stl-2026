/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Containers
*/

module;
#include <map>
#include <print>
#include <ranges>
export module eff_todo_list_multimap;

export namespace etlm {
	using todomap = std::multimap<int, std::string>;

	void rprint(const todomap& todo) {
		for (auto const& [priority, task] : todo | std::views::reverse) {
			std::println("{}: {}", priority, task);
		}
		std::print("\n");
	}
}