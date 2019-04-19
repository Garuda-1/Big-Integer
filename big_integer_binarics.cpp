//
// Created by oktet on 12.04.19.
//

#include "big_integer.h"
#include <algorithm>

big_integer operator&(const big_integer &a, const big_integer &b) {
    big_integer ret;

    size_t op_size = max(a._arr.size(), b._arr.size()) + 1;

    big_integer op0 = a._to_two_component(op_size);
    big_integer op1 = b._to_two_component(op_size);

    for (size_t i = 0; i < op_size; i++) {
        ret._arr.push_back(op0._arr[i] & op1._arr[i]);
    }
    ret = ret._from_two_component();
    ret._shrink();
    return ret;
}

big_integer operator|(const big_integer &a, const big_integer &b) {
    big_integer ret;

    size_t op_size = max(a._arr.size(), b._arr.size()) + 1;

    big_integer op0 = a._to_two_component(op_size);
    big_integer op1 = b._to_two_component(op_size);

    for (size_t i = 0; i < op_size; i++) {
        ret._arr.push_back(op0._arr[i] | op1._arr[i]);
    }
    ret = ret._from_two_component();
    ret._shrink();
    return ret;
}

big_integer operator^(const big_integer &a, const big_integer &b) {
    big_integer ret;

    size_t op_size = max(a._arr.size(), b._arr.size()) + 1;

    big_integer op0 = a._to_two_component(op_size);
    big_integer op1 = b._to_two_component(op_size);

    for (size_t i = 0; i < op_size; i++) {
        ret._arr.push_back(op0._arr[i] ^ op1._arr[i]);
    }
    ret = ret._from_two_component();
    ret._shrink();
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
    big_integer ret(a);
    ret._shl_64(shift / 64);
    shift %= 64;
    uint64_t carry = 0;
    uint64_t next_carry = 0;

    for (unsigned long &i : ret._arr) {
        next_carry = (i >> (64 - shift));
        i = ((i << shift) + carry);
        carry = next_carry;
    }

    if (carry != 0) {
        ret._arr.push_back(carry);
    }

    return ret;
}

big_integer operator>>(const big_integer &a, uint32_t shift) {
    big_integer ret(a);
    ret._shr_64(shift / 64);
    bool tmp_sign = a.sign;
    ret.sign = false;

    shift %= 64;
    uint64_t carry = 0;
    uint64_t next_carry = 0;

    for (size_t i = ret._arr.size() - 1; i + 1 > i; i--) {
        next_carry = (ret._arr[i] << (64 - shift));
        ret._arr[i] = ((ret._arr[i] >> shift) + carry);
        carry = next_carry;
    }

    ret._shrink();
    if (tmp_sign) {
        ret = -ret - 1;
    }

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