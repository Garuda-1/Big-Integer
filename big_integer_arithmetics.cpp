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
    ret.sign = (!arr.empty()) && (!ret.sign);
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
        this->modular_add(that);
        sign = false;
    }
    else if (!sign && that.sign) {
        if (this->modular_compare(that) < 0) {
            *this = that.modular_subtract(*this);
            sign = true;
        } else {
            *this = this->modular_subtract(that);
            sign = false;
        }
    }
    else if (sign && !that.sign) {
        if (this->modular_compare(that) <= 0) {
            *this = that.modular_subtract(*this);
            sign = false;
        } else {
            *this = this->modular_subtract(that);
            sign = true;
        }
    }
    else if (sign && that.sign) {
        this->modular_add(that);
        sign = true;
    }

    return *this;
}

big_integer& big_integer::operator-=(const big_integer &that) {
    if (!sign && !that.sign) {
        if (this->modular_compare(that) < 0) {
            *this = that.modular_subtract(*this);
            sign = true;
        } else {
            *this = this->modular_subtract(that);
            sign = false;
        }
    }
    else if (!sign && that.sign) {
        this->modular_add(that);
        sign = false;
    }
    else if (sign && !that.sign) {
        this->modular_add(that);
        sign = true;
    }
    else if (sign && that.sign) {
        if (this->modular_compare(that) <= 0) {
            *this = that.modular_subtract(*this);
            sign = false;
        } else {
            *this = this->modular_subtract(that);
            sign = true;
        }
    }

    return *this;
}

big_integer& big_integer::operator*=(const big_integer &that) {
    size_t op_size = arr.size();

    big_integer ret;

    for (size_t i = 0; i < op_size; i++) {
        ret += (that.mul_internal(arr[i])).shl_64_bitwise(i);
    }

    ret.shrink();
    ret.sign = (ret != 0) && (sign ^ that.sign);
    *this = ret;

    return *this;
}

big_integer& big_integer::operator/=(const big_integer &that) {
    *this = this->divide_mod(that).first;
    return *this;
}

big_integer& big_integer::operator%=(const big_integer &that) {
    *this = this->divide_mod(that).second;
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