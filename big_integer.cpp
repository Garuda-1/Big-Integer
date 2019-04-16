//
// Created by oktet on 09.04.19.
//

#include "big_integer.h"
#include <algorithm>

big_integer &big_integer::operator=(const big_integer &that) {
    if (this != &that) {
        arr.resize(that.arr.size());
        for (size_t i = 0; i < arr.size(); i++) {
            arr[i] = that.arr[i];
        }
        sign = that.sign;
    }
    return *this;
}

string to_string(const big_integer &a) {
    string ret;
    uint64_t radix = 10;
    big_integer tmp(a);

    if (a == 0) {
        return "0";
    }

    bool sign = a < 0;

    while (tmp != 0) {
        auto p = tmp.div_internal(radix);
        ret.append(to_string(p.second));
        tmp = p.first;
    }

    if (sign) ret.append("-");

    reverse(ret.begin(), ret.end());
    return ret;
}