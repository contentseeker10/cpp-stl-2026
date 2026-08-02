module;
#include <map>
export module eff_modify_keys_map;

export namespace emkm {
	template<typename M, typename K>
	bool node_swap(M& m, K k1, K k2) {
		if (k1 == k2) return true;
		auto node1 = m.extract(k1);
		auto node2 = m.extract(k2);
		if (node1.empty() || node2.empty()) {
			if (!node1.empty()) m.insert(move(node1));
			if (!node2.empty()) m.insert(move(node2));
			return false;
		}
		std::swap(node1.key(), node2.key());
		m.insert(move(node1));
		m.insert(move(node2));
		return true;
	}
}