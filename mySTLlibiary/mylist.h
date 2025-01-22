#pragma once

#include<cassert>
#include"normal_iterator.h"
//list
namespace nihil {

	template<typename T>
	struct list_node {
		list_node* _prev;
		list_node* _next;
		T _val;
		list_node() :_next(nullptr), _prev(nullptr), _val(T()) {}
		list_node(const T& val) :_next(nullptr), _prev(nullptr), _val(val) {}

		list_node(T&& val) :_next(nullptr), _prev(nullptr), _val(forward<T>(val)) {}

		template<typename... Args>
		list_node(Args&&... args) : _next(nullptr), _prev(nullptr), _val(forward<Args>(args)...) {}
	};

	template<typename T>
	class list {
		using Node = list_node<T>;
	public:

		using iterator = _list_iterator<Node,T, T&, T*>;
		using const_iterator = _list_iterator<Node, T, const T&, const T*>;
		using const_reverse_iterator = reverse_iterator<const_iterator, const T&, const T*>;//编译器原因反向const迭代器要在反向迭代器之前
		using reverse_iterator = reverse_iterator<iterator, T&, T*>;

		list() {
			empty_init();
		}
		list(const list<T>& lt) {
			empty_init();

			for (auto& val : lt) {
				push_back(val);
			}
		}
		list(size_t n, const T& val) {
			empty_init();
			for (size_t i = 0; i < n; ++i) {
				{
					push_back(val);
				}
			}
		}
		template<typename InputIterator>
		list(InputIterator first, InputIterator last) {
			empty_init();
			while (first != last) {
				push_back(*first);
				++first;
			}
		}
		~list() {
			clear();
			delete _head;
			_head = nullptr;

		}
		void swap(const list<T>& lt) {
			std::swap(_head, lt._head);
			std::swap(_size, lt._size);
		}
		list<T>& operator=(list<T> lt) {
			swap(lt);
			return *this;
		}

		void empty_init() {
			_head = new Node;
			_head->_prev = _head;
			_head->_next = _head;
			_size = 0;
		}
		void clear() {

			auto cur = begin();
			while (cur != end()) {
				cur = erase(cur);
			}
		}
		bool empty() {
			return begin() == end();
		}
		size_t size() {
			return _size;
		}

		void push_back(const T& val) {
			//Node* newnode = new Node(val);

			//newnode->_next = _head;
			//newnode->_prev = _head->_prev;
			//_head->_prev->_next = newnode;
			//_head->_prev = newnode;

			insert(end(), val);
		}
		void push_back(T&& val) {
			insert(end(), move(val));
		}
		template<typename... Args>
		void emplace_back(Args&&... args) {
			insert(end(), forward<Args>(args)...);
		}

		void push_front(const T& val) {
			insert(begin(), val);
		}
		void push_front(T&& val) {
			insert(begin(), move(val));
		}
		void pop_back() {
			erase(end()._node->_prev);
		}
		void pop_front() {
			erase(begin());
		}
		iterator insert(iterator pos, const T& val) {
			Node* newnode = new Node(val);

			newnode->_next = pos._node;
			newnode->_prev = pos._node->_prev;

			pos._node->_prev->_next = newnode;
			pos._node->_prev = newnode;

			++_size;
			return newnode;
		}
		iterator insert(iterator pos, T&& val) {
			Node* newnode = new Node(move(val));

			newnode->_next = pos._node;
			newnode->_prev = pos._node->_prev;

			pos._node->_prev->_next = newnode;
			pos._node->_prev = newnode;

			++_size;
			return newnode;
		}
		template<typename... Args>
		iterator insert(iterator pos, Args&&... args) {
			Node* newnode = new Node(forward<Args>(args)...);

			newnode->_next = pos._node;
			newnode->_prev = pos._node->_prev;

			pos._node->_prev->_next = newnode;
			pos._node->_prev = newnode;

			++_size;
			return newnode;
		}
		iterator erase(iterator pos) {
			assert(pos != end());
			iterator nt = pos._node->_next;

			pos._node->_prev->_next = pos._node->_next;
			pos._node->_next->_prev = pos._node->_prev;

			delete pos._node;
			--_size;
			return nt;
		}

		//一般版本
		iterator begin() {
			return iterator(_head->_next);
		}
		iterator end() {
			return iterator(_head);
		}
		reverse_iterator rbegin() {
			return end();
		}
		reverse_iterator rend() {
			return begin();
		}
		//const重载版本,针对函数参数列表的const,在内部即使使用普通迭代器也不可修改()
		const_iterator begin() const {
			return const_iterator(_head->_next);
		}
		const_iterator end() const {
			return const_iterator(_head);
		}
		const_reverse_iterator rbegin() const {
			return cend();
		}
		const_reverse_iterator rend() const {
			return cbegin();
		}
		//一般const
		const_iterator cbegin() const {
			return const_iterator(_head->_next);
		}
		const_iterator cend() const {
			return const_iterator(_head);
		}
		const_reverse_iterator crbegin() const {
			return cend();
		}
		const_reverse_iterator crend() const {
			return cbegin();
		}

	private:
		Node* _head;
		size_t _size;
	};

}