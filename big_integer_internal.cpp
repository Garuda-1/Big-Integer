//
// Created by oktet on 11.04.19.
//

#include "big_integer.h"
#include <algorithm>

big_integer::big_integer(uint64_t val) {
    sign = false;
    if (val == 0) {
        arr.resize(0);
    } else {
        arr.resize(1);
        arr[0] = val;
    }
}

uint64_t big_integer::digit_internal(size_t i) const {
    if (i < arr.size()) {
        return arr[i];
    } else {
        return 0;
    }
}

void big_integer::modular_add(const big_integer &that) {
    bool carry = false;
    size_t op_size = max(arr.size(), that.arr.size());
    arr.resize(op_size, 0);

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = this->digit_internal(i);
        uint64_t d2 = that.digit_internal(i);
        arr[i] = (d1 + d2 + carry);
        carry = ((numeric_limits<uint64_t>::max() - d1 < d2) ||
                 (numeric_limits<uint64_t>::max() - d1 - d2 < (uint64_t) carry));
    }

    if (carry) {
        arr.push_back((uint64_t) carry);
    }

    shrink();
}

big_integer big_integer::modular_subtract(const big_integer &that) const {
    big_integer ret;
    bool borrow = false;
    size_t op_size = arr.size();

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = this->digit_internal(i);
        uint64_t d2 = that.digit_internal(i);
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
        uint64_t d1 = digit_internal(i);
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
        uint64_t d1 = digit_internal(op_size - i - 1);
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
        auto tmp = (this->div_internal(that.digit_internal(0)));
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
    uint64_t normalizer = normalize(dividend, divisor);

    size_t n = divisor.arr.size();
    size_t m = dividend.arr.size() - n;

    big_integer aux(divisor);
    aux.shl_64_bitwise(m);

    if (dividend >= aux) {
        ret.arr.push_back(1);
        dividend -= aux;
    }

    aux.shr_64_bitwise(1);

    for (size_t i = m - 1; i + 1 > i; i--, aux.shr_64_bitwise(1)) {
        big_integer d;
        d.arr.push_back(dividend.digit_internal(n + i - 1));
        d.arr.push_back(dividend.digit_internal(n + i));
        d = (d.div_internal(divisor.digit_internal(n - 1))).first;
        if (d.arr.size() > 1) {
            d.arr.resize(0);
            d.arr.push_back(numeric_limits<uint64_t>::max());
        }
        dividend -= d * aux;
        while (dividend < 0) {
            --d;
            dividend += aux;
        }
        ret.arr.push_back(d.digit_internal(0));
    }

    reverse(ret.arr.begin(), ret.arr.end());
    ret.shrink();
    ret.sign = (ret != 0) && (sign ^ that.sign);
    dividend /= normalizer;
    dividend.sign = (dividend != 0) && sign;
    return pair<big_integer, big_integer>(ret, dividend);
}

big_integer& big_integer::shl_64_bitwise(size_t n) {
    size_t old_size = arr.size();
    arr.resize(old_size + n, 0);

    for (size_t i = old_size - 1; old_size != 0 && i + 1 > i; i--) {
        arr[i + n] = arr[i];
    }
    for (size_t i = n - 1; n != 0 && i + 1 > i; i--) {
        arr[i] = 0;
    }

    return *this;
}

big_integer& big_integer::shr_64_bitwise(size_t n) {
    size_t new_size = arr.size() - n;
    for (size_t i = 0; i < new_size; i++) {
        arr[i] = arr[i + n];
    }
    arr.resize(new_size);

    return *this;
}

big_integer big_integer::bitwise_convert(size_t len) const {
    big_integer ret(*this);
    ret.sign = false;

    if (!sign) {
        ret.arr.resize(len, 0);
    } else {
        for (unsigned long & i : ret.arr) {
            i = ~i;
        }
        ret.arr.resize(len, ~((uint64_t) 0));
        ++ret;
    }

    return ret;
}

big_integer big_integer::bitwise_revert() const {
    big_integer ret(*this);

    bool tmp_sign = (!ret.arr.empty()) && (arr[arr.size() - 1] >> 63U);

    if (tmp_sign) {
        --ret;
        for (unsigned long &i : ret.arr) {
            i = ~i;
        }
        ret.shrink();
        ret.sign = tmp_sign;
    }

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

uint64_t big_integer::normalize(big_integer &a, big_integer &b) const {
    uint64_t normalizer = (big_integer(1).shl_64_bitwise(1).div_internal(b.arr[b.arr.size() - 1] + 1)).first.arr[0];
    a *= normalizer;
    b *= normalizer;
    return normalizer;
}