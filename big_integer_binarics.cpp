//
// Created by oktet on 12.04.19.
//

#include "big_integer.h"
#include <algorithm>

big_integer big_integer::operator&(const big_integer &that) const {
    big_integer ret;

    size_t op_size = max(a.size(), that.a.size());

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d0 = (i < a.size()) ? a[i] : 0;
        uint64_t d1 = (i < that.a.size()) ? that.a[i] : 0;

        ret.a.push_back(d0 & d1);
    }
    ret.sign = sign & that.sign;

    return ret;
}

big_integer big_integer::operator&=(const big_integer &that) {
    *this = *this & that;
    return *this;
}

big_integer big_integer::operator|(const big_integer &that) const {
    big_integer ret;

    size_t op_size = max(a.size(), that.a.size());

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d0 = (i < a.size()) ? a[i] : 0;
        uint64_t d1 = (i < that.a.size()) ? that.a[i] : 0;

        ret.a.push_back(d0 | d1);
    }
    ret.sign = sign | that.sign;

    return ret;
}

big_integer big_integer::operator|=(const big_integer &that) {
    *this = *this | that;
    return *this;
}

big_integer big_integer::operator^(const big_integer &that) const {
    big_integer ret;

    size_t op_size = max(a.size(), that.a.size());

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d0 = (i < a.size()) ? a[i] : 0;
        uint64_t d1 = (i < that.a.size()) ? that.a[i] : 0;

        ret.a.push_back(d0 ^ d1);
    }
    ret.sign = sign ^ that.sign;

    return ret;
}

big_integer big_integer::operator^=(const big_integer &that) {
    *this = *this ^ that;
    return *this;
}

big_integer big_integer::operator~() const {
    big_integer ret;

    for (uint64_t d : a) {
        ret.a.push_back(~d);
    }
    ret.sign = ~sign;

    return ret;
}

big_integer big_integer::operator<<(uint32_t shift) const {
    big_integer ret = shl_64_bitwise(shift / 64);
    shift %= 64;
    uint64_t carry = 0;
    uint64_t next_carry = 0;

    for (unsigned long &i : ret.a) {
        next_carry = (i >> (64 - shift));
        i = ((i << shift) + carry);
        carry = next_carry;
    }

    if (carry != 0) {
        ret.a.push_back(carry);
    }

    return ret;
}

big_integer big_integer::operator<<=(const uint32_t shift) {
    *this = *this << shift;
    return *this;
}

big_integer big_integer::operator>>(uint32_t shift) const {
    big_integer ret = shr_64_bitwise(shift / 64);
    shift %= 64;
    uint64_t carry = 0;
    uint64_t next_carry = 0;

    for (size_t i = ret.a.size() - 1; i + 1 > i; i--) {
        next_carry = (ret.a[i] << (64 - shift));
        ret.a[i] = ((ret.a[i] >> shift) + carry);
        carry = next_carry;
    }

    return ret;
}

big_integer big_integer::operator>>=(const uint32_t shift) {
    *this = *this >> shift;
    return *this;
}