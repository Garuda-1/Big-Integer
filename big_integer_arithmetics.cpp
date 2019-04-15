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

big_integer big_integer::operator+(const big_integer &that) const {
    big_integer ret;

    if (!sign && !that.sign) {
        ret = this->modular_add(that);
        ret.sign = false;
    }
    if (!sign && that.sign) {
        if (this->modular_compare(that) < 0) {
            ret = that.modular_subtract(*this);
            ret.sign = true;
        } else {
            ret = this->modular_subtract(that);
            ret.sign = false;
        }
    }
    if (sign && !that.sign) {
        if (this->modular_compare(that) <= 0) {
            ret = that.modular_subtract(*this);
            ret.sign = false;
        } else {
            ret = this->modular_subtract(that);
            ret.sign = true;
        }
    }
    if (sign && that.sign) {
        ret = this->modular_add(that);
        ret.sign = true;
    }

    return ret;
}

big_integer big_integer::operator+=(const big_integer &that) {
    *this = *this + that;
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer ret(*this);
    ret.sign ^= 1;

    return ret;
}

big_integer big_integer::operator-(const big_integer &that) const {
    big_integer ret;

    if (!sign && !that.sign) {
        if (this->modular_compare(that) < 0) {
            ret = that.modular_subtract(*this);
            ret.sign = true;
        } else {
            ret = this->modular_subtract(that);
            ret.sign = false;
        }
    }
    if (!sign && that.sign) {
        ret = this->modular_add(that);
        ret.sign = false;
    }
    if (sign && !that.sign) {
        ret = this->modular_add(that);
        ret.sign = true;
    }
    if (sign && that.sign) {
        if (this->modular_compare(that) <= 0) {
            ret = that.modular_subtract(*this);
            ret.sign = false;
        } else {
            ret = this->modular_subtract(that);
            ret.sign = true;
        }
    }

    return ret;
}

big_integer big_integer::operator-=(const big_integer &that) {
    *this = *this - that;
    return *this;
}

big_integer big_integer::operator*(const big_integer &that) const {
    size_t op_size = a.size();

    big_integer ret;

    for (size_t i = 0; i < op_size; i++) {
        ret += (that * a[i]).shl_64_bitwise(i);
    }

    ret.shrink();
    ret.sign = (ret != 0) && (sign ^ that.sign);
    return ret;
}

big_integer big_integer::operator*=(const big_integer &that) {
    *this = *this * that;
    return *this;
}

big_integer big_integer::operator/(const big_integer &that) const {
    return divide_mod(that).first;
}

big_integer big_integer::operator/=(const big_integer &that) {
    *this = *this / that;
    return *this;
}

big_integer big_integer::operator%(const big_integer &that) const {
    return divide_mod(that).second;
}

big_integer big_integer::operator%=(const big_integer &that) {
    *this = *this % that;
    return *this;
}

//big_integer big_integer::operator/(const big_integer &that) const {
//    big_integer ret;
//
//    if (that.a.size() == 1) {
//        ret = (*this / that.a[0]).first;
//        ret.sign = (ret != 0) && (sign ^ that.sign);
//        return ret;
//    }
//    if (a.size() < that.a.size()) {
//        return big_integer();
//    }
//
//    big_integer dividend(*this);
//    big_integer divisor(that);
//    dividend.sign = false;
//    divisor.sign = false;
//    normalize(dividend, divisor);
//
//    size_t n = divisor.a.size();
//    size_t m = dividend.a.size() - n;
//
//    if (dividend >= divisor.shl_64_bitwise(m)) {
//        ret.a.push_back(1);
//        dividend -= divisor.shl_64_bitwise(m);
//    }
//
//    for (size_t i = m - 1; i + 1 > i; i--) {
//        big_integer d;
//        d.a.push_back(dividend.a[n + i - 1]);
//        d.a.push_back(dividend.a[n + i]);
//        d = (d / divisor.a[n - 1]).first;
//        if (d.a.size() > 1) {
//            d.a.resize(0);
//            d.a.push_back(numeric_limits<uint64_t>::max());
//        }
//        dividend -= d.shl_64_bitwise(i) * divisor;
//
//        while (dividend < 0) {
//            --d;
//            dividend += divisor.shl_64_bitwise(i);
//        }
//        ret.a.push_back(d.a[0]);
//    }
//
//    reverse(ret.a.begin(), ret.a.end());
//    ret.sign = (ret != 0) && (sign ^ that.sign);
//    return ret;
//}

big_integer big_integer::operator++() {
    *this += 1;
    return *this;
}

const big_integer big_integer::operator++(int) {
    big_integer res(*this);
    return res += 1;
}

big_integer big_integer::operator--() {
    *this -= 1;
    return *this;
}

const big_integer big_integer::operator--(int) {
    big_integer res(*this);
    return res -= 1;
}