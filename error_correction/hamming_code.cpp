#include "hamming_code.hpp"
#include <stdint.h>
#include <iostream>
#include "bit_array.hpp"
#include "bit_string.hpp"

template <typename T>
T from_string(std::string data) {
  T output(data.size());
  for (int i = 0; i < data.size(); ++i) {
    output[i] = (data[i] == '1');
  }
  return output;
}

template <typename T>
std::string to_string(T& data) {
  std::string output(data.size(), '0');
  for (int i = 0; i < data.size(); ++i) {
    output[i] = data[i] ? '0' : '1';
  }
  return output;
}

int main(void) {
  HammingCode c;

  auto message = BitString("00110001110000011001010");
  std::cout << to_string(message) << std::endl;

  auto encoded = c.encode(message);
  std::cout << to_string(encoded) << std::endl;

  // 0011000111
  // ___0_011_000111
  // 00101011100011

  // flip bit
  encoded[5] = ~encoded[5];
  std::cout << to_string(encoded) << std::endl;

  auto decoded = c.decode(encoded);
  std::cout << to_string(decoded) << std::endl;
}
