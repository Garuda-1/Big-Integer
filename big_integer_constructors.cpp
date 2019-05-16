//
// Created by oktet on 09.04.19.
//

#include "big_integer.h"
#include <cstdlib>

big_integer::big_integer() {
    _arr.resize(0);
    sign = false;
}

big_integer::big_integer(const big_integer &that) {
    _arr = that._arr;
    sign = that.sign;
}

big_integer::big_integer(const int val) {
    if (val == 0) {
        _arr.resize(0);
        sign = false;
    } else {
        _arr.resize(1);
        _arr[0] = (uint64_t) ((val > 0) ? val : -((int64_t) val));
        sign = val < 0;
    }
}

big_integer::big_integer(string s) {
    uint64_t radix = 10;

    for (size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0; i < s.length(); i++) {
        *this *= radix;
        *this += big_integer(s[i] - '0');
    }

    sign = (!_arr.empty()) && (s[0] == '-');
}