#pragma once

namespace open_address {
    enum STATE {
        EMPTY,
        EXIST,
        DELETE
    };

    template<typename K, typename V>
    struct HashData {
        pair<K, V> _kv;
        STATE _state = EMPTY;
    };

    template<typename K>
    struct defaultHashFunc {
        size_t operator()(const K& k) {
            return (size_t)k;
        }
    };

    template<>
    struct defaultHashFunc<string> {
        size_t operator()(const string& str) {
            size_t hash = 0;
            for (auto val : str) {
                hash *= 131;
                hash += (size_t)val;
            }
            return hash;
        }
    };

    template<typename K, typename V, typename HashFunc = defaultHashFunc<K>>
    class HashTable {
    public:
        HashTable() {
            _table.resize(10);
        }

        HashData<const K, V>* find(const K& key) {

            size_t k = hf(key);
            size_t hashi = k % _table.size();
            size_t tk = hf(_table[hashi]._kv.first);

            while (_table[hashi]._state != EMPTY) {
                if (_table[hashi]._state == EXIST && k == tk) {
                    return (HashData<const K, V>*) & _table[hashi];
                }
                ++hashi;
                hashi %= _table.size();
            }
            return nullptr;
        }

        bool insert(const pair<K, V> kv) {

            if (find(hf(kv.first))) {
                return false;
            }

            if ((double)n / (double)_table.size() > 0.7) {// 扩容
                HashTable<K, V, HashFunc> newtable;
                newtable._table.resize(_table.size() * 2);

                for (auto val : _table) {
                    if (val._state == EXIST) {
                        newtable.insert(val._kv);
                    }
                }
                swap(newtable._table, _table);
            }

            size_t k = hf(kv.first);
            size_t hashi = k % _table.size();
            while (_table[hashi]._state == EXIST) {
                ++hashi;
                hashi %= _table.size();
            }

            _table[hashi]._kv = kv;
            _table[hashi]._state = EXIST;
            ++n;

            return true;
        }

        bool erase(const K& key) {
            HashData<const K, V>* target = find(key);

            if (target) {
                target->_state = DELETE;
                return true;
            }
            return false;
        }

    private:
        vector<HashData<K, V>> _table;
        size_t n = 0;// 有效数据的个数
        HashFunc hf;
    };
}

namespace hash_bucket {
    //节点类
    template<typename T>
    struct HashNode {
        HashNode(const T& data) :_data(data), _next(nullptr) {}

        T _data;
        HashNode<T>* _next;
    };
    //Hash运算
    template<typename K>
    struct defaultHashFunc {
        size_t operator()(const K& key) {
            return (size_t)key;
        }
    };
    template<>
    struct defaultHashFunc<string> {
        size_t operator()(const string& str) {
            size_t hashi = 0;

            for (auto val : str) {
                hashi *= 131;
                hashi += (size_t)val;
            }
            return hashi;
        }
    };
    //前置声明
    template<typename K, typename T, typename KeyOfT, typename HashFunc>
    class HashTable;
    //迭代器
    template<typename K, typename T, typename Ref, typename Ptr, typename KeyOfT, typename HashFunc = defaultHashFunc<K>>
    struct HashIterator {
        using self = HashIterator<K, T, Ref, Ptr, KeyOfT, HashFunc>;

        HashNode<T>* _node;
        const HashTable<K, T, KeyOfT, HashFunc>* _hashP;

