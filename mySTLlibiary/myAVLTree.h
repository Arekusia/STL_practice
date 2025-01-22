#pragma once

#include<cassert>
//AVLTree
namespace nihil {
	//AVL节点
	template <typename K, typename V>
	struct AVLTreeNode {

		pair<K, V> _kv;
		AVLTreeNode<K, V>* _left;
		AVLTreeNode<K, V>* _right;
		AVLTreeNode<K, V>* _parent;
		int _bf;

		AVLTreeNode(const pair<K, V>& kv) : _kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr), _bf(0) {}
	};

	//AVLTree
	template<typename K, typename V>
	class AVLTree {
		using Node = AVLTreeNode<K, V>;

	public:
		AVLTree() : _root(nullptr) {}

		bool insert(const pair<K, V>& kv) {
			if (_root == nullptr) {
				_root = new Node(kv);
				return true;
			}

			Node* curr = _root;
			Node* prev = nullptr;
			//判断左右
			while (curr) {
				if (kv.first > curr->_kv.first) {
					prev = curr;
					curr = curr->_right;
				}
				else if (kv.first < curr->_kv.first) {
					prev = curr;
					curr = curr->_left;
				}
				else {
					return false;
				}

			}
			//插入
			curr = new Node(kv);
			curr->_parent = prev;
			if (kv.first > prev->_kv.first) {
				prev->_right = curr;
			}
			else {
				prev->_left = curr;
			}
			//调整平衡因子
			while (prev) {
				if (curr == prev->_right) {
					++prev->_bf;
				}
				else {
					--prev->_bf;
				}

				if (prev->_bf == 0) {
					break;
				}
				else if (abs(prev->_bf) == 1) {

					curr = curr->_parent;
					prev = prev->_parent;
				}
				else if (abs(prev->_bf) == 2) {
					if (prev->_bf == 2 && curr->_bf == 1) {
						RotateL(prev);
					}
					else if (prev->_bf == -2 && curr->_bf == -1) {
						RotateR(prev);
					}
					else if (prev->_bf == 2 && curr->_bf == -1) {
						RotateRL(prev);
					}
					else if (prev->_bf == -2 && curr->_bf == 1) {
						RotateLR(prev);
					}
					break;
				}
				else {
					assert(false);
				}
			}
			return true;
		}

	private:
		Node* _root;

		void RotateL(Node* prev) {//左单旋
			Node* curr = prev->_right;

			curr->_parent = prev->_parent;

			prev->_right = curr->_left;
			if (curr->_left) {//curr->_left可能为空
				curr->_left->_parent = prev;
			}

			curr->_left = prev;
			prev->_parent = curr;

			if (prev == _root) {
				_root = curr;
			}
			else {
				Node* pprev = curr->_parent;
				if (pprev->_left == prev) {
					pprev->_left = curr;
				}
				else {
					pprev->_right = curr;
				}
			}

			curr->_bf = 0;
			prev->_bf = 0;
		}
		void RotateR(Node* prev) {//右单旋
			Node* curr = prev->_left;

			curr->_parent = prev->_parent;

			prev->_left = curr->_right;
			if (curr->_right) {//curr->_right可能为空
				curr->_right->_parent = prev;
			}

			curr->_right = prev;
			prev->_parent = curr;

			if (prev == _root) {
				_root = curr;
			}
			else {
				Node* pprev = curr->_parent;
				if (pprev->_left == prev) {
					pprev->_left = curr;
				}
				else {
					pprev->_right = curr;
				}
			}

			curr->_bf = 0;
			prev->_bf = 0;
		}
		void RotateRL(Node* prev) {

			Node* curr = prev->_right;
			Node* currLeft = curr->_left;
			int bf = currLeft->_bf;

			RotateR(curr);
			RotateL(prev);

			if (bf == 0) {
				currLeft->_bf = 0;
				curr->_bf = 0;
				prev->_bf = 0;
			}
			else if (bf == -1) {
				currLeft->_bf = 0;
				curr->_bf = 1;
				prev->_bf = 0;
			}
			else if (bf == 1) {
				currLeft->_bf = 0;
				curr->_bf = 0;
				prev->_bf = -1;
			}
			else {
				assert(false);
			}
		}
		void RotateLR(Node* prev) {

			Node* curr = prev->_left;
			Node* currRight = curr->_right;
			int bf = currRight->_bf;

			RotateL(curr);
			RotateR(prev);

			if (bf == 0) {
				currRight->_bf = 0;
				curr->_bf = 0;
				prev->_bf = 0;
			}
			else if (bf == -1) {
				currRight->_bf = 0;
				curr->_bf = 0;
				prev->_bf = 1;
			}
			else if (bf == 1) {
				currRight->_bf = 0;
				curr->_bf = -1;
				prev->_bf = 0;
			}
			else {
				assert(false);
			}
		}
	};

	//测试
	//vector<vector<int>> levelOrder(AVLTreeNode<int, int>* root) {
	//	queue<AVLTreeNode<int, int>*> que;
	//	vector<vector<int>> res;
	//	if (root != nullptr) que.push(root);
	//	while (!que.empty()) {
	//		vector<int> tmp;
	//		for (int i = que.size(); i > 0; --i) {
	//			root = que.front();
	//			que.pop();
	//			tmp.push_back(root->_kv.first);
	//			if (root->_left != nullptr) que.push(root->_left);
	//			if (root->_right != nullptr) que.push(root->_right);
	//		}
	//		res.push_back(tmp);
	//	}
	//	return res;
	//}
	//void InOrder(AVLTreeNode<int, int>* root) {
	//	if (!root) {
	//		return;
	//	}
	//	InOrder(root->_left);
	//	cout << root->_kv.first << " ";
	//	InOrder(root->_right);
	//}
}