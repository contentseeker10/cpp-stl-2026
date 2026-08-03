module;
#include <unordered_map>
#include <print>
export module use_um_custom_keys;

export struct Coord {
	int x{};
	int y{};
};

export using Coordmap = std::unordered_map<Coord, int>;

export bool operator==(const Coord& lhs, const Coord& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

namespace std {
	template<>
	struct hash<Coord> {
		size_t operator()(const Coord& c) const noexcept {
			size_t h1{ std::hash<int>{}(c.x) };
			size_t h2{ std::hash<int>{}(c.y) };
			return h1 ^ (h1 << 1);
		}
	};
}

export void print_Coordmap(const Coordmap& m) {
	for (const auto& [key, value] : m) {
		std::print("{{ ({}, {}): {} }} ", key.x, key.y, value);
	}
}