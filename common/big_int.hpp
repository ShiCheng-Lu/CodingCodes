#ifndef BIG_INT_H_
#define BIG_INT_H_

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// #include "data.hpp"

class BigInt {
 public:
  std::vector<std::uint8_t> data;
  static const size_t item_bits = (sizeof(std::uint8_t) * 8);
  bool neg = false;

  void prune() {
    while (data.size() > 0 && data.back() == 0) {
      data.pop_back();
    }
  }

  std::uint8_t get(size_t idx) const {
    if (idx >= data.size()) {
      return 0;
    }
    return data[idx];
  }

  friend BigInt add_value(const BigInt& lhs, const BigInt& rhs) {
    std::uint8_t carry = 0;
    std::size_t len = std::max(lhs.data.size(), rhs.data.size());
    BigInt result;
    for (std::size_t i = 0; i < len; ++i) {
      result.data.push_back((lhs.get(i) + rhs.get(i) + carry));
      carry = (lhs.get(i) + rhs.get(i) + carry) >> result.item_bits;
    }
    if (carry) {
      result.data.push_back(1);
    }
    result.prune();
    return result;
  }

  // lhs must be >= rhs
  friend BigInt sub_value(const BigInt& lhs, const BigInt& rhs) {
    // lhs must be >= rhs;
    std::uint8_t carry = 0;
    std::size_t len = std::max(lhs.data.size(), rhs.data.size());
    BigInt result;
    for (std::size_t i = 0; i < len; ++i) {
      result.data.push_back((lhs.get(i) - rhs.get(i) - carry));
      carry = (lhs.get(i) - rhs.get(i) - carry) < 0;
    }
    result.prune();
    return result;
  }

  // count trailing zeros
  size_t ctz() const {
    size_t zeros = 0;
    std::uint8_t byte;
    for (std::size_t i = 0; i < data.size(); ++i) {
      if (data[i] != 0) {
        byte = data[i];
        break;
      }
      zeros += 8;
    }
    for (std::uint8_t i = 0; i < 8; ++i) {
      if ((byte >> i) & 1) {
        return i + zeros;
      }
    }
  }

 public:
  BigInt() {}
  BigInt(std::string str_num) {
    char str[3] = "00";
    for (int i = str_num.size() - 2; i >= -1; i -= 2) {
      str[0] = (i == -1) ? '0' : str_num[i];
      str[1] = str_num[i + 1];
      data.push_back(strtol(str, NULL, 16));
    }
    prune();
    if (str_num[0] == '-') {
      neg = true;
    }
  }
  BigInt(const char* num_chars) {
    std::string str_num(num_chars);
    if (str_num[0] == '0' && str_num[1] == 'x') {
      // string is a hex value
      char str[3] = "00";
      for (int i = str_num.size() - 2; i >= 1; i -= 2) {
        str[0] = (i == 1) ? '0' : str_num[i];
        str[1] = str_num[i + 1];
        data.push_back(strtol(str, NULL, 16));
      }
      prune();
      if (str_num[0] == '-') {
        neg = true;
      }
    } else {
      // decode as base 10
      for (int i = 0; i < str_num.size(); ++i) {
        *this = (*this * BigInt(10)) + BigInt(str_num[i] - '0');
      }
    }
  }
  BigInt(int num) {
    if (num < 0) {
      neg = true;
      num *= -1;
    }
    while (num != 0) {
      data.push_back(num & 0xff);
      num >>= 8;
    }
  }
  BigInt(std::vector<std::uint8_t>& data) : data{data} {}

  std::vector<std::uint8_t> vector() { return data; }

  explicit operator bool() const {
    for (auto v : data) {
      if (v != 0) {
        return true;
      }
    }
    return false;
  }

  std::string to_hex_string() {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

  friend BigInt operator<<(const BigInt& num, unsigned int shift) {
    BigInt result;
    //
    for (std::size_t i = 0; i < shift / 8; ++i) {
      result.data.push_back(0);
    }
    shift = shift % 8;
    //
    std::uint8_t carry = 0;
    for (std::size_t i = 0; i < num.data.size(); ++i) {
      result.data.push_back((num.data[i] << shift) + carry);
      carry = num.data[i] >> (8 - shift);
    }
    if (carry) {
      result.data.push_back(carry);
    }
    result.neg = num.neg;
    result.prune();
    return result;
  }

  friend BigInt operator>>(const BigInt& num, unsigned int shift) {
    BigInt result;

    size_t start = shift / 8;
    shift = shift % 8;
    std::uint8_t carry = num.get(start) >> shift;
    for (std::size_t i = start + 1; i < num.data.size(); ++i) {
      result.data.push_back((num.data[i] << (8 - shift)) + carry);
      carry = num.data[i] >> shift;
    }
    if (carry) {
      result.data.push_back(carry);
    }
    result.neg = num.neg;
    result.prune();
    return result;
  }

  // undefined behaviour if values are signed
  friend BigInt operator&(const BigInt& lhs, const BigInt& rhs) {
    std::size_t len = std::max(lhs.data.size(), rhs.data.size());
    BigInt result;
    for (std::size_t i = 0; i < len; ++i) {
      result.data.push_back(lhs.get(i) & rhs.get(i));
    }
    result.prune();
    return result;
  }

  // undefined behaviour if values are signed
  friend BigInt operator|(const BigInt& lhs, const BigInt& rhs) {
    std::size_t len = std::max(lhs.data.size(), rhs.data.size());
    BigInt result;
    for (std::size_t i = 0; i < len; ++i) {
      result.data.push_back(lhs.get(i) | rhs.get(i));
    }
    result.prune();
    return result;
  }

  friend std::ostream& operator<<(std::ostream& out, const BigInt& num) {
    if (num.data.size() == 0) {
      return out << '0';
    }
    out << std::hex;
    if (num.neg) {
      out << '-';
    }
    // out << num.data;
    return out;
  }

  friend int cmp_abs(const BigInt& lhs, const BigInt& rhs) {
    if (lhs.data.size() != rhs.data.size()) {
      // if one is longer, then it's larger
      return (lhs.data.size() - rhs.data.size());
    }
    for (size_t i = lhs.data.size(); i > 0; --i) {
      if (lhs.data[i - 1] != rhs.data[i - 1]) {
        // if the value is larger, then it's larger
        return (lhs.data[i - 1] - rhs.data[i - 1]);
      }
    }
    return 0;
  }
  friend int cmp(const BigInt& lhs, const BigInt& rhs) {
    if (lhs.neg != rhs.neg) {
      // if two number does not have the same sign, lhs is < if it's negative
      return rhs.neg - lhs.neg;
    }
    // compare absolute values, if numbers are negative, result is reversed
    return cmp_abs(lhs, rhs) * (lhs.neg ? -1 : 1);
  }
  friend std::pair<BigInt, BigInt> divide(const BigInt& lhs, const BigInt& rhs);
  friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);
  friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
  friend BigInt operator*(const BigInt& lhs, const BigInt& rhs);
  friend BigInt operator/(const BigInt& lhs, const BigInt& rhs);
  friend BigInt operator%(const BigInt& lhs, const BigInt& rhs);
};

bool operator==(const BigInt& lhs, const BigInt& rhs);
bool operator!=(const BigInt& lhs, const BigInt& rhs);
bool operator<(const BigInt& lhs, const BigInt& rhs);
bool operator>(const BigInt& lhs, const BigInt& rhs);
bool operator<=(const BigInt& lhs, const BigInt& rhs);
bool operator>=(const BigInt& lhs, const BigInt& rhs);

#endif  // BIG_INT_H_