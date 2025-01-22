#pragma once
#include<functional>

namespace nihil {
	template<typename T>
	class smart_ptr {
	public:
		smart_ptr(T* ptr = nullptr) : _ptr(ptr){}
		smart_ptr(smart_ptr<T>& sp) {// 使用权转移
			_ptr = sp._ptr;
			sp._ptr = nullptr;
		}
		~smart_ptr() {
			delete  _ptr;
		}
		
		smart_ptr<T>& operator=(const T* ptr) {
			_ptr = ptr;
			return *this;
		}
		smart_ptr<T>& operator=(smart_ptr<T>& sp) {// 使用权转移
			_ptr = sp._ptr;
			sp._ptr = nullptr;
			return *this;
		}
		T& operator*() {
			return *_ptr;
		}
		T* operator->() {
			return _ptr;
		}
	private:
		T* _ptr;
	};

	template<typename T>
	class unique_ptr {
	public:
		unique_ptr(T* ptr = nullptr) : _ptr(ptr) {}
		~unique_ptr() {
			delete  _ptr;
		}
		unique_ptr<T>& operator=(const T* ptr) {
			if (_ptr != nullptr) {
				delete _ptr;
			}
			_ptr = ptr;
			return *this;
		}
		T& operator*() {
			return *_ptr;
		}
		T* operator->() {
			return _ptr;
		}

		//防拷贝
		unique_ptr(unique_ptr<T>& sp) = delete;
		unique_ptr<T>& operator=(unique_ptr<T>& sp) = delete;
	private:
		T* _ptr;
	};

	template<typename T>
	class shared_ptr {
		template<typename T>
		friend class weak_ptr;
	public:
		shared_ptr(T* ptr = nullptr) : _ptr(ptr), _pct(new int(1)), _del([](T* ptr) {delete ptr; }) {}// 默认删除器
		template<typename D>
		shared_ptr(T* ptr, D del) : _ptr(ptr), _pct(new int(1)), _del(del) {}// 用户提供删除器

		shared_ptr(shared_ptr<T>& sp) : _ptr(sp._ptr), _pct(sp._pct), _del(sp._del) {
			(*_pct)++;
		}
		~shared_ptr() {
			if (--(*_pct) == 0) {
				_del(_ptr);
				delete _pct;
			}
		}
		
		int use_count() {
			return *_pct;
		}
		shared_ptr<T>& operator=(const shared_ptr<T>& sp) {
			if (&sp != this) {// 防止自身拷贝
				if (--(*_pct) == 0) {// 检测是否为最后一个资源
					_del(_ptr);
					delete _pct;
				}
				// 更新
				_ptr = sp._ptr;
				_pct = sp._pct;
				_del = sp._del;
				(*_pct)++;
			}
			return *this;
		}
		T& operator*() {
			return *_ptr;
		}
		T* operator->() {
			return _ptr;
		}
	private:
		T* _ptr;
		int* _pct;
		std::function<void(T*)> _del;
	};

	template<typename T>
	class weak_ptr {
	public:
		weak_ptr() : _ptr(nullptr) {}
		weak_ptr(shared_ptr<T>& sp) : _ptr(sp._ptr) {}

		weak_ptr<T>& operator=(const shared_ptr<T>& sp) {// 不增加计数
			_ptr = sp._ptr;
			return *this;
		}
		T& operator*() {
			return *_ptr;
		}
		T* operator->() {
			return _ptr;
		}
	private:
		T* _ptr;
	};
}