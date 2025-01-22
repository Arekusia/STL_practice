#pragma once

//iterator
namespace nihil {

	//Node��֤������ǰ����list_node����ʹ��
	template<typename Node,typename T, typename Ref, typename Ptr>
	struct _list_iterator {
		using self = _list_iterator<Node,T, Ref, Ptr>;

		Node* _node;

		_list_iterator(Node* node) : _node(node) {}
		_list_iterator(const _list_iterator<Node, T, T&, T*>& it) : _node(it._node){}//��ͨ������תconst�������Ĺ���/��ͨ������ǳ��������

		Ref operator*() {
			return _node->_val;
		}
		Ptr operator->() {//�����ڶ���Ϊ�Զ���ṹ,ֱ�ӷ��ʽṹ�ڲ�
			return &(_node->_val);
		}

		self& operator++() {//ǰ��
			_node = _node->_next;
			return *this;
		}
		self& operator++(int) {//����
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