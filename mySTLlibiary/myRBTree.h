#pragma once

#include<iostream>
using std::cout;
using std::endl;
using std::pair;

//RBTree
namespace nihil {

	//RBTree��ɫö��
	enum color {
		RED,
		BLACK
	};

	//RBtreeNode
	template <typename T>
	struct RBTreeNode {
		RBTreeNode<T>* _left;
		RBTreeNode<T>* _right;
		RBTreeNode<T>* _parent;

		T _data;
		color _col;

		RBTreeNode(T data) : _left(nullptr), _right(nullptr), _parent(nullptr), _data(data), _col(RED) {}
	};

	//RBTreeIterator
	template <typename T, typename Ref, typename Ptr>
	struct __RBTreeIterator {

		using Node = RBTreeNode<T>;
		using self = __RBTreeIterator<T, Ref, Ptr>;

		__RBTreeIterator(Node* node) : _node(node) {}
		__RBTreeIterator(const __RBTreeIterator<T, T&, T*>& it) : _node(it._node){}//��ͨ������תconst�������Ĺ���/��ͨ������ǳ��������

		Ref operator*() {
			return _node->_data;
		}

		Ptr operator->() {
			return &_node->_data;
		}

		bool operator!=(const self& s)const {
			return _node != s._node;
		}
		bool operator==(const self& s)const {
			return _node == s._node;
		}

		self& operator++() {
			if (_node->_right) {
				Node* subleft = _node->_right;
				while (subleft->_left) {
					subleft = subleft->_left;
				}
				_node = subleft;
			}
			else {
				Node* curr = _node;
				Node* parent = _node->_parent;
				while (parent && curr == parent->_right) {
					parent = parent->_parent;
					curr = curr->_parent;
				}
				_node = parent;
			}
			return *this;
		}
		self& operator--() {
			if (_node->_left) {
				Node* subleft = _node->_left;
				while (subleft->_right) {
					subleft = subleft->_right;
				}
				_node = subleft;
			}
			else {
				Node* curr = _node;
				Node* parent = _node->_parent;
				while (parent && curr == parent->_left) {
					parent = parent->_parent;
					curr = curr->_parent;
				}
				_node = parent;
			}
			return *this;
		}

		Node* _node;
	};

	//RBTree
	template <typename K, typename T, typename KeyOfT>
	class RBTree {
		using Node = RBTreeNode<T>;

	public:
		using iterator = __RBTreeIterator<T, T&, T*>;
		using const_iterator = __RBTreeIterator<T, const T&, const T*>;

		RBTree() : _root(nullptr) {}

		Node* find(const K& k) {

			Node* curr = _root;
			KeyOfT kot;

			while (curr) {
				if (k > kot(curr->_data)) {
					curr = curr->_right;
				}
				else if (k < kot(curr->_data)) {
					curr = curr->_left;
				}
				else {
					return curr;
				}
			}
			return nullptr;
		}

		pair<iterator, bool> insert(const T& data) {
			if (_root == nullptr) {
				_root = new Node(data);
				_root->_col = BLACK;
				return { iterator(_root), true };
			}

			Node* curr = _root;
			Node* parent = nullptr;
			KeyOfT kot;//��ȡkey
			//�ж�����
			while (curr) {
				if (kot(data) > kot(curr->_data)) {
					parent = curr;
					curr = curr->_right;
				}
				else if (kot(data) < kot(curr->_data)) {
					parent = curr;
					curr = curr->_left;
				}
				else {
					{
						return { iterator(curr), false };
					}
				}
			}
			//����
			curr = new Node(data);
			Node* retNode = curr;//�������淵��ֵ
			curr->_col = RED;
			curr->_parent = parent;
			if (kot(data) > kot(parent->_data)) {
				parent->_right = curr;
			}
			else {
				parent->_left = curr;
			}

			//����
			while (parent && parent->_col == RED) {
				Node* grandparent = parent->_parent;

				if (parent == grandparent->_left) {

					Node* uncle = grandparent->_right;
					if (uncle && uncle->_col == RED) {//uncle������Ϊ��
						uncle->_col = BLACK;
						parent->_col = BLACK;
						grandparent->_col = RED;
						curr = grandparent;
						parent = curr->_parent;
					}
					else {//uncleΪ�ڻ򲻴���
						if (curr == parent->_left) {//�ҵ���
							//    g
							//  p
							//c
							RotateR(grandparent);
							parent->_col = BLACK;
							grandparent->_col = RED;

						}
						else {//curr == parent->_left	����˫��
							//    g
							//  p
							//    c
							RotateL(parent);
							RotateR(grandparent);

							curr->_col = BLACK;
							grandparent->_col = RED;
						}
						break;
					}

				}
				else {//parent == grandparent->_right

					Node* uncle = grandparent->_left;
					if (uncle && uncle->_col == RED) {//uncle������Ϊ��
						uncle->_col = BLACK;
						parent->_col = BLACK;
						grandparent->_col = RED;
						curr = grandparent;
						parent = curr->_parent;
					}
					else {//uncleΪ�ڻ򲻴���
						if (curr == parent->_right) {//����
							//g
							//  p
							//    c
							RotateL(grandparent);
							parent->_col = BLACK;
							grandparent->_col = RED;
						}
						else {//curr == parent->_right	����˫��
							//g
							//  p
							//c
							RotateR(parent);
							RotateL(grandparent);

							curr->_col = BLACK;
							grandparent->_col = RED;
						}
						break;
					}
				}
			}
			_root->_col = BLACK;//ȷ�����������ʼ��Ϊ��
			return { iterator(retNode), true };
		}

