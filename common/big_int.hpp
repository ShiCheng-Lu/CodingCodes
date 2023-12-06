#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

    std::uint8_t carry = 0;
    size_t start = shift / 8;
    shift = shift % 8;
    for (std::size_t i = start; i < num.data.size(); ++i) {
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
    out << std::hex;
    if (num.neg) {
      out << '-';
    }
    for (int i = num.data.size() - 1; i >= 0; --i) {
      out << std::setw(2) << std::setfill('0') << (int)num.data[i];
    }
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
};

bool operator==(const BigInt& lhs, const BigInt& rhs) {
  return cmp(lhs, rhs) == 0;
}
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
  return cmp(lhs, rhs) != 0;
}
bool operator<(const BigInt& lhs, const BigInt& rhs) {
  return cmp(lhs, rhs) < 0;
}
bool operator>(const BigInt& lhs, const BigInt& rhs) {
  return cmp(lhs, rhs) > 0;
}
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
  return cmp(lhs, rhs) <= 0;
}
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
  return cmp(lhs, rhs) >= 0;
}

BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
  if (lhs.neg == rhs.neg) {
    BigInt result = add_value(lhs, rhs);
    result.neg = lhs.neg;
    return result;
  }
  if (cmp_abs(lhs, rhs) < 0) {
    BigInt result = sub_value(rhs, lhs);
    result.neg = rhs.neg;
    return result;
  } else {
    BigInt result = sub_value(lhs, rhs);
    result.neg = lhs.neg;
    return result;
  }
}

BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
  if (lhs.neg != rhs.neg) {
    BigInt result = add_value(lhs, rhs);
    result.neg = lhs.neg;
    return result;
  }
  if (cmp_abs(lhs, rhs) < 0) {
    BigInt result = sub_value(rhs, lhs);
    result.neg = !rhs.neg;
    return result;
  } else {
    BigInt result = sub_value(lhs, rhs);
    result.neg = lhs.neg;
    return result;
  }
}

BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
  BigInt result;
  result.data.resize(lhs.data.size() + rhs.data.size());

  for (size_t n = 0; n < rhs.data.size(); ++n) {
    std::uint8_t carry = 0;
    for (size_t i = 0; i < lhs.data.size(); ++i) {
      std::uint8_t product =
          result.get(n + i) + lhs.get(i) * rhs.get(n) + carry;
      carry = (result.get(n + i) + lhs.get(i) * rhs.get(n) + carry) >>
              (sizeof(std::uint8_t) * 8);
      result.data[n + i] = product;
    }
    if (carry) {
      result.data[lhs.data.size() + n] = carry;
    }
  }
  result.prune();
  result.neg = lhs.neg != rhs.neg;
  return result;
}

std::pair<BigInt, BigInt> divide(const BigInt& lhs, const BigInt& rhs) {
  std::vector<BigInt> divisors;
  BigInt remainder(lhs);
  BigInt quotient;
  std::size_t size_diff =
      std::max(lhs.data.size(), rhs.data.size()) - rhs.data.size();

  for (int s = BigInt::item_bits - 1; s >= 0; --s) {
    BigInt divisor = rhs << s;
    divisor.data.insert(divisor.data.begin(), size_diff, 0);
    divisors.push_back(divisor);
  }

  for (std::size_t i = 0; i < size_diff + 1; ++i) {
    quotient.data.insert(quotient.data.begin(), 0);
    for (int s = 0; s < BigInt::item_bits; ++s) {
      if (remainder >= divisors[s]) {
        remainder = remainder - divisors[s];
        quotient.data[0] = quotient.data[0] | (0x80 >> s);
      }
      divisors[s] = divisors[s] >> 8;
    }
  }

  quotient.prune();
  remainder.prune();

  return std::pair(quotient, remainder);
}

BigInt operator/(const BigInt& lhs, const BigInt& rhs) {
  return divide(lhs, rhs).first;
}

BigInt operator%(const BigInt& lhs, const BigInt& rhs) {
  return divide(lhs, rhs).second;
}