        HashIterator(HashNode<T>* node, const HashTable<K, T, KeyOfT, HashFunc>* hashP) : _node(node), _hashP(hashP) {}
        HashIterator(const HashIterator<K, T, T&, T*, KeyOfT, HashFunc>& it) : _node(it._node), _hashP(it._hashP){}//普通迭代器转const迭代器的构造/普通迭代器浅拷贝函数
        self& operator++() {
            KeyOfT kot;
            HashFunc hf;

            if (_node->_next) {
                _node = _node->_next;
                return *this;
            }
            else {
                size_t hashi = hf(kot(_node->_data)) % _hashP->_table.size();
                ++hashi;
                while (hashi < _hashP->_table.size()) {
                    if (_hashP->_table[hashi]) {
                        _node = _hashP->_table[hashi];
                        return *this;
                    }
                    else {
                        ++hashi;
                    }
                }
                _node = nullptr;
            }
            return *this;
        }
        bool operator!=(const self& sf) {
            return _node != sf._node;
        }
        bool operator==(const self& sf) {
            return _node == sf._node;
        }
        Ref operator*() {
            return _node->_data;
        }
        Ptr operator->() {
            return &_node->_data;
        }
    };
    //哈希
    template<typename K, typename T,typename KeyOfT, typename HashFunc = defaultHashFunc<K>>
    class HashTable {
    public:
        using Node = HashNode<T>;
        using iterator = HashIterator<K, T, T&, T*, KeyOfT, HashFunc>;
        using const_iterator = HashIterator<K, T, const T&, const T*, KeyOfT, HashFunc>;
        
        template<typename K, typename T, typename Ref, typename Ptr, typename KeyOfT, typename HashFunc>
        friend struct HashIterator;

        HashTable() {
            _table.resize(10, nullptr);
        }
        HashTable(const HashTable<K, T, KeyOfT, HashFunc>& ht) {
            _table.resize(ht._table.size(), nullptr);
            for (auto val : ht._table) {
                while (val) {
                    insert(val->_data);
                    val = val->_next;
                }
            }
        }
        ~HashTable() {
            for (auto val : _table) {
                while (val) {
                    auto next = val->_next;
                    delete val;
                    val = next;
                }
                val = nullptr;
            }
        }

        iterator find(const K& k) {
            size_t key = hf(k);
            size_t hashi = key % _table.size();
            auto curr = _table[hashi];
            while (curr) {
                size_t cmp = hf(kot(curr->_data));
                if (cmp == key) {
                    return iterator(curr, this);
                }
                curr = curr->_next;
            }
            return iterator(nullptr, this);
        }

        pair<iterator, bool> insert(const T& data) {
            iterator it = find(kot(data));
            if (it != end()) {
                return {it, false};
            }

            if (_n >= _table.size()) {
                vector<Node*> newTable;
                newTable.resize(_table.size() * 2, nullptr);
                
                for (Node* val : _table) {
                    Node* curr = val;
                    while (curr) {
                        Node* next = curr->_next;

                        size_t key = hf(kot(curr->_data));
                        size_t hashi = key % newTable.size();
                        curr->_next = newTable[hashi];
                        newTable[hashi] = curr;

                        curr = next;
                    }
                }
                _table.swap(newTable);
            }

            size_t key = hf(kot(data));
            size_t hashi = key % _table.size();
            Node* newNode = new Node(data);

            newNode->_next = _table[hashi];
            _table[hashi] = newNode;
            ++_n;

            return {iterator(newNode, this), true};
        }

        bool erase(const K& key) {
            auto target = find(key);
            if (!target) {
                return false;
            }

            size_t hashi = key % _table.size();
            auto curr = _table[hashi];
            if (curr == target) {
                _table[hashi] = curr->_next;
            }
            else {
                while (curr) {
                    if (curr->_next == target) {
                        curr->_next = target->_next;
                        delete target;
                        return true;
                    }
                    curr = curr->_next;
                }
            }
            --_n;
            return true;
        }

        iterator begin() {
            for (size_t i = 0; i < _table.size(); ++i) {
                if (_table[i]) {
                    return iterator(_table[i], this);
                }
            }
            return iterator(nullptr, this);
        }
        iterator end() {
            return iterator(nullptr, this);
        }
        const_iterator begin() const {
            for (size_t i = 0; i < _table.size(); ++i) {
                if (_table[i]) {
                    return const_iterator(_table[i], this);
                }
            }
            return const_iterator(nullptr, this);
        }
        const_iterator end() const {
            return const_iterator(nullptr, this);
        }
        const_iterator cbegin() const {
            for (size_t i = 0; i < _table.size(); ++i) {
                if (_table[i]) {
                    return const_iterator(_table[i], this);
                }
            }
            return const_iterator(nullptr, this);
        }
        const_iterator cend() const {
            return const_iterator(nullptr, this);
        }

