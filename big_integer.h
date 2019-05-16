//
// Created by oktet on 09.04.19.
//

#ifndef UNTITLED_BIGINTEGER_H
#define UNTITLED_BIGINTEGER_H


#include <cstdint>
#include <string>
#include "optimized_vector.h"

using namespace std;

class big_integer {

public:
    big_integer();
    big_integer(const big_integer &that);
    big_integer(int val);
    explicit big_integer(string s);

    big_integer& operator=(const big_integer &that);

    friend bool operator<(const big_integer &a, const big_integer &b);
    friend bool operator>(const big_integer &a, const big_integer &b);
    friend bool operator<=(const big_integer &a, const big_integer &b);
    friend bool operator>=(const big_integer &a, const big_integer &b);
    friend bool operator==(const big_integer &a, const big_integer &b);
    friend bool operator!=(const big_integer &a, const big_integer &b);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    friend big_integer operator+(const big_integer &a, const big_integer &b);
    friend big_integer operator-(const big_integer &a, const big_integer &b);
    friend big_integer operator*(const big_integer &a, const big_integer &b);
    friend big_integer operator/(const big_integer &a, const big_integer &b);
    friend big_integer operator%(const big_integer &a, const big_integer &b);

    big_integer& operator+=(const big_integer &that);
    big_integer& operator-=(const big_integer &that);
    big_integer& operator*=(const big_integer &that);
    big_integer& operator/=(const big_integer &that);
    big_integer& operator%=(const big_integer &that);

    big_integer& operator++();
    big_integer& operator--();
    const big_integer operator++(int);
    const big_integer operator--(int);

    friend big_integer operator&(const big_integer &a, const big_integer &b);
    friend big_integer operator|(const big_integer &a, const big_integer &b);
    friend big_integer operator^(const big_integer &a, const big_integer &b);

    big_integer& operator&=(const big_integer &that);
    big_integer& operator|=(const big_integer &that);
    big_integer& operator^=(const big_integer &that);

    friend big_integer operator<<(const big_integer &a, uint32_t shift);
    friend big_integer operator>>(const big_integer &a, uint32_t shift);

    big_integer& operator<<=(uint32_t shift);
    big_integer& operator>>=(uint32_t shift);

    friend string to_string(const big_integer &a);

    friend int main();

private:
    big_integer(uint64_t val);
    uint64_t _digit(size_t i) const;
    void _shrink();

    void _add(const big_integer &that, size_t insert_pos);
    big_integer _subtract(const big_integer &that) const;
    big_integer _mul(uint64_t val) const;
    pair<big_integer, uint64_t> _div(uint64_t val) const;
    int32_t _compare(const big_integer &that) const;

    big_integer& _shl_64(size_t n);
    big_integer& _shr_64(size_t n);
    big_integer _to_two_component(size_t len) const;
    big_integer _from_two_component() const;

    pair<big_integer, big_integer> _divide_mod(const big_integer &that) const;
    uint64_t _normalize(big_integer &a, big_integer &b) const;

    optimized_vector<uint64_t> _arr;
    bool sign;
};

bool operator<(const big_integer &a, const big_integer &b);
bool operator>(const big_integer &a, const big_integer &b);
bool operator<=(const big_integer &a, const big_integer &b);
bool operator>=(const big_integer &a, const big_integer &b);
bool operator==(const big_integer &a, const big_integer &b);
bool operator!=(const big_integer &a, const big_integer &b);

big_integer operator+(const big_integer &a, const big_integer &b);
big_integer operator-(const big_integer &a, const big_integer &b);
big_integer operator*(const big_integer &a, const big_integer &b);
big_integer operator/(const big_integer &a, const big_integer &b);
big_integer operator%(const big_integer &a, const big_integer &b);

big_integer operator&(const big_integer &a, const big_integer &b);
big_integer operator|(const big_integer &a, const big_integer &b);
big_integer operator^(const big_integer &a, const big_integer &b);

big_integer operator<<(const big_integer &a, uint32_t shift);
big_integer operator>>(const big_integer &a, uint32_t shift);

string to_string(const big_integer &a);

#endif
