//
// Created by oktet on 12.04.19.
//

#include "big_integer.h"
#include <algorithm>

big_integer operator&(const big_integer &a, const big_integer &b) {
    big_integer ret;

    size_t op_size = max(a.arr.size(), b.arr.size());

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d0 = (i < a.arr.size()) ? a.arr[i] : 0;
        uint64_t d1 = (i < b.arr.size()) ? b.arr[i] : 0;

        ret.arr.push_back(d0 & d1);
    }
    ret.sign = a.sign & b.sign;

    return ret;
}

big_integer operator|(const big_integer &a, const big_integer &b) {
    big_integer ret;

    size_t op_size = max(a.arr.size(), b.arr.size());

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d0 = (i < a.arr.size()) ? a.arr[i] : 0;
        uint64_t d1 = (i < b.arr.size()) ? b.arr[i] : 0;
        ret.arr.push_back(d0 | d1);
    }
    ret.sign = a.sign | b.sign;

    return ret;
}

big_integer operator^(const big_integer &a, const big_integer &b) {
    big_integer ret;

    size_t op_size = max(a.arr.size(), b.arr.size());

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d0 = (i < a.arr.size()) ? a.arr[i] : 0;
        uint64_t d1 = (i < b.arr.size()) ? b.arr[i] : 0;

        ret.arr.push_back(d0 ^ d1);
    }
    ret.sign = a.sign ^ b.sign;

    return ret;
}

big_integer& big_integer::operator&=(const big_integer &that) {
    *this = *this & that;
    return *this;
}

big_integer& big_integer::operator|=(const big_integer &that) {
    *this = *this | that;
    return *this;
}

big_integer& big_integer::operator^=(const big_integer &that) {
    *this = *this ^ that;
    return *this;
}

big_integer big_integer::operator~() const {
    big_integer ret;

    for (uint64_t d : arr) {
        ret.arr.push_back(~d);
    }
    ret.sign = ~sign;

    return ret;
}

big_integer operator<<(const big_integer &a, uint32_t shift) {
    big_integer ret = a.shl_64_bitwise(shift / 64);
    shift %= 64;
    uint64_t carry = 0;
    uint64_t next_carry = 0;

    for (unsigned long &i : ret.arr) {
        next_carry = (i >> (64 - shift));
        i = ((i << shift) + carry);
        carry = next_carry;
    }

    if (carry != 0) {
        ret.arr.push_back(carry);
    }

    return ret;
}

big_integer operator>>(const big_integer &a, uint32_t shift) {
    big_integer ret = a.shr_64_bitwise(shift / 64);
    shift %= 64;
    uint64_t carry = 0;
    uint64_t next_carry = 0;

    for (size_t i = ret.arr.size() - 1; i + 1 > i; i--) {
        next_carry = (ret.arr[i] << (64 - shift));
        ret.arr[i] = ((ret.arr[i] >> shift) + carry);
        carry = next_carry;
    }

    ret.shrink();

    return ret;
}

big_integer& big_integer::operator<<=(const uint32_t shift) {
    *this = *this << shift;
    return *this;
}

big_integer& big_integer::operator>>=(const uint32_t shift) {
    *this = *this >> shift;
    return *this;
}