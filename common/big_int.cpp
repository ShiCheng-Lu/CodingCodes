#include "big_int.hpp"

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
  if (rhs == 0) {
    throw "Division by 0";
  }

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
  quotient.neg = lhs.neg != rhs.neg;
  remainder.neg = lhs.neg;

  return std::pair(quotient, remainder);
}

BigInt operator/(const BigInt& lhs, const BigInt& rhs) {
  return divide(lhs, rhs).first;
}

BigInt operator%(const BigInt& lhs, const BigInt& rhs) {
  return divide(lhs, rhs).second;
}
