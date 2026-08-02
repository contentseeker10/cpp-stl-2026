/* 
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Containers
*/

import std;
import uniform_erase;

using std::print, std::println;

int main() {
	{
		println("Use Uniform Erase funcs to delete elems from a cont.\n");

		std::vector v{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		ue::printc(v);
		// C++20 function for erase-remove idiom
		std::erase(v, 5);
		ue::printc(v);

		std::list l{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		ue::printc(l);
		// Custom erase-remove idiom
		ue::remove_value(l, 5);
		ue::printc(l);
		println();
	}

	
}