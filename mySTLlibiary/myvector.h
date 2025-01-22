#pragma once

#include<cassert>
#include"normal_iterator.h"
//vector
namespace nihil {

	template<typename T>
	class vector {
	public:
		using iterator = T*;
		using const_iterator = const T*;
		//反向迭代器使用normal_iterator.h
		using const_reverse_iterator = reverse_iterator<const_iterator, const T&, const T*>;//编译器原因反向const迭代器要在反向迭代器之前
		using reverse_iterator = reverse_iterator<iterator, T&, T*>;

		vector() :_start(nullptr), _finish(nullptr), _end_of_storge(nullptr) {}
		vector(int n, const T& val = T()) :_start(nullptr), _finish(nullptr), _end_of_storge(nullptr) {
			resize(n, val);
		}
		vector(const vector<T>& v) :_start(nullptr), _finish(nullptr), _end_of_storge(nullptr) {
			_start = new T[v.size()];
			//memcpy(_start, v._start, sizeof(T) * v.size());
			for (size_t i = 0; i < v.size(); ++i) {
				_start[i] = v[i];
			}
			_finish = _start + v.size();
			_end_of_storge = _start + v.size();
		}
		template<typename InputIterator>
		vector(InputIterator first, InputIterator last) :_start(nullptr), _finish(nullptr), _end_of_storge(nullptr) {
			while (first != last) {
				push_back(*first);
				++first;
			}
		}
		~vector() {
			if (_start) {

				delete[] _start;
			}
			_start = nullptr;
			_finish = nullptr;
			_end_of_storge = nullptr;
		}

		void swap(vector<T>& v) {
			if (&v == this) {
				return;
			}
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_end_of_storge, v._end_of_storge);
		}
		vector<T>& operator=(vector<T> v) {
			swap(v);
			return *this;
		}

		void reserve(size_t new_capacity) {
			if (new_capacity > capacity()) {

				size_t old_size = size();
				T* tmp = new T[new_capacity];
				if (_start) {
					//memcpy(tmp, _start, sizeof(T) * size());
					for (size_t i = 0; i < old_size; ++i) {
						tmp[i] = _start[i];
					}
					delete[] _start;
				}
				_start = tmp;
				_finish = _start + old_size;
				_end_of_storge = _start + new_capacity;
			}
		}
		void resize(size_t n, const T& val = T()) {
			if (n < size()) {
				_finish = _start + n;
			}
			else {
				reserve(n);
				while (_finish < _start + n) {
					*_finish = val;
					++_finish;
				}
			}
		}
		size_t size() const {
			return _finish - _start;
		}
		size_t capacity() const {
			return _end_of_storge - _start;
		}
		bool empty() {
			return size() == 0;
		}

		void push_back(T val) {
			if (_finish == _end_of_storge || _start == nullptr) {
				size_t new_capacity = (capacity() == 0) ? 4 : 2 * capacity();
				reserve(new_capacity);
			}
			*_finish = val;
			++_finish;
			//insert(end(), val);
		}
		void pop_back() {
			erase(end() - 1);
		}
		iterator insert(iterator pos, const T& val) {
			assert(pos >= _start && pos <= _finish);

			if (_finish == _end_of_storge || _start == nullptr) {
				size_t ipos = pos - _start;
				size_t new_capacity = (capacity() == 0) ? 4 : 2 * capacity();
				reserve(new_capacity);
				pos = _start + ipos;
			}
			auto end = _finish;
			while (size() && end >= pos) {
				*end = *(end - 1);
				--end;
			}
			*pos = val;
			++_finish;
			return pos;
		}
		iterator erase(iterator pos) {
			assert(pos >= _start && pos < _finish);

			auto end = pos;
			while (end < _finish) {
				*end = *(end + 1);
				++end;
			}
			--_finish;

			return pos;
		}

		T& operator[](size_t pos) {
			assert(pos < size());
			return _start[pos];
		}
		const T& operator[](size_t pos) const {
			assert(pos < size());
			return _start[pos];
		}

		//一般版本
		iterator begin() {
			return iterator(_start);
		}
		iterator end() {
			return iterator(_finish);
		}
		reverse_iterator rbegin() {
			return end();
		}
		reverse_iterator rend() {
			return begin();
		}
		//const重载版本
		const_iterator begin() const {
			return const_iterator(_start);
		}
		const_iterator end() const {
			return const_iterator(_finish);
		}
		const_reverse_iterator rbegin() const {
			return cend();
		}
		const_reverse_iterator rend() const {
			return cbegin();
		}
		//一般const
		const_iterator cbegin() const {
			return const_iterator(_start);
		}
		const_iterator cend() const {
			return const_iterator(_finish);
		}
		const_reverse_iterator crbegin() const {
			return cend();
		}
		const_reverse_iterator crend() const {
			return cbegin();
		}

	private:
		iterator _start;
		iterator _finish;
		iterator _end_of_storge;
	};
}