#pragma once

//iterator
namespace nihil {

	//Node保证无需提前声明list_node即可使用
	template<typename Node,typename T, typename Ref, typename Ptr>
	struct _list_iterator {
		using self = _list_iterator<Node,T, Ref, Ptr>;

		Node* _node;

		_list_iterator(Node* node) : _node(node) {}
		_list_iterator(const _list_iterator<Node, T, T&, T*>& it) : _node(it._node){}//普通迭代器转const迭代器的构造/普通迭代器浅拷贝函数

		Ref operator*() {
			return _node->_val;
		}
		Ptr operator->() {//适用于对象为自定义结构,直接访问结构内部
			return &(_node->_val);
		}

		self& operator++() {//前置
			_node = _node->_next;
			return *this;
		}
		self& operator++(int) {//后置
			self tmp(*this);
			_node = _node->_next;
			return tmp;
		}

		self& operator--() {
			_node = _node->_prev;
			return *this;
		}
		self& operator--(int) {
			self tmp(*this);
			_node = _node->_prev;
			return tmp;
		}

		bool operator!=(const self it) const {
			return _node != it._node;
		}
		bool operator==(const self it) const {
			return _node == it._node;
		}

	};

	template<typename Iterator, typename Ref, typename Ptr>
	struct reverse_iterator {
		using self = reverse_iterator<Iterator, Ref, Ptr> ;

		Iterator _Rit;

		reverse_iterator(Iterator it) : _Rit(it) {}

		Ref operator*() {
			Iterator tmp = _Rit;
			return *(--tmp);
		}
		Ptr operator->() {
			Iterator tmp = _Rit;
			return &(*(--tmp));
		}

		self& operator++() {
			--_Rit;
			return *this;
		}
		self& operator++(int) {
			self tmp(*this);
			--_Rit;
			return tmp;
		}
		self& operator--() {
			++_Rit;
			return *this;
		}
		self& operator--(int) {
			self tmp(*this);
			++_Rit;
			return tmp;
		}

		bool operator!=(const self& it) const {
			return _Rit != it._Rit;
		}
		bool operator==(const self& it) const {
			return _Rit == it._Rit;
		}
	};
}