/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * STL Containers
*/

module;
#include <deque>
#include <map>
#include <print>
#include <string>
export module rpn_calc;

export class RPN
{
public:
	double op(const std::string& s) {
		if (is_numeric(s)) {
			double v{ std::stod(s) };
			stk.push_front(v);
			return v;
		}
		else return optor(s);
	}

	void clear() {
		stk.clear();
	}

	std::string get_stack_string() const {
		std::string s{};
		for (auto v : stk) {
			s += std::format("{} ", v);
		}
		return s;
	}
private:
	std::deque<double> stk{};
	constexpr static double zero{ 0.0 };
	constexpr static double inf{ std::numeric_limits<double>::infinity() };

	std::pair<double, double> pop_get2() {
		if (stk.size() < 2) return { zero, zero };
		double v1{ stk.front() };
		stk.pop_front();
		double v2{ stk.front() };
		stk.pop_front();
		return { v2, v1 };
	}

	bool is_numeric(const std::string& s) {
		size_t pos{};
		try {
			std::stod(s, &pos);
			return pos == s.size();
		}
		catch (...) {
			return false;
		}
	}

	double optor(const std::string& op) {
		std::map<std::string, double (*)(double, double)> opmap {
			{ "+", [](double l, double r) { return l + r; } },
			{ "-", [](double l, double r) { return l - r; } },
			{ "*", [](double l, double r) { return l * r; } },
			{ "/", [](double l, double r) { return l / r; } },
			{ "^", [](double l, double r) { return pow(l, r); } },
			{ "%", [](double l, double r) { return fmod(l, r); } },
		};

		if (opmap.find(op) == opmap.end()) return zero;
		auto [l, r] = pop_get2();

		if (op == "/" && r == zero) stk.push_front(inf);
		else stk.push_front(opmap.at(op)(l, r));
		return stk.front();
	}
};