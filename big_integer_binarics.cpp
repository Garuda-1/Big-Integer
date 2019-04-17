//
// Created by oktet on 12.04.19.
//

#include "big_integer.h"
#include <algorithm>

big_integer operator&(const big_integer &a, const big_integer &b) {
    big_integer ret;

    size_t op_size = max(a.arr.size(), b.arr.size()) + 1;

    big_integer op0 = a.bitwise_convert(op_size);
    big_integer op1 = b.bitwise_convert(op_size);

    for (size_t i = 0; i < op_size; i++) {
        ret.arr.push_back(op0.arr[i] & op1.arr[i]);
    }
    ret = ret.bitwise_revert();
    ret.shrink();
    return ret;
}

big_integer operator|(const big_integer &a, const big_integer &b) {
    big_integer ret;

    size_t op_size = max(a.arr.size(), b.arr.size()) + 1;

    big_integer op0 = a.bitwise_convert(op_size);
    big_integer op1 = b.bitwise_convert(op_size);

    for (size_t i = 0; i < op_size; i++) {
        ret.arr.push_back(op0.arr[i] | op1.arr[i]);
    }
    ret = ret.bitwise_revert();
    ret.shrink();
    return ret;
}

big_integer operator^(const big_integer &a, const big_integer &b) {
    big_integer ret;

    size_t op_size = max(a.arr.size(), b.arr.size()) + 1;

    big_integer op0 = a.bitwise_convert(op_size);
    big_integer op1 = b.bitwise_convert(op_size);

    for (size_t i = 0; i < op_size; i++) {
        ret.arr.push_back(op0.arr[i] ^ op1.arr[i]);
    }
    ret = ret.bitwise_revert();
    ret.shrink();
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

    ret = -*this - 1;

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