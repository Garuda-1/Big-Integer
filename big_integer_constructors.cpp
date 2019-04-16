//
// Created by oktet on 09.04.19.
//

#include "big_integer.h"
#include <cstdlib>

big_integer::big_integer() {
    arr.resize(0);
    sign = false;
}

big_integer::big_integer(const big_integer &that) {
    arr.resize(that.arr.size());
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = that.arr[i];
    }
    sign = that.sign;
}

big_integer::big_integer(const int val) {
    if (val == 0) {
        arr.resize(0);
        sign = false;
    } else {
        arr.resize(1);
        arr[0] = (uint64_t) ((val > 0) ? val : -((int64_t) val));
        sign = val < 0;
    }
}

big_integer::big_integer(string s) {
    uint64_t radix = 10;

    for (size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0; i < s.length(); i++) {
        *this *= radix;
        *this += big_integer(s[i] - '0');
    }

    sign = (!arr.empty()) && (s[0] == '-');
}