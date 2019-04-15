//
// Created by oktet on 09.04.19.
//

#ifndef UNTITLED_BIGINTEGER_H
#define UNTITLED_BIGINTEGER_H


#include <cstdint>
#include <vector>
#include <string>

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

private:
    big_integer operator*(uint64_t val) const;
    pair<big_integer, uint64_t> operator/(uint64_t val) const;

    big_integer shl_64_bitwise(size_t n) const;
    big_integer shr_64_bitwise(size_t n) const;

    big_integer modular_add(const big_integer &that) const;
    big_integer modular_subtract(const big_integer &that) const;
    int32_t modular_compare(const big_integer &that) const;
    pair<big_integer, big_integer> divide_mod(const big_integer &that) const;

    void shrink();
    void normalize(big_integer &a, big_integer &b) const;

    vector<uint64_t> arr;
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
