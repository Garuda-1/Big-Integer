//
// Created by oktet on 09.04.19.
//

#include <limits>
#include <algorithm>
#include "big_integer.h"

big_integer big_integer::operator+() const {
    return big_integer(*this);
}

big_integer big_integer::operator-() const {
    big_integer ret(*this);
    ret.sign = (!_arr.empty()) && (!ret.sign);
    return ret;
}

big_integer operator+(const big_integer &a, const big_integer &b) {
    big_integer ret(a);
    ret += b;
    return ret;
}

big_integer operator-(const big_integer &a, const big_integer &b) {
    big_integer ret(a);
    ret -= b;
    return ret;
}

big_integer operator*(const big_integer &a, const big_integer &b) {
    big_integer ret(a);
    ret *= b;
    return ret;
}

big_integer operator/(const big_integer &a, const big_integer &b) {
    big_integer ret(a);
    ret /= b;
    return ret;
}

big_integer operator%(const big_integer &a, const big_integer &b) {
    big_integer ret(a);
    ret %= b;
    return ret;
}

big_integer& big_integer::operator+=(const big_integer &that) {
    if (!sign && !that.sign) {
        this->_add(that, 0);
        sign = false;
    }
    else if (!sign && that.sign) {
        if (this->_compare(that) < 0) {
            *this = that._subtract(*this);
            sign = true;
        } else {
            *this = this->_subtract(that);
            sign = false;
        }
    }
    else if (sign && !that.sign) {
        if (this->_compare(that) <= 0) {
            *this = that._subtract(*this);
            sign = false;
        } else {
            *this = this->_subtract(that);
            sign = true;
        }
    }
    else if (sign && that.sign) {
        this->_add(that, 0);
        sign = true;
    }

    return *this;
}

big_integer& big_integer::operator-=(const big_integer &that) {
    if (!sign && !that.sign) {
        if (this->_compare(that) < 0) {
            *this = that._subtract(*this);
            sign = true;
        } else {
            *this = this->_subtract(that);
            sign = false;
        }
    }
    else if (!sign && that.sign) {
        this->_add(that, 0);
        sign = false;
    }
    else if (sign && !that.sign) {
        this->_add(that, 0);
        sign = true;
    }
    else if (sign && that.sign) {
        if (this->_compare(that) <= 0) {
            *this = that._subtract(*this);
            sign = false;
        } else {
            *this = this->_subtract(that);
            sign = true;
        }
    }

    return *this;
}

big_integer& big_integer::operator*=(const big_integer &that) {
    size_t op_size = _arr.size();

    big_integer ret;

    for (size_t i = 0; i < op_size; i++) {
        ret._add(that._mul(_arr[i]), i);
    }

    ret._shrink();
    ret.sign = (ret != 0) && (sign ^ that.sign);
    *this = ret;

    return *this;
}

big_integer& big_integer::operator/=(const big_integer &that) {
    *this = this->_divide_mod(that).first;
    return *this;
}

big_integer& big_integer::operator%=(const big_integer &that) {
    *this = this->_divide_mod(that).second;
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