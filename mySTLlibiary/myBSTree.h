#pragma once

#include<queue>
//binary_search_tree
namespace nihil {
	//value版本
	template<typename K>
	struct BSTreeNode {

		BSTreeNode* _left;
		BSTreeNode* _right;
		K _key;

		BSTreeNode(const K& val) :_left(nullptr), _right(nullptr), _key(val) {}

	};

	template<typename K>
	class BSTree {
		using Node = BSTreeNode<K> ;
	public:
		BSTree() : _root(nullptr) {};
		BSTree(const BSTree<K>& bst) :_root(nullptr) {

			//queue<Node*> lib;
			//lib.push(bst._root);
			//while (!lib.empty()) {

			//    Node* top = lib.front();
			//    insert(top->_key);
			//    lib.pop();

			//    if (top->_left) {
			//        lib.push(top->_left);
			//    }
			//    if (top->_right) {
			//        lib.push(top->_right);
			//    }
			//}

			_root = _copy(bst._root);
		}
		~BSTree() {
			_destroy(_root);
		}
		BSTree<K>& operator=(BSTree<K> bst) {
			swap(_root, bst._root);
			return *this;
		}

		bool insert(const K& val) {
			if (!_root) {
				_root = new Node(val);
				return true;
			}

			Node* cur = _root;
			Node* pre = nullptr;

			while (cur) {
				if (val > cur->_key) {
					pre = cur;
					cur = cur->_right;
				}
				else if (val < cur->_key) {
					pre = cur;
					cur = cur->_left;
				}
				else {
					return false;
				}
			}

			if (val > pre->_key) {
				pre->_right = new Node(val);
			}
			if (val < pre->_key) {
				pre->_left = new Node(val);
			}
			return true;
		}
		bool erase(const K& val) {

			Node* cur = _root;
			Node* pre = nullptr;

			while (cur) {
				if (val > cur->_key) {
					pre = cur;
					cur = cur->_right;
				}
				else if (val < cur->_key) {
					pre = cur;
					cur = cur->_left;
				}
				else {
					if (cur->_left && cur->_right) {//双孩子
						Node* lmax = cur->_left;
						pre = cur;

						while (lmax->_right) {
							pre = lmax;
							lmax = lmax->_right;
						}
						swap(cur->_key, lmax->_key);

						if (pre->_left == lmax) {//直接链接父节点
							pre->_left = lmax->_left;
						}
						else {
							pre->_right = lmax->_left;
						}

						delete lmax;
						lmax = nullptr;
					}
					else {//单孩子或无孩子

						Node* child = cur->_left == nullptr ? cur->_right : cur->_left;//取左右非空

						if (pre == nullptr) {//处理根节点
							_root = child;
						}
						else if (pre->_left == cur) {
							pre->_left = child;
						}
						else {
							pre->_right = child;
						}
						delete cur;
						cur = nullptr;
					}
					return true;
				}
			}
			return false;
		}
		bool find(const K& val) {

			if (!_root) {
				return false;
			}

			Node* cur = _root;
			while (cur) {
				if (val > cur->_key) {
					cur = cur->_right;
				}
				else if (val < cur->_key) {
					cur = cur->_left;
				}
				else {
					return true;
				}
			}
			return false;
		}

		bool insert_rever(const K& val) {
			return _insert_rever(_root, val);
		}
		bool erase_rever(const K& val) {
			return _erase_rever(_root, val);
		}
		bool find_rever(const K& val) {
			return _find_rever(_root, val);
		}

		void NLR() {
			_NLR(_root);
			cout << endl;
		}
		void level() {
			if (!_root) {
				return;
			}

			queue<Node*> lib;

			lib.push(_root);

			while (!lib.empty()) {

				Node* top = lib.front();
				cout << top->_key << "  ";
				lib.pop();

				if (top->_left) {
					lib.push(top->_left);
				}
				if (top->_right) {
					lib.push(top->_right);
				}
			}
			cout << endl;
		}


	private:
		void _NLR(const Node* root) {
			if (!root) {
				return;
			}

			_NLR(root->_left);
			cout << root->_key << "  ";
			_NLR(root->_right);
		}

