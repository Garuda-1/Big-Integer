//
// Created by oktet on 09.04.19.
//

#include "big_integer.h"

bool big_integer::operator<(const big_integer &that) const {
    if (sign != that.sign) {
        return sign > that.sign;
    }
    int32_t cmp = this->modular_compare(that);
    return cmp != 0 && sign ^ (cmp == -1);
}

bool big_integer::operator>(const big_integer &that) const {
    if (sign != that.sign) {
        return sign < that.sign;
    }
    int32_t cmp = this->modular_compare(that);
    return cmp != 0 && sign ^ (cmp == 1);
}

bool big_integer::operator<=(const big_integer &that) const {
    return !(*this > that);
}

bool big_integer::operator>=(const big_integer &that) const {
    return !(*this < that);
}

bool big_integer::operator==(const big_integer &that) const {
    if (sign != that.sign) {
        return false;
    }
    return this->modular_compare(that) == 0;
}

bool big_integer::operator!=(const big_integer &that) const {
    return !(*this == that);
}
