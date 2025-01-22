#pragma once

namespace Bitset {
    template<size_t N>
    class bitset {
    public:
        bitset() {
            _lib.resize(N / 32 + 1, 0);
        }
        void set(size_t x) {//置1
            size_t i = x / 32;
            size_t j = x % 32;

            _lib[i] |= (1 << j);// 向高位移j位，然后求或
        }

        void reset(size_t x) {///置0
            size_t i = x / 32;
            size_t j = x % 32;

            _lib[i] &= (~(1 << j));// 向高位移j位，取反，然后求或
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