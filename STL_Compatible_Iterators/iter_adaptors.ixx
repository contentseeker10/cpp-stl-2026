/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Compatible Iterators
*/

module;
#include <string>
#include <print>
export module iter_adaptors;

export namespace iadapt {

	void printc(const auto& v, const std::string_view s = "") {
		if (!s.empty()) std::print("{}: ", s);
		for (auto e : v) std::print("{} ", e);
		std::print("\n");
	}

}