//
// Created by oktet on 09.04.19.
//

#include <limits>
#include <algorithm>
#include <iostream>
#include "big_integer.h"

big_integer big_integer::operator+() const {
    return big_integer(*this);
}

big_integer big_integer::operator-() const {
    big_integer ret(*this);
    ret.sign ^= true;

    return ret;
}

big_integer operator+(const big_integer &a, const big_integer &b) {
    big_integer ret;

    if (!a.sign && !b.sign) {
        ret = a.modular_add(b);
        ret.sign = false;
    }
    if (!a.sign && b.sign) {
        if (a.modular_compare(b) < 0) {
            ret = b.modular_subtract(a);
            ret.sign = true;
        } else {
            ret = a.modular_subtract(b);
            ret.sign = false;
        }
    }
    if (a.sign && !b.sign) {
        if (a.modular_compare(b) <= 0) {
            ret = b.modular_subtract(a);
            ret.sign = false;
        } else {
            ret = a.modular_subtract(b);
            ret.sign = true;
        }
    }
    if (a.sign && b.sign) {
        ret = a.modular_add(b);
        ret.sign = true;
    }

    return ret;
}

big_integer operator-(const big_integer &a, const big_integer &b) {
    big_integer ret;

    if (!a.sign && !b.sign) {
        if (a.modular_compare(b) < 0) {
            ret = b.modular_subtract(a);
            ret.sign = true;
        } else {
            ret = a.modular_subtract(b);
            ret.sign = false;
        }
    }
    if (!a.sign && b.sign) {
        ret = a.modular_add(b);
        ret.sign = false;
    }
    if (a.sign && !b.sign) {
        ret = a.modular_add(b);
        ret.sign = true;
    }
    if (a.sign && b.sign) {
        if (a.modular_compare(b) <= 0) {
            ret = b.modular_subtract(a);
            ret.sign = false;
        } else {
            ret = a.modular_subtract(b);
            ret.sign = true;
        }
    }

    return ret;
}

big_integer operator*(const big_integer &a, const big_integer &b) {
    size_t op_size = a.arr.size();

    big_integer ret;

    for (size_t i = 0; i < op_size; i++) {
        ret += (b * a.arr[i]).shl_64_bitwise(i);
    }

    ret.shrink();
    ret.sign = (ret != 0) && (a.sign ^ b.sign);
    return ret;
}

big_integer operator/(const big_integer &a, const big_integer &b) {
    return a.divide_mod(b).first;
}

big_integer operator%(const big_integer &a, const big_integer &b) {
    return a.divide_mod(b).second;
}

big_integer& big_integer::operator+=(const big_integer &that) {
    *this = *this + that;
    return *this;
}

big_integer& big_integer::operator-=(const big_integer &that) {
    *this = *this - that;
    return *this;
}

big_integer& big_integer::operator*=(const big_integer &that) {
    *this = *this * that;
    return *this;
}

big_integer& big_integer::operator/=(const big_integer &that) {
    *this = *this / that;
    return *this;
}

big_integer& big_integer::operator%=(const big_integer &that) {
    *this = *this % that;
    return *this;
}

big_integer& big_integer::operator++() {
    *this += 1;
    return *this;
}

big_integer& big_integer::operator--() {
    *this -= 1;
    return *this;
}

const big_integer big_integer::operator++(int) {
    big_integer res(*this);
    return res += 1;
}

const big_integer big_integer::operator--(int) {
    big_integer res(*this);
    return res -= 1;
}