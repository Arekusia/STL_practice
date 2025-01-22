#pragma once

#include"myRBTree.h"

namespace nihil {
	template<typename K, typename V>
	class map {
		struct MapKeyOfT {
			const K& operator()(const pair<K, V>& kv) {
				return kv.first;
			}
		};

	public:
		using iterator = typename RBTree<K, pair<const K, V>, MapKeyOfT>::iterator;
		using const_iterator = typename RBTree<K, const pair<K, V>, MapKeyOfT>::const_iterator;

		pair<iterator, bool> insert(const pair<K,V>& kv) {
			return _t.insert(kv);
		}

		V& operator[](const K& key) {
			pair<iterator, bool> ret = _t.insert({ key, V() });
			return ret.first->second;
		}

		iterator begin() {
			return _t.begin();
		}
		iterator end() {
			return _t.end();
		}

		const_iterator begin() const{
			return _t.begin();
		}
		const_iterator end() const{
			return _t.end();
		}

		const_iterator cbegin() const{
			return _t.begin();
		}
		const_iterator cend() const{
			return _t.end();
		}

	private:
		RBTree<K, pair<const K, V>,MapKeyOfT> _t;

	};
}	