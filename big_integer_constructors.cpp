//
// Created by oktet on 09.04.19.
//

#include "big_integer.h"
#include <cstdlib>

big_integer::big_integer() {
    a.resize(0);
    sign = false;
}

big_integer::big_integer(const big_integer &that) {
    a.resize(that.a.size());
    for (size_t i = 0; i < a.size(); i++) {
        a[i] = that.a[i];
    }
    sign = that.sign;
}

big_integer::big_integer(const int val) {
    if (val == 0) {
        a.resize(0);
        sign = false;
    } else {
        a.resize(1);
        a[0] = (uint64_t) abs(val);
        sign = val < 0;
    }
}

big_integer::big_integer(string s) {
    uint64_t radix = 10;

    for (size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0; i < s.length(); i++) {
        *this *= radix;
        *this += big_integer(s[i] - '0');
    }

    //1844674407370955161618446744073709551616

    sign = s[0] == '-';
}