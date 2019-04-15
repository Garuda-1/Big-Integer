//
// Created by oktet on 11.04.19.
//

#include "big_integer.h"
#include <algorithm>

//big_integer::big_integer(uint64_t val) {
//    if (val == 0) {
//        a.resize(0);
//        sign = false;
//    } else {
//        a.resize(1);
//        a[0] = val;
//        sign = false;
//    }
//}

big_integer big_integer::modular_add(const big_integer &that) const {
    big_integer ret;
    bool carry = false;
    size_t op_size = max(a.size(), that.a.size());

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = (i < a.size()) ? a[i] : 0;
        uint64_t d2 = (i < that.a.size()) ? that.a[i] : 0;
        ret.a.push_back(d1 + d2 + carry);
        carry = ((numeric_limits<uint64_t>::max() - d1 < d2) ||
                 (numeric_limits<uint64_t>::max() - d1 - d2 < (uint64_t) carry));
    }

    if (carry) {
        ret.a.push_back((uint64_t) carry);
    }

    ret.shrink();
    return ret;
}

big_integer big_integer::modular_subtract(const big_integer &that) const {
    big_integer ret;
    bool borrow = false;
    size_t op_size = a.size();

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = (i < a.size()) ? a[i] : 0;
        uint64_t d2 = (i < that.a.size()) ? that.a[i] : 0;
        ret.a.push_back(d1 - d2 - (uint64_t) borrow);
        borrow = (d1 < d2) || (d1 - d2 < (uint64_t) borrow);
    }

    ret.shrink();
    return ret;
}

int32_t big_integer::modular_compare(const big_integer &that) const {
    if (a.size() != that.a.size()) {
        return (a.size() < that.a.size()) ? -1 : 1;
    }
    for (size_t i = 0; i < a.size(); i++) {
        if (a[a.size() - i - 1] != that.a[that.a.size() - i - 1]) {
            return (a[a.size() - i - 1] < that.a[that.a.size() - i - 1]) ? -1 : 1;
        }
    }
    return 0;
}

big_integer big_integer::operator*(uint64_t val) const {
    uint64_t carry = 0;
    size_t op_size = a.size();

    big_integer res;

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = a[i];
        uint64_t d0;
        asm (
        "mul %%rbx;"
        "add %%rcx, %%rax;"
        "adc $0, %%rdx;"
        : "=a" (d0), "=d" (carry)
        : "a" (d1), "b" (val), "c" (carry));
        res.a.push_back(d0);
    }

    if (carry != 0) {
        res.a.push_back(carry);
    }

    res.shrink();
    return res;
}

pair<big_integer, uint64_t> big_integer::operator/(uint64_t val) const {
    uint64_t carry = 0;
    size_t op_size = a.size();

    big_integer res;

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = a[op_size - i - 1];
        uint64_t d0;
        __asm__ (
        "div %%rbx"
        : "=a" (d0), "=d" (carry)
        : "a" (d1), "b" (val), "d" (carry)
        );
        res.a.push_back(d0);
    }

    reverse(res.a.begin(), res.a.end());

    res.shrink();
    return pair(res, carry);
}

pair<big_integer, big_integer> big_integer::divide_mod(const big_integer &that) const {
    big_integer ret;

    if (that.a.size() == 1) {
        auto tmp = (*this / that.a[0]);
        tmp.first.sign = (tmp.first != 0) && (sign ^ that.sign);
        return pair<big_integer, big_integer>(tmp.first, tmp.second);
    }
    if (a.size() < that.a.size()) {
        return pair<big_integer, big_integer>(0, *this);
    }

    big_integer dividend(*this);
    big_integer divisor(that);
    dividend.sign = false;
    divisor.sign = false;
    normalize(dividend, divisor);

    size_t n = divisor.a.size();
    size_t m = dividend.a.size() - n;

    if (dividend >= divisor.shl_64_bitwise(m)) {
        ret.a.push_back(1);
        dividend -= divisor.shl_64_bitwise(m);
    }

    for (size_t i = m - 1; i + 1 > i; i--) {
        big_integer d;
        d.a.push_back(dividend.a[n + i - 1]);
        d.a.push_back(dividend.a[n + i]);
        d = (d / divisor.a[n - 1]).first;
        if (d.a.size() > 1) {
            d.a.resize(0);
            d.a.push_back(numeric_limits<uint64_t>::max());
        }
        dividend -= d.shl_64_bitwise(i) * divisor;

        while (dividend < 0) {
            --d;
            dividend += divisor.shl_64_bitwise(i);
        }
        ret.a.push_back(d.a[0]);
    }

    reverse(ret.a.begin(), ret.a.end());
    ret.sign = (ret != 0) && (sign ^ that.sign);
    return pair<big_integer, big_integer>(ret, dividend);
}

big_integer big_integer::shl_64_bitwise(size_t n) const {
    big_integer ret;

    for (size_t i = 0; i < n; i++) {
        ret.a.push_back(0);
    }
    for (uint64_t x : a) {
        ret.a.push_back(x);
    }

    ret.sign = sign;
    return ret;
}

big_integer big_integer::shr_64_bitwise(size_t n) const {
    big_integer ret;

    for (size_t i = n; i < a.size(); i++) {
        ret.a.push_back(a[i]);
    }

    ret.sign = sign;
    return ret;
}

void big_integer::shrink() {
    size_t i = a.size();
    while (i > 0 && a[i - 1] == 0) --i;
    a.resize(i);
};

void big_integer::normalize(big_integer &a, big_integer &b) const {
    big_integer normalizer = (big_integer(1).shl_64_bitwise(1) / (b.a[b.a.size() - 1] + 1)).first;
    a *= normalizer;
    b *= normalizer;
}