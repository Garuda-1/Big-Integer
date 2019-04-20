//
// Created by oktet on 11.04.19.
//

#include "big_integer.h"
#include <algorithm>

big_integer::big_integer(uint64_t val) {
    sign = false;
    if (val == 0) {
        _arr.resize(0);
    } else {
        _arr.resize(1);
        _arr[0] = val;
    }
}

uint64_t big_integer::_digit(size_t i) const {
    if (i < _arr.size()) {
        return _arr[i];
    } else {
        return 0;
    }
}

void big_integer::_add(const big_integer &that, const size_t insert_pos) {
    bool carry = false;
    size_t op_size = max(_arr.size(), (that._arr.size() + insert_pos));
    _arr.resize(op_size, 0);

    for (size_t i = 0; i < op_size - insert_pos; i++) {
        uint64_t d1 = this->_digit(i + insert_pos);
        uint64_t d2 = that._digit(i);
        __asm__ (
        "xor %%rdx, %%rdx;"
        "add %%rbx, %%rax;"
        "adc $0, %%rdx;"
        "add %%rcx, %%rax;"
        "adc $0, %%rdx;"
        : "=a" (_arr[i + insert_pos]), "=d" (carry)
        : "a" (d1), "b" (d2), "c" (carry));
    }

    if (carry) {
        _arr.push_back((uint64_t) carry);
    }

    _shrink();
}

big_integer big_integer::_subtract(const big_integer &that) const {
    big_integer ret;
    bool borrow = false;
    size_t op_size = _arr.size();
    ret._arr.reserve(op_size);

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = this->_digit(i);
        uint64_t d2 = that._digit(i);
        uint64_t d;
        __asm__ (
        "xor %%rdx, %%rdx;"
        "sbb %%rbx, %%rax;"
        "adc $0, %%rdx;"
        "sbb %%rcx, %%rax;"
        "adc $0, %%rdx;"
        : "=a" (d), "=d" (borrow)
        : "a" (d1), "b" (d2), "c" (borrow));
        ret._arr.push_back(d);
    }

    ret._shrink();
    return ret;
}

int32_t big_integer::_compare(const big_integer &that) const {
    if (_arr.size() != that._arr.size()) {
        return (_arr.size() < that._arr.size()) ? -1 : 1;
    }
    for (size_t i = 0; i < _arr.size(); i++) {
        if (_arr[_arr.size() - i - 1] != that._arr[that._arr.size() - i - 1]) {
            return (_arr[_arr.size() - i - 1] < that._arr[that._arr.size() - i - 1]) ? -1 : 1;
        }
    }

    return 0;
}

big_integer big_integer::_mul(uint64_t val) const {
    uint64_t carry = 0;
    size_t op_size = _arr.size();

    big_integer res;
    res._arr.reserve(op_size + 1);

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = _digit(i);
        uint64_t d0;
        __asm__ (
        "mul %%rbx;"
        "add %%rcx, %%rax;"
        "adc $0, %%rdx;"
        : "=a" (d0), "=d" (carry)
        : "a" (d1), "b" (val), "c" (carry));
        res._arr.push_back(d0);
    }

    if (carry != 0) {
        res._arr.push_back(carry);
    }

    res._shrink();
    return res;
}

pair<big_integer, uint64_t> big_integer::_div(uint64_t val) const {
    uint64_t carry = 0;
    size_t op_size = _arr.size();

    big_integer res;

    for (size_t i = 0; i < op_size; i++) {
        uint64_t d1 = _digit(op_size - i - 1);
        uint64_t d0;
        __asm__ (
        "div %%rbx;"
        : "=a" (d0), "=d" (carry)
        : "a" (d1), "b" (val), "d" (carry)
        );
        res._arr.push_back(d0);
    }

    reverse(res._arr.begin(), res._arr.end());

    res._shrink();
    return pair<big_integer, uint64_t>(res, carry);
}