		bool _insert_rever(Node*& root, const K& val) {
			if (!root) {
				root = new Node(val);
				return true;
			}

			if (val > root->_key) {
				return _insert_rever(root->_right, val);
			}
			else if (val < root->_key) {
				return _insert_rever(root->_left, val);
			}
			else {
				return false;
			}
		}
		bool _erase_rever(Node*& root, const K& val) {
			if (!root) {
				return false;
			}

			if (val > root->_key) {
				return _erase_rever(root->_right, val);
			}
			else if (val < root->_key) {
				return _erase_rever(root->_left, val);
			}
			else {
				if (root->_left && root->_right) {
					Node* lmax = root->_left;
					Node* prev = root;
					while (lmax->_right) {
						prev = lmax;
						lmax = lmax->_right;
					}
					swap(root->_key, lmax->_key);//转化为删除非双孩子节点
					return _erase_rever(root->_left, val);
					//if (prev->_left == lmax) {
					//    prev->_left = lmax->_left;
					//}
					//else {
					//    prev->_right = lmax->_left;
					//}
					//delete lmax;
				}
				else {
					Node* child = root->_left == nullptr ? root->_right : root->_left;

					delete root;
					root = child;
				}
				return true;
			}

		}
		bool _find_rever(Node* root, const K& val) {
			if (!root) {
				return false;
			}

			if (val > root->_key) {
				return _find_rever(root->_right, val);
			}
			else if (val < root->_key) {
				return _find_rever(root->_left, val);
			}
			else {
				return true;
			}
		}
		Node* _copy(Node* root) {
			if (!root) {
				return nullptr;
			}

			Node* newnode = new Node(root->_key);
			newnode->_left = _copy(root->_left);
			newnode->_right = _copy(root->_right);
			return newnode;
		}
		void _destroy(Node*& root) {
			if (!root) {
				return;
			}

			_destroy(root->_left);
			_destroy(root->_right);

			delete root;
			root = nullptr;
		}

		Node* _root;
	};

	//key-value版本
	//template<typename K, typename V>
	//struct BSTreeNode {

	//	BSTreeNode* _left;
	//	BSTreeNode* _right;
	//	K _key;
	//	V _val;

	//	BSTreeNode(const K& key, const V& val) :_left(nullptr), _right(nullptr), _key(key), _val(val) {}
	//};

	//template<typename K, typename V>
	//class BSTree {
	//	using Node = BSTreeNode<K, V>;
	//public:
	//	BSTree() : _root(nullptr) {};
	//	BSTree(const BSTree<K, V>& bst) :_root(nullptr) {

	//		//queue<Node*> lib;
	//		//lib.push(bst._root);
	//		//while (!lib.empty()) {

	//		//    Node* top = lib.front();
	//		//    insert(top->_key);
	//		//    lib.pop();

	//		//    if (top->_left) {
	//		//        lib.push(top->_left);
	//		//    }
	//		//    if (top->_right) {
	//		//        lib.push(top->_right);
	//		//    }
	//		//}

	//		_root = _copy(bst._root);
	//	}
	//	~BSTree() {
	//		_destroy(_root);
	//	}
	//	BSTree<K, V>& operator=(BSTree<K, V> bst) {
	//		swap(_root, bst._root);
	//		return *this;
	//	}

	//	bool insert(const K& key, const V& val) {
	//		if (!_root) {
	//			_root = new Node(key, val);
	//			return true;
	//		}

	//		Node* cur = _root;
	//		Node* pre = nullptr;

	//		while (cur) {
	//			if (key > cur->_key) {
	//				pre = cur;
	//				cur = cur->_right;
	//			}
	//			else if (key < cur->_key) {
	//				pre = cur;
	//				cur = cur->_left;
	//			}
	//			else {
	//				return false;
	//			}
	//		}

	//		if (key > pre->_key) {
	//			pre->_right = new Node(key, val);
	//		}
	//		if (key < pre->_key) {
	//			pre->_left = new Node(key, val);
	//		}
	//		return true;
	//	}
	//	bool erase(const K& key) {

	//		Node* cur = _root;
	//		Node* pre = nullptr;

	//		while (cur) {
	//			if (key > cur->_key) {
	//				pre = cur;
	//				cur = cur->_right;
	//			}
	//			else if (key < cur->_key) {
	//				pre = cur;
	//				cur = cur->_left;
	//			}
	//			else {
	//				if (cur->_left && cur->_right) {//双孩子
	//					Node* lmax = cur->_left;
	//					pre = cur;

	//					while (lmax->_right) {
	//						pre = lmax;
	//						lmax = lmax->_right;
	//					}
	//					swap(cur->_key, lmax->_key);

	//					if (pre->_left == lmax) {//直接链接父节点
	//						pre->_left = lmax->_left;
	//					}
	//					else {
	//						pre->_right = lmax->_left;
	//					}

	//					delete lmax;
	//					lmax = nullptr;
	//				}
	//				else {//单孩子或无孩子