		bool checkColor(Node* root, int blackNodes, int blackStandard) {
			if (!root) {
				if (blackNodes != blackStandard) {
					return false;
				}
				return true;
			}

			if (root->_col == BLACK) {
				++blackNodes;
			}

			if (root->_col == RED && root->_parent && root->_parent->_col == RED) {
				cout << "color error" << endl;
				return false;
			}

			return checkColor(root->_left, blackNodes, blackStandard)
				&& checkColor(root->_right, blackNodes, blackStandard);
		}
		bool isBalance() {
			if (!_root) {
				return true;
			}

			if (_root->_col == RED) {
				return false;
			}

			//��ɫ�ڵ�������׼ֵ
			int blackStandard = 0;
			Node* curr = _root;
			while (curr) {
				if (curr->_col == BLACK) {
					++blackStandard;
				}
				curr = curr->_left;
			}

			return checkColor(_root, 0, blackStandard);
		}

		iterator begin() {
			Node* left = _root;
			while (left && left->_left) {
				left = left->_left;
			}
			return iterator(left);
		}
		iterator end() {
			return iterator(nullptr);
		}
		const_iterator begin() const{
			Node* left = _root;
			while (left && left->_left) {
				left = left->_left;
			}
			return const_iterator(left);
		}
		const_iterator end() const{
			return const_iterator(nullptr);
		}



	private:
		Node* _root;

		void RotateL(Node* parent) {//����
			Node* curr = parent->_right;

			curr->_parent = parent->_parent;

			parent->_right = curr->_left;
			if (curr->_left) {//curr->_left����Ϊ��
				curr->_left->_parent = parent;
			}

			curr->_left = parent;
			parent->_parent = curr;

			if (parent == _root) {
				_root = curr;
			}
			else {
				Node* grandparent = curr->_parent;
				if (grandparent->_left == parent) {
					grandparent->_left = curr;
				}
				else {
					grandparent->_right = curr;
				}
			}
		}
		void RotateR(Node* parent) {//�ҵ���
			Node* curr = parent->_left;

			curr->_parent = parent->_parent;

			parent->_left = curr->_right;
			if (curr->_right) {//curr->_right����Ϊ��
				curr->_right->_parent = parent;
			}

			curr->_right = parent;
			parent->_parent = curr;

			if (parent == _root) {
				_root = curr;
			}
			else {
				Node* grandparent = curr->_parent;
				if (grandparent->_left == parent) {
					grandparent->_left = curr;
				}
				else {
					grandparent->_right = curr;
				}
			}
		}
	};



	//����(�����ڳ�ʼ��RBTree,�����������map,set���޸İ汾,��ͬ)
	//vector<int> v = { 30, 20, 10, 5, 25, 40, 35, 50, 15, 45, 60 };
	//for (auto val : v) {
	//	tree.insert({ val,val });
	//}
	////random_device seed;
	////mt19937_64 engine(seed());
	////uniform_int_distribution<> distrib(1, 100000);
	////for (int i = 0; i < 10000; ++i) {
	////	int randnum = distrib(engine);
	////	tree.insert({ randnum, randnum });
	////}
	//�������
	//vector<vector<pair<int, string>>> res = {};
	//res = levelOrder(tree._root);
	//for (auto& val1 : res) {
	//	for (auto val2 : val1) {
	//		cout << val2.first << " :" << val2.second << "    ";
	//	}cout << endl;
	//}cout << endl;
	//InOrder(tree._root);
	//cout << endl;
	//if (tree.isBalance()) {
	//	cout << "yes" << endl;
	//}
	//else {
	//	cout << " no " << endl;
	//}

	//vector<vector<pair<int, string>>> levelOrder(RBTreeNode<int, int>* root) {
	//	queue<RBTreeNode<int, int>*> que;
	//	vector<vector<pair<int, string>>> res;
	//	if (root != nullptr) que.push(root);
	//	while (!que.empty()) {
	//		vector<pair<int, string>> tmp;
	//		for (int i = que.size(); i > 0; --i) {
	//			root = que.front();
	//			que.pop();
	//			string col = root->_col == RED ? "red" : "black";;
	//			tmp.push_back({ root->_kv.first, col });
	//			if (root->_left != nullptr) que.push(root->_left);
	//			if (root->_right != nullptr) que.push(root->_right);
	//		}
	//		res.push_back(tmp);
	//	}
	//	return res;
	//}
	//void InOrder(RBTreeNode<int, int>* root) {
	//	if (!root) {
	//		return;
	//	}
	//	InOrder(root->_left);
	//	cout << root->_kv.first << " ";
	//	InOrder(root->_right);
	//}
}