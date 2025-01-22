#pragma once

namespace Bitset {
    template<size_t N>
    class bitset {
    public:
        bitset() {
            _lib.resize(N / 32 + 1, 0);
        }
        void set(size_t x) {//��1
            size_t i = x / 32;
            size_t j = x % 32;

            _lib[i] |= (1 << j);// ���λ��jλ��Ȼ�����
        }

        void reset(size_t x) {///��0
            size_t i = x / 32;
            size_t j = x % 32;

            _lib[i] &= (~(1 << j));// ���λ��jλ��ȡ����Ȼ�����
        }

        bool test(size_t x) {
            size_t i = x / 32;
            size_t j = x % 32;
            return _lib[i] & (1 << j);
        }
    private:
        vector<int> _lib;
    };
}