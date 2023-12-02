#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class BigInt {
  std::vector<std::uint8_t> data;

 public:
  BigInt() {}
  BigInt(std::string str_num) {}

  BigInt& operator+(BigInt& other) {
    std::uint8_t carry = 0;
    std::size_t result_size = std::max(data.size(), other.data.size());
    BigInt result;
    for (std::size_t i = 0; i < result_size; ++i) {
      result.data.push_back((data[i] + other.data[i] + carry) & 0xff);
      carry = (data[i] + other.data[i] + carry) >> 8;
    }
    if (carry) {
      result.data.push_back(1);
    }
  }

  BigInt& operator/(BigInt& other) {}

  std::string to_hex_string() {
    std::stringstream ss;
    ss << std::hex;
    for (int i = data.size() - 1; i >= 0; --i) {
      ss << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    return ss.str();
  }
};