pair<big_integer, big_integer> big_integer::_divide_mod(const big_integer &that) const {
    big_integer ret;

    if (that._arr.size() == 1) {
        auto tmp = (this->_div(that._digit(0)));
        tmp.first.sign = (tmp.first != 0) && (sign ^ that.sign);
        big_integer remainder = big_integer(to_string(tmp.second));
        remainder.sign = (remainder != 0) && sign;
        return pair<big_integer, big_integer>(tmp.first, remainder);
    }

    if (_arr.size() < that._arr.size()) {
        return pair<big_integer, big_integer>(0, *this);
    }

    big_integer dividend(*this);
    big_integer divisor(that);
    dividend.sign = false;
    divisor.sign = false;
    uint64_t normalizer = _normalize(dividend, divisor);

    size_t n = divisor._arr.size();
    size_t m = dividend._arr.size() - n;

    ret._arr.reserve(m);

    big_integer aux(divisor);
    aux._shl_64(m);

    if (dividend >= aux) {
        ret._arr.push_back(1);
        dividend -= aux;
    }

    aux._shr_64(1);

    for (size_t i = m - 1; i + 1 > i; i--, aux._shr_64(1)) {
        big_integer d;
        d._arr.push_back(dividend._digit(n + i - 1));
        d._arr.push_back(dividend._digit(n + i));
        d = (d._div(divisor._digit(n - 1))).first;
        if (d._arr.size() > 1) {
            d._arr.resize(0);
            d._arr.push_back(numeric_limits<uint64_t>::max());
        }

        dividend -= d * aux;
        while (dividend < 0) {
            --d;
            dividend += aux;
        }
        ret._arr.push_back(d._digit(0));
    }

    reverse(ret._arr.begin(), ret._arr.end());
    ret._shrink();
    ret.sign = (ret != 0) && (sign ^ that.sign);
    dividend /= normalizer;
    dividend.sign = (dividend != 0) && sign;
    return pair<big_integer, big_integer>(ret, dividend);
}

big_integer &big_integer::_shl_64(size_t n) {
    size_t old_size = _arr.size();
    _arr.resize(old_size + n, 0);

    for (size_t i = old_size - 1; old_size != 0 && i + 1 > i; i--) {
        _arr[i + n] = _arr[i];
    }
    for (size_t i = n - 1; n != 0 && i + 1 > i; i--) {
        _arr[i] = 0;
    }

    return *this;
}

big_integer &big_integer::_shr_64(size_t n) {
    size_t new_size = _arr.size() - n;
    for (size_t i = 0; i < new_size; i++) {
        _arr[i] = _arr[i + n];
    }
    _arr.resize(new_size);

    return *this;
}

big_integer big_integer::_to_two_component(size_t len) const {
    big_integer ret(*this);
    ret.sign = false;

    if (!sign) {
        ret._arr.resize(len, 0);
    } else {
        for (unsigned long &i : ret._arr) {
            i = ~i;
        }
        ret._arr.resize(len, ~((uint64_t) 0));
        ++ret;
    }

    return ret;
}

big_integer big_integer::_from_two_component() const {
    big_integer ret(*this);

    bool tmp_sign = (!ret._arr.empty()) && (_arr[_arr.size() - 1] >> 63U);

    if (tmp_sign) {
        --ret;
        for (unsigned long &i : ret._arr) {
            i = ~i;
        }
        ret._shrink();
        ret.sign = tmp_sign;
    }

    return ret;
}

void big_integer::_shrink() {
    size_t i = _arr.size();
    while (i > 0 && _arr[i - 1] == 0) --i;
    _arr.resize(i);
    if (_arr.empty()) {
        sign = false;
    }
}

uint64_t big_integer::_normalize(big_integer &a, big_integer &b) const {
    uint64_t normalizer = (big_integer(1)._shl_64(1)._div(b._arr[b._arr.size() - 1] + 1)).first._arr[0];
    a *= normalizer;
    b *= normalizer;
    return normalizer;
}