	//					Node* child = cur->_left == nullptr ? cur->_right : cur->_left;//取左右非空

	//					if (pre == nullptr) {//处理根节点
	//						_root = child;
	//					}
	//					else if (pre->_left == cur) {
	//						pre->_left = child;
	//					}
	//					else {
	//						pre->_right = child;
	//					}
	//					delete cur;
	//					cur = nullptr;
	//				}
	//				return true;
	//			}
	//		}
	//		return false;
	//	}
	//	Node* find(const K& key) {

	//		if (!_root) {
	//			return nullptr;
	//		}

	//		Node* cur = _root;
	//		while (cur) {
	//			if (key > cur->_key) {
	//				cur = cur->_right;
	//			}
	//			else if (key < cur->_key) {
	//				cur = cur->_left;
	//			}
	//			else {
	//				return cur;
	//			}
	//		}
	//		return nullptr;
	//	}

	//	bool insert_rever(const K& key, const V& val) {
	//		return _insert_rever(_root, key, val);
	//	}
	//	bool erase_rever(const K& key) {
	//		return _erase_rever(_root, key);
	//	}
	//	Node* find_rever(const K& key) {
	//		return _find_rever(_root, key);
	//	}

	//	void NLR() {
	//		_NLR(_root);
	//		cout << endl;
	//	}
	//	void level() {
	//		if (!_root) {
	//			return;
	//		}

	//		queue<Node*> lib;
	//		lib.push(_root);

	//		while (!lib.empty()) {

	//			Node* top = lib.front();
	//			cout << top->_key << ": " << top->_val << "  ";
	//			lib.pop();

	//			if (top->_left) {
	//				lib.push(top->_left);
	//			}
	//			if (top->_right) {
	//				lib.push(top->_right);
	//			}
	//		}
	//		cout << endl;
	//	}

	//private:
	//	void _NLR(const Node* root) {
	//		if (!root) {
	//			return;
	//		}

	//		_NLR(root->_left);
	//		cout << root->_key << ": " << root->_val << "  ";
	//		_NLR(root->_right);
	//	}
	//	bool _insert_rever(Node*& root, const K& key, const V& val) {
	//		if (!root) {
	//			root = new Node(key, val);
	//			return true;
	//		}

	//		if (key > root->_key) {
	//			return _insert_rever(root->_right, key, val);
	//		}
	//		else if (key < root->_key) {
	//			return _insert_rever(root->_left, key, val);
	//		}
	//		else {
	//			return false;
	//		}
	//	}
	//	bool _erase_rever(Node*& root, const K& key) {
	//		if (!root) {
	//			return false;
	//		}

	//		if (key > root->_key) {
	//			return _erase_rever(root->_right, key);
	//		}
	//		else if (key < root->_key) {
	//			return _erase_rever(root->_left, key);
	//		}
	//		else {
	//			if (root->_left && root->_right) {
	//				Node* lmax = root->_left;
	//				Node* prev = root;
	//				while (lmax->_right) {
	//					prev = lmax;
	//					lmax = lmax->_right;
	//				}
	//				swap(root->_key, lmax->_key);//转化为删除非双孩子节点
	//				return _erase_rever(root->_left, key);
	//				//if (prev->_left == lmax) {
	//				//    prev->_left = lmax->_left;
	//				//}
	//				//else {
	//				//    prev->_right = lmax->_left;
	//				//}
	//				//delete lmax;
	//			}
	//			else {
	//				Node* child = root->_left == nullptr ? root->_right : root->_left;

	//				delete root;
	//				root = child;
	//			}
	//			return true;
	//		}

	//	}
	//	Node* _find_rever(Node* root, const K& key) {
	//		if (!root) {
	//			return nullptr;
	//		}

	//		if (key > root->_key) {
	//			return _find_rever(root->_right, key);
	//		}
	//		else if (key < root->_key) {
	//			return _find_rever(root->_left, key);
	//		}
	//		else {
	//			return root;
	//		}
	//	}
	//	Node* _copy(Node* root) {
	//		if (!root) {
	//			return nullptr;
	//		}

	//		Node* newnode = new Node(root->_key, root->_val);
	//		newnode->_left = _copy(root->_left);
	//		newnode->_right = _copy(root->_right);
	//		return newnode;
	//	}
	//	void _destroy(Node*& root) {
	//		if (!root) {
	//			return;
	//		}

	//		_destroy(root->_left);
	//		_destroy(root->_right);

	//		delete root;
	//		root = nullptr;
	//	}

	//	Node* _root;
	//};
}