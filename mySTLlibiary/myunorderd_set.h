#pragma once
#include"Hash.h"

namespace  nihil {
	template<typename K>
	class unordered_set {
		struct setKeyOfT {
			K operator()(const K& key) {
				return key;
			}
		};
	public:
		using iterator = typename hash_bucket::HashTable<K, K, setKeyOfT>::const_iterator;
		using const_iterator = typename hash_bucket::HashTable<K, K, setKeyOfT>::const_iterator;

		pair<iterator, bool> insert(const K& key) {
			pair<typename hash_bucket::HashTable<K, K, setKeyOfT>::iterator, bool> ret = _ht.insert(key);
			return pair<iterator, bool>(ret.first, ret.second);
		}

		iterator begin()  {
			return _ht.cbegin();
		}
		iterator end()  {
			return _ht.cend();
		}

	private:
		hash_bucket::HashTable<K, K, setKeyOfT> _ht;
	};
}
