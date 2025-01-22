#pragma once
#include"Hash.h"

namespace  nihil {
	template<typename K,typename V>
	class unordered_map {
		struct mapKeyOfT {
			K operator()(const pair<const K, V>& kv) {
				return kv.first;
			}
		};
	public:
		using iterator = typename hash_bucket::HashTable<K, pair<const K, V>, mapKeyOfT>::iterator;
		using const_iterator = typename hash_bucket::HashTable<K, pair<const K, V>, mapKeyOfT>::const_iterator;

		pair<iterator, bool> insert(const pair<const K, V>& kv) {
			return _ht.insert(kv);
		}

		V& operator[](const K& key) {
			pair<iterator, bool> ret = _ht.insert({ key, V() });
			return ret.first->second;
		}

		iterator begin() {
			return _ht.begin();
		}
		iterator end() {
			return _ht.end();
		}

		const_iterator begin() const {
			return _ht.cbegin();
		}
		const_iterator end() const {
			return _ht.cend();
		}

	private:
		hash_bucket::HashTable<K, pair<const K, V>, mapKeyOfT> _ht;
	};
}