//
// Created by oktet on 11.04.19.
//

#include "big_integer.h"
#include <algorithm>

big_integer big_integer::modular_add(const big_integer &that) const {
    big_integer ret;
    bool carry = false;
    size_t op_size = max(arr.size(), that.arr.size());

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = (i < arr.size()) ? arr[i] : 0;
        uint64_t d2 = (i < that.arr.size()) ? that.arr[i] : 0;
        ret.arr.push_back(d1 + d2 + carry);
        carry = ((numeric_limits<uint64_t>::max() - d1 < d2) ||
                 (numeric_limits<uint64_t>::max() - d1 - d2 < (uint64_t) carry));
    }

    if (carry) {
        ret.arr.push_back((uint64_t) carry);
    }

    ret.shrink();
    return ret;
}

big_integer big_integer::modular_subtract(const big_integer &that) const {
    big_integer ret;
    bool borrow = false;
    size_t op_size = arr.size();

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = (i < arr.size()) ? arr[i] : 0;
        uint64_t d2 = (i < that.arr.size()) ? that.arr[i] : 0;
        ret.arr.push_back(d1 - d2 - (uint64_t) borrow);
        borrow = (d1 < d2) || (d1 - d2 < (uint64_t) borrow);
    }

    ret.shrink();
    return ret;
}

int32_t big_integer::modular_compare(const big_integer &that) const {
    if (arr.size() != that.arr.size()) {
        return (arr.size() < that.arr.size()) ? -1 : 1;
    }
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[arr.size() - i - 1] != that.arr[that.arr.size() - i - 1]) {
            return (arr[arr.size() - i - 1] < that.arr[that.arr.size() - i - 1]) ? -1 : 1;
        }
    }
    return 0;
}

big_integer big_integer::mul_internal(uint64_t val) const {
    uint64_t carry = 0;
    size_t op_size = arr.size();

    big_integer res;

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = arr[i];
        uint64_t d0;
        __asm__ (
        "mul %%rbx;"
        "add %%rcx, %%rax;"
        "adc $0, %%rdx;"
        : "=a" (d0), "=d" (carry)
        : "a" (d1), "b" (val), "c" (carry));
        res.arr.push_back(d0);
    }

    if (carry != 0) {
        res.arr.push_back(carry);
    }

    res.shrink();
    return res;
}

pair<big_integer, uint64_t> big_integer::div_internal(uint64_t val) const {
    uint64_t carry = 0;
    size_t op_size = arr.size();

    big_integer res;

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = arr[op_size - i - 1];
        uint64_t d0;
        __asm__ (
        "div %%rbx"
        : "=a" (d0), "=d" (carry)
        : "a" (d1), "b" (val), "d" (carry)
        );
        res.arr.push_back(d0);
    }

    reverse(res.arr.begin(), res.arr.end());

    res.shrink();
    return pair<big_integer, uint64_t>(res, carry);
}

pair<big_integer, big_integer> big_integer::divide_mod(const big_integer &that) const {
    big_integer ret;

    if (that.arr.size() == 1) {
        auto tmp = (this->div_internal(that.arr[0]));
        tmp.first.sign = (tmp.first != 0) && (sign ^ that.sign);

        big_integer remainder = big_integer(to_string(tmp.second));
        remainder.sign = (remainder != 0) && sign;

        return pair<big_integer, big_integer>(tmp.first, remainder);
    }
    if (arr.size() < that.arr.size()) {
        return pair<big_integer, big_integer>(0, *this);
    }

    big_integer dividend(*this);
    big_integer divisor(that);
    dividend.sign = false;
    divisor.sign = false;
    big_integer normalizer = normalize(dividend, divisor);

    size_t n = divisor.arr.size();
    size_t m = dividend.arr.size() - n;

    if (dividend >= divisor.shl_64_bitwise(m)) {
        ret.arr.push_back(1);
        dividend -= divisor.shl_64_bitwise(m);
    }

    for (size_t i = m - 1; i + 1 > i; i--) {
        big_integer d;
        d.arr.push_back(dividend.arr[n + i - 1]);
        d.arr.push_back(dividend.arr[n + i]);
        d = (d.div_internal(divisor.arr[n - 1])).first;
        if (d.arr.size() > 1) {
            d.arr.resize(0);
            d.arr.push_back(numeric_limits<uint64_t>::max());
        }
        dividend -= d.shl_64_bitwise(i) * divisor;

        while (dividend < 0) {
            --d;
            dividend += divisor.shl_64_bitwise(i);
        }
        ret.arr.push_back(d.arr[0]);
    }

    reverse(ret.arr.begin(), ret.arr.end());
    ret.sign = (ret != 0) && (sign ^ that.sign);
    dividend /= normalizer;
    dividend.sign = (dividend != 0) && sign;
    return pair<big_integer, big_integer>(ret, dividend);
}

big_integer big_integer::shl_64_bitwise(size_t n) const {
    big_integer ret;

    for (size_t i = 0; i < n; i++) {
        ret.arr.push_back(0);
    }
    for (uint64_t x : arr) {
        ret.arr.push_back(x);
    }

    ret.sign = sign;
    return ret;
}

big_integer big_integer::shr_64_bitwise(size_t n) const {
    big_integer ret;

    for (size_t i = n; i < arr.size(); i++) {
        ret.arr.push_back(arr[i]);
    }

    ret.sign = sign;
    return ret;
}

void big_integer::shrink() {
    size_t i = arr.size();
    while (i > 0 && arr[i - 1] == 0) --i;
    arr.resize(i);
    if (arr.empty()) {
        sign = false;
    }
}

big_integer big_integer::normalize(big_integer &a, big_integer &b) const {
    big_integer normalizer = (big_integer(1).shl_64_bitwise(1).div_internal(b.arr[b.arr.size() - 1] + 1)).first;
    a *= normalizer;
    b *= normalizer;
    return normalizer;
}