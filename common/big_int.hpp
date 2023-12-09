#ifndef BIG_INT_H_
#define BIG_INT_H_

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "data.hpp"

class BigInt {
   private:
    std::vector<std::uint8_t> data;
    bool neg = false;

   public:
    BigInt() {}
    BigInt(std::string str_num);
    BigInt(const char* num_chars);
    BigInt(int num);
    BigInt(const std::vector<std::uint8_t>& data);

    std::vector<std::uint8_t> vector() { return data; }

    explicit operator bool() const { return data.size() != 0; }

    std::string to_hex_string();

    friend std::ostream& operator<<(std::ostream& out, const BigInt& num);

    friend int cmp(const BigInt& lhs, const BigInt& rhs);

    friend std::pair<BigInt, BigInt> divide(const BigInt& lhs,
                                            const BigInt& rhs);
    friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator*(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator/(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator%(const BigInt& lhs, const BigInt& rhs);
    BigInt& operator-=(const BigInt& other);
    BigInt& operator+=(const BigInt& other);
    // BigInt operator*=(const BigInt& other);
    // BigInt operator/=(const BigInt& other);
    // BigInt operator%=(const BigInt& other);

    // logical operators behaviour are undefined if values are signed
    friend BigInt operator<<(const BigInt& num, unsigned int shift);
    friend BigInt operator>>(const BigInt& num, unsigned int shift);
    friend BigInt operator&(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator|(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator^(const BigInt& lhs, const BigInt& rhs);
    // BigInt& operator<<=(unsigned int shift);
    // BigInt& operator>>=(unsigned int shift);
    BigInt& operator&=(const BigInt& other);
    BigInt& operator|=(const BigInt& other);
    BigInt& operator^=(const BigInt& other);
};

bool operator==(const BigInt& lhs, const BigInt& rhs);
bool operator!=(const BigInt& lhs, const BigInt& rhs);
bool operator<(const BigInt& lhs, const BigInt& rhs);
bool operator>(const BigInt& lhs, const BigInt& rhs);
bool operator<=(const BigInt& lhs, const BigInt& rhs);
bool operator>=(const BigInt& lhs, const BigInt& rhs);

#endif  // BIG_INT_H_