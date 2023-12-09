#include "big_int.hpp"

#include <functional>

/**
 * @brief remove extra 0 elements in the data vector
 *
 * @param data
 */
static void prune(std::vector<std::uint8_t>& data) {
    while (data.size() > 0 && data.back() == 0) {
        data.pop_back();
    }
}

/**
 * @brief apply an operator on every element of the array
 *
 * @param lhs
 * @param rhs
 * @param output
 * @param func
 */
template <typename Function>
static void apply(const std::vector<std::uint8_t>& lhs,
                  const std::vector<std::uint8_t>& rhs,
                  std::vector<std::uint8_t>& output,
                  Function& func) {
    if (lhs.size() < rhs.size()) {
        return apply(rhs, lhs, output, func);
    }
    output.resize(lhs.size());
    for (std::size_t i = 0; i < rhs.size(); ++i) {
        output[i] = func(lhs[i], rhs[i]);
    }
    for (std::size_t i = rhs.size(); i < lhs.size(); ++i) {
        output[i] = func(lhs[i], 0);
    }
}
template <typename Function>
static void apply(const std::vector<std::uint8_t>& lhs,
                  const std::vector<std::uint8_t>& rhs,
                  std::vector<std::uint8_t>& output,
                  Function&& func) {
    apply(lhs, rhs, output, func);
}

struct Adder {
    std::uint8_t carry = 0;
    std::uint8_t operator()(const std::uint8_t& lhs, const std::uint8_t& rhs) {
        std::uint16_t value = lhs + rhs + carry;
        carry = value > UINT8_MAX;
        return value;
    }
};

struct Subtractor {
    std::uint8_t carry = 0;
    std::uint8_t operator()(const std::uint8_t& lhs, const std::uint8_t& rhs) {
        std::uint16_t value = lhs - rhs - carry;
        carry = value > UINT8_MAX;  // negative if it underflowed
        return value;
    }
};

/**
 * @brief add two values store the result in |result|
 *
 * @param lhs
 * @param rhs
 * @param result
 */
static void add_value(const std::vector<std::uint8_t>& lhs,
                      const std::vector<std::uint8_t>& rhs,
                      std::vector<std::uint8_t>& result) {
    Adder adder = {0};
    apply(lhs, rhs, result, adder);
    if (adder.carry != 0) {
        result.push_back(1);
    }
}

/**
 * @brief subtract two values, lhs must be >= rhs, store the result in |result|
 *
 * @param lhs
 * @param rhs
 * @param result
 */
static void sub_value(const std::vector<std::uint8_t>& lhs,
                      const std::vector<std::uint8_t>& rhs,
                      std::vector<std::uint8_t>& result) {
    Subtractor subtractor;
    apply(lhs, rhs, result, subtractor);
    prune(result);
}

/**
 * @brief compare two arrays
 *
 * @param lhs
 * @param rhs
 * @return int
 */
static int cmp_abs(const std::vector<std::uint8_t>& lhs,
                   const std::vector<std::uint8_t>& rhs) {
    if (lhs.size() != rhs.size()) {
        // if one is longer, then it's larger
        return (lhs.size() - rhs.size());
    }
    for (size_t i = lhs.size(); i > 0; --i) {
        if (lhs[i - 1] != rhs[i - 1]) {
            // if the value is larger, then it's larger
            return (lhs[i - 1] - rhs[i - 1]);
        }
    }
    return 0;
}

