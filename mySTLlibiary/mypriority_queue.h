#pragma once

#include<algorithm>
#include"myvector.h"

//priority_queue
namespace nihil {
	template <typename T, typename Container = nihil::vector<T>, typename Compare = std::less<T>>
	class priority_queue {

	public:
		template<typename InputIterator>
		priority_queue(InputIterator first, InputIterator last) {//Ð¡¶Ñ
			while (first != last) {
				_con.push_back(*first);
				++first;
			}

			for (int i = (_con.size() - 2) / 2; i >= 0; --i) {
				adjustdn(i);
			}
		}

		priority_queue() : _con(), _cp() {}

		void push(const T& val) {
			_con.push_back(val);
			adjustup(_con.size() - 1);
		}
		void pop() {
			swap(_con[0], _con[_con.size() - 1]);
			_con.pop_back();
			adjustdn(0);
		}

		const T& top() {
			return _con[0];
		}
		bool empty() {
			return _con.empty();
		}
		size_t size() {
			return _con.size();
		}


	private:
		Container _con;
		Compare _cp;

		void adjustdn(int parent) {
			int child = parent * 2 + 1;

			while (child < _con.size()) {

				if (child + 1 < _con.size() && _cp(_con[child + 1], _con[child])) {
					++child;
				}

				if (_cp(_con[child], _con[parent])) {
					swap(_con[child], _con[parent]);
					parent = child;
					child = parent * 2 + 1;
				}
				else {
					break;
				}
			}
		}
		void adjustup(int child) {
			int parent = (child - 1) / 2;

			while (parent >= 0) {
				if (_cp(_con[child], _con[parent])) {
					swap(_con[child], _con[parent]);
					child = parent;
					parent = (child - 1) / 2;
				}
				else {
					break;
				}
			}
		}
	};
}