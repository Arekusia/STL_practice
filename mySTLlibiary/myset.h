#pragma once

#include"myRBTree.h"

namespace nihil {
	template<typename K>
	class set {
		struct SetKeyOfT {
			const K& operator()(const K& k) {
				return k;	
			}
		};

	public:
		using iterator = typename RBTree<K, K, SetKeyOfT>::const_iterator;
		using const_iterator = typename RBTree<K, K, SetKeyOfT>::const_iterator;

		pair<iterator, bool> insert(const K& k) {
			pair<typename RBTree<K, K, SetKeyOfT>::iterator, bool> ret =  _t.insert(k);
			return pair<iterator, bool>(ret.first, ret.second);
		}

		iterator begin() const{
			return _t.begin();
		}
		iterator end() const{
			return _t.end();
		}

		const_iterator cbegin() const{
			return _t.begin();
		}
		const_iterator cend() const{
			return _t.end();
		}



	private:
		RBTree<K, K,SetKeyOfT> _t;

	};
}