// BigInt class methods
BigInt::BigInt(std::string str_num) {
    char str[3] = "00";
    for (int i = str_num.size() - 2; i >= -1; i -= 2) {
        str[0] = (i == -1) ? '0' : str_num[i];
        str[1] = str_num[i + 1];
        data.push_back(strtol(str, NULL, 16));
    }
    prune(data);
    if (str_num[0] == '-') {
        neg = true;
    }
}
BigInt::BigInt(const char* num_chars) {
    std::string str_num(num_chars);
    if (str_num[0] == '0' && str_num[1] == 'x') {
        // string is a hex value
        char str[3] = "00";
        for (int i = str_num.size() - 2; i >= 1; i -= 2) {
            str[0] = (i == 1) ? '0' : str_num[i];
            str[1] = str_num[i + 1];
            data.push_back(strtol(str, NULL, 16));
        }
        prune(data);
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
BigInt::BigInt(int num) {
    if (num < 0) {
        neg = true;
        num *= -1;
    }
    while (num != 0) {
        data.push_back(num & 0xff);
        num >>= 8;
    }
}
BigInt::BigInt(const std::vector<std::uint8_t>& data) : data{data} {}

BigInt& BigInt::operator-=(const BigInt& other) {
    if (neg != other.neg) {
        add_value(data, other.data, data);
    } else if (cmp_abs(data, other.data) < 0) {
        sub_value(other.data, data, data);
        neg = !neg;
    } else {
        sub_value(data, other.data, data);
    }
    return *this;
}
BigInt& BigInt::operator+=(const BigInt& other) {
    if (neg == other.neg) {
        add_value(data, other.data, data);
    } else if (cmp_abs(data, other.data) < 0) {
        sub_value(other.data, data, data);
        neg = other.neg;
    } else {
        sub_value(data, other.data, data);
    }
    return *this;
}

BigInt& BigInt::operator&=(const BigInt& other) {
    apply(data, other.data, data, std::bit_and<std::uint8_t>());
    prune(data);
    return *this;
}
BigInt& BigInt::operator|=(const BigInt& other) {
    apply(data, other.data, data, std::bit_or<std::uint8_t>());
    return *this;
}
BigInt& BigInt::operator^=(const BigInt& other) {
    apply(data, other.data, data, std::bit_xor<std::uint8_t>{});
    prune(data);
    return *this;
}

// functions

/**
 * @brief return negative if lhs < rhs, 0 if lhs == rhs, positive if lhs > rhs
 *
 * @param lhs
 * @param rhs
 * @return int
 */
int cmp(const BigInt& lhs, const BigInt& rhs) {
    if (lhs.neg != rhs.neg) {
        // if two number have different signs, lhs is < if it's negative
        return rhs.neg - lhs.neg;
    }
    // compare absolute values, if numbers are negative, result is reversed
    return cmp_abs(lhs.data, rhs.data) * (lhs.neg ? -1 : 1);
}

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
    BigInt result;
    if (lhs.neg == rhs.neg) {
        add_value(lhs.data, rhs.data, result.data);
        result.neg = lhs.neg;
    } else if (cmp_abs(lhs.data, rhs.data) < 0) {
        sub_value(rhs.data, lhs.data, result.data);
        result.neg = rhs.neg;
    } else {
        sub_value(lhs.data, rhs.data, result.data);
        result.neg = lhs.neg;
    }
    return result;
}

BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
    BigInt result;
    if (lhs.neg != rhs.neg) {
        add_value(lhs.data, rhs.data, result.data);
        result.neg = lhs.neg;
    } else if (cmp_abs(lhs.data, rhs.data) < 0) {
        sub_value(rhs.data, lhs.data, result.data);
        result.neg = !rhs.neg;
    } else {
        sub_value(lhs.data, rhs.data, result.data);
        result.neg = lhs.neg;
    }
    return result;
}

BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
    BigInt result;
    result.data.resize(lhs.data.size() + rhs.data.size());

    for (size_t n = 0; n < rhs.data.size(); ++n) {
        std::uint8_t carry = 0;
        for (size_t i = 0; i < lhs.data.size(); ++i) {
            std::uint8_t product =
                result.data[n + i] + lhs.data[i] * rhs.data[n] + carry;
            carry =
                (result.data[n + i] + lhs.data[i] * rhs.data[n] + carry) >> 8;
            result.data[n + i] = product;
        }
        if (carry) {
            result.data[lhs.data.size() + n] = carry;
        }
    }
    prune(result.data);
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

    for (int s = 8 - 1; s >= 0; --s) {
        BigInt divisor = rhs << s;
        divisor.data.insert(divisor.data.begin(), size_diff, 0);
        divisors.push_back(divisor);
    }

    for (std::size_t i = 0; i < size_diff + 1; ++i) {
        quotient.data.insert(quotient.data.begin(), 0);
        for (int s = 0; s < 8; ++s) {
            if (remainder >= divisors[s]) {
                remainder = remainder - divisors[s];
                quotient.data[0] = quotient.data[0] | (0x80 >> s);
            }
            divisors[s] = divisors[s] >> 8;
        }
    }

    prune(quotient.data);
    prune(remainder.data);
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

BigInt operator<<(const BigInt& num, unsigned int shift) {
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
    return result;
}

BigInt operator>>(const BigInt& num, unsigned int shift) {
    BigInt result;

    size_t start = shift / 8;
    shift = shift % 8;

    if (num.data.size() <= start) {
        return result;
    }

    std::uint8_t carry = num.data[start] >> shift;
    for (std::size_t i = start + 1; i < num.data.size(); ++i) {
        result.data.push_back((num.data[i] << (8 - shift)) + carry);
        carry = num.data[i] >> shift;
    }
    if (carry) {
        result.data.push_back(carry);
    }
    result.neg = num.neg;
    prune(result.data);
    return result;
}

BigInt operator&(const BigInt& lhs, const BigInt& rhs) {
    BigInt result;
    apply(lhs.data, rhs.data, result.data, std::bit_and<std::uint8_t>());
    prune(result.data);
    return result;
}
BigInt operator|(const BigInt& lhs, const BigInt& rhs) {
    BigInt result;
    apply(lhs.data, rhs.data, result.data, std::bit_or<std::uint8_t>());
    return result;
}
BigInt operator^(const BigInt& lhs, const BigInt& rhs) {
    BigInt result;
    apply(lhs.data, rhs.data, result.data, std::bit_xor<std::uint8_t>());
    prune(result.data);
    return result;
}

std::string BigInt::to_hex_string() {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

std::ostream& operator<<(std::ostream& out, const BigInt& num) {
    if (num.data.size() == 0) {
        return out << '0';
    }
    if (num.neg) {
        out << '-';
    }
    out << std::hex << num.data;
    return out;
}
