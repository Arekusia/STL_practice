#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <cassert>

//string
namespace nihil {

	class string {

	public:
		using iterator = char*;
		using const_iterator = const char*;

		string() : _str(nullptr), _size(0), _capacity(0){}
		string(const char* cstr) : _size(strlen(cstr)), _capacity(_size) {
			_str = new char[_capacity + 1];
			std::copy(cstr, cstr + _size + 1, _str);
			cout << "深拷贝构造" << endl;
		}
		string(const string& cstr) : _str(nullptr) {
			string tmp(cstr._str);
			swap(tmp);
		}
		string(string&& cstr) : _str(nullptr) {
			swap(cstr);
			cout << "移动拷贝构造" << endl;
		}
		//传统写法
		//string(const string& cstr) : _size(cstr._size), _capacity(cstr._capacity) {
		//	_str = new char[_capacity + 1];
		//	memcpy(_str, cstr._str, cstr._size + 1);
		//	cout << "string(const string& cstr)" << endl;
		//}
		~string() {
			if (_str) {
				delete[] _str;
				_str = nullptr;
				_size = 0;
				_capacity = 0;
			}
		}

		size_t size() const {
			return _size;
		}
		const char* c_str() const {
			return _str;
		}

		void reserve(size_t n) {
			if (n > _capacity) {
				char* tmp = new char[n + 1];
				memcpy(tmp, _str, _size + 1);
				delete[] _str;
				_str = tmp;
				_capacity = n;
			}
		}
		void resize(size_t n, char c = '\0') {
			if (n < _size) {
				_size = n;
				_str[_size] = '\0';
			}
			else {
				reserve(n);
				for (size_t i = _size; i < n; ++i) {
					_str[i] = c;
				}
				_size = n;
				_str[_size] = '\0';
			}
		}
		void clear() {
			_str[0] = '\0';
			_size = 0;
		}
		void push_back(char c) {
			if (_size == _capacity) {
				reserve(_capacity == 0 ? 4 : 2 * _capacity);
			}
			_str[_size++] = c;
			_str[_size] = '\0';
		}
		void append(const char* cstr) {
			size_t len = strlen(cstr);
			if (_size + len >= _capacity) {
				reserve(_size + len);
			}
			memcpy(_str + _size, cstr, len);
			_size += len;
		}
		void insert(size_t pos, size_t n, char c) {
			assert(pos <= _size);

			if (_size + n >= _capacity) {
				reserve(_size + n);
			}

			size_t ed = _size + n;
			while (ed >= pos + n) {
				_str[ed] = _str[ed - n];
				--ed;
			}

			for (size_t i = 0; i < n; ++i) {
				_str[pos + i] = c;
			}

			_size += n;
			_str[_size] = '\0';
		}
		void insert(size_t pos, const char* cstr) {
			assert(pos <= _size);
			size_t n = strlen(cstr);
			if (_size + n >= _capacity) {
				reserve(_size + n);
			}

			size_t ed = _size + n;
			while (ed >= pos + n) {
				_str[ed] = _str[ed - n];
				--ed;
			}

			for (size_t i = 0; i < n; ++i) {
				_str[pos + i] = cstr[i];
			}
			_size += n;
			_str[_size] = '\0';
		}
		void erase(size_t pos, size_t n = npos) {
			assert(pos < _size);
			if (n == npos || pos + n >= _size) {
				_size = pos;
				_str[_size] = '\0';
			}
			else {
				size_t ed = pos + n;
				while (ed <= _size) {
					_str[pos++] = _str[ed++];
				}
				_size -= n;
			}
		}
		void swap(string& tmp) {
			if (&tmp == this) {
				return;
			}
			std::swap(_str, tmp._str);
			std::swap(_size, tmp._size);
			std::swap(_capacity,tmp._capacity);
		}
		size_t find(char c, size_t pos = 0) {
			assert(pos < _size);
			for (size_t i = pos; i < _size; ++i) {
				if (_str[i] == c) {
					return i;
				}
			}
			return npos;
		}
		size_t find(const char* cstr, size_t pos = 0) {
			assert(pos < _size);
			const char* str = strstr(_str, cstr);
			if (str) {
				return  str - _str;
			}
			else {
				return npos;
			}
		}
		string substr(size_t pos = 0, size_t n = npos) {
			assert(pos < _size);
			size_t len;
			if (n == npos || pos + n >= _size) {
				len = _size - pos;
			}
			else {
				len = n;
			}

			string tmp;
			tmp.reserve(len);
			for (size_t i = pos; i < pos + len; ++i) {
				tmp += _str[i];
			}
			return tmp;
		}

		iterator begin() {
			return _str;
		}
		iterator end() {
			return _str + _size;
		}
		const_iterator begin() const {
			return _str;
		}
		const_iterator end() const {
			return _str + _size;
		}
		const_iterator cbegin() const {
			return _str;
		}
		const_iterator cend() const {
			return _str + _size;
		}

		string& operator=(const string& cstr) {
			//传统写法
			//if (this != &cstr) {
			//	_size = cstr._size;
			//	_capacity = cstr._capacity;
			//	if (_str) {
			//		delete[] _str;
			//		_str = nullptr;
			//	}
			//	_str = new char[_capacity + 1];
			//	memcpy(_str, cstr._str, cstr._size + 1);
			//}

			string tmp(cstr);
			swap(tmp);
			cout << "深拷贝赋值" << endl;
			return *this;
		}
		string& operator=(string&& cstr) {
			swap(cstr);
			cout << "移动赋值" << endl;
			return *this;
		}
		void operator=(const char* cstr) {
			//_size = strlen(cstr);
			//_capacity = _size;
			//if (_str) {
			//	delete[] _str;
			//	_str = nullptr;
			//}
			//_str = new char[_capacity + 1];
			//memcpy(_str, cstr, _size + 1);
			string tmp(cstr);
			swap(tmp);
		}
		string& operator+=(char c) {
			push_back(c);
			return *this;
		}
		string& operator+=(const char* cstr) {
			append(cstr);
			return *this;
		}
		char& operator[](int pos) {
			assert(pos < _size);
			return _str[pos];
		}
		const char& operator[](int pos) const {
			assert(pos < _size);
			return _str[pos];
		}

		const static size_t npos;
	private:
		char* _str;
		size_t _size;
		size_t _capacity;
	};
	const size_t string::npos = -1;

	std::ostream& operator<<(std::ostream& out, const string& str) {
		for (auto val : str) {
			out << val;
		}
		return out;
	}
	std::istream& operator>>(std::istream& in, string& str) {

		str.clear();
		char c = in.get();
		char buffer[128];
		int pos = 0;

		while (c == ' ' || c == '\n') {
			c = in.get();
		}

		while (c != ' ' && c != '\n') {
			buffer[pos++] = c;

			if (pos == 127) {
				buffer[pos] = '\0';
				str += buffer;
				pos = 0;
			}
			c = in.get();
		}

		if (pos != 0) {
			buffer[pos] = '\0';
			str += buffer;
		}
		return in;
	}
}