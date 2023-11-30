#include <iostream>
#include <string>
#include <vector>

class BigInt {
  std::vector<uint8_t> number;

 public:
  BigInt() {}
  BigInt(std::string str_num) {}

  BigInt& operator+(BigInt& other) {
    uint8_t carry = 0;
    size_t result_size = std::max(number.size(), other.number.size());
    BigInt result;
    for (size_t i = 0; i < result_size; ++i) {
      result.number.push_back((number[i] + other.number[i] + carry) & 0xff);
      carry = (number[i] + other.number[i] + carry) >> 8;
    }
    if (carry) {
      result.number.push_back(1);
    }
  }

  //   friend operator<<(ostream& out, BigInt& num) {

  //   }
};
