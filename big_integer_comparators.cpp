//
// Created by oktet on 09.04.19.
//

#include "big_integer.h"

bool operator<(const big_integer &a, const big_integer &b) {
    if (a.sign != b.sign) {
        return a.sign > b.sign;
    }
    int32_t cmp = a.modular_compare(b);
    return cmp != 0 && a.sign ^ (cmp == -1);
}

bool operator>(const big_integer &a, const big_integer &b) {
    if (a.sign != b.sign) {
        return a.sign < b.sign;
    }
    int32_t cmp = a.modular_compare(b);
    return cmp != 0 && a.sign ^ (cmp == 1);
}

bool operator<=(const big_integer &a, const big_integer &b) {
    return !(a > b);
}

bool operator>=(const big_integer &a, const big_integer &b) {
    return !(a < b);
}

bool operator==(const big_integer &a, const big_integer &b) {
    if (a.sign != b.sign) {
        return false;
    }
    return a.modular_compare(b) == 0;
}

bool operator!=(const big_integer &a, const big_integer &b) {
    return !(a == b);
}