    private:
        vector<Node*> _table;
        size_t _n = 0;
        HashFunc hf;
        KeyOfT kot;
    };
}

//未封装版本，使用KV结构
//namespace hash_bucket {
//    template<typename K, typename V>
//    struct HashNode {
//
//        HashNode(const pair<K, V>& kv) :_kv(kv), _next(nullptr) {}
//
//        pair<K, V> _kv;
//        HashNode<K, V>* _next;
//    };
//
//    template<typename K>
//    struct defaultHashFunc {
//        size_t operator()(const K& key) {
//            return (size_t)key;
//        }
//    };
//
//    template<>
//    struct defaultHashFunc<string> {
//        size_t operator()(const string& str) {
//            size_t hashi = 0;
//
//            for (auto val : str) {
//                hashi *= 131;
//                hashi += (size_t)val;
//            }
//            return hashi;
//        }
//    };
//
//    template<typename K, typename V, typename HashFunc = defaultHashFunc<K>>
//    class HashTable {
//    public:
//        using Node = HashNode<K, V>;
//
//        HashTable() {
//            _table.resize(10, nullptr);
//        }
//        HashTable(const HashTable<K, V>& ht) {
//            _table.resize(ht._table.size(), nullptr);
//            for (auto val : ht._table) {
//                while (val) {
//                    insert(val->_kv);
//                    val = val->_next;
//                }
//            }
//        }
//        ~HashTable() {
//            for (auto val : _table) {
//                while (val) {
//                    auto next = val->_next;
//                    delete val;
//                    val = next;
//                }
//                val = nullptr;
//            }
//        }
//
//
//        Node* find(const K& k) {
//            size_t key = hf(k);
//            size_t hashi = key % _table.size();
//            auto curr = _table[hashi];
//            while (curr) {
//                size_t cmp = hf(curr->_kv.first);
//                if (cmp == key) {
//                    return curr;
//                }
//                curr = curr->_next;
//            }
//            return curr;
//        }
//
//        bool insert(const pair<K, V>& kv) {
//            if (find(kv.first)) {
//                return false;
//            }
//
//            if (_n >= _table.size()) {
//                vector<Node*> newTable;
//                newTable.resize(_table.size() * 2, nullptr);
//
//                for (auto& val : _table) {
//                    auto curr = val;
//                    while (curr) {
//                        auto next = curr->_next;
//
//                        size_t key = hf(curr->_kv.first);
//                        size_t hashi = key % newTable.size();
//                        curr->_next = newTable[hashi];
//                        newTable[hashi] = curr;
//
//                        curr = next;
//                    }
//                }
//
//                _table.swap(newTable);
//            }
//
//            size_t key = hf(kv.first);
//            size_t hashi = key % _table.size();
//            Node* newNode = new Node(kv);
//
//            newNode->_next = _table[hashi];
//            _table[hashi] = newNode;
//            ++_n;
//
//            return true;
//        }
//
//        bool erase(const K& key) {
//            auto target = find(key);
//            if (!target) {
//                return false;
//            }
//
//            size_t hashi = key % _table.size();
//            auto curr = _table[hashi];
//            if (curr == target) {
//                _table[hashi] = curr->_next;
//            }
//            else {
//                while (curr) {
//                    if (curr->_next == target) {
//                        curr->_next = target->_next;
//                        delete target;
//                        return true;
//                    }
//                    curr = curr->_next;
//                }
//            }
//            --_n;
//            return true;
//        }
//
//        void print() {
//            for (int i = 0; i < _table.size(); ++i) {
//                cout << "[" << i << "]: ";
//                auto curr = _table[i];
//                while (curr) {
//                    cout << "{" << curr->_kv.first << ", " << curr->_kv.second << "} -> ";
//                    curr = curr->_next;
//                }
//                cout << "NULL" << endl;
//            }
//        }
//
//    private:
//        vector<Node*> _table;
//        size_t _n = 0;
//        HashFunc hf;
//    };
//}