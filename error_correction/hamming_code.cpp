#include "hamming_code.hpp"
#include <stdint.h>
#include <iostream>

// std::vector<uint8_t> from_string(std::string data) {
//   uint64_t index = 0;
//   std::vector<uint8_t> output;
//   for (char c : data) {
//     if (index % 8 == 0) {  // a new byte for the output is needed
//       output.push_back(0);
//     }
//     if (c == '1') {
//       output[index / 8] |= 1 << (index % 8);
//     }
//     index += 1;
//   }
//   return output;
// }

// std::string to_string(std::vector<uint8_t>& data) {
//   uint64_t index = 0;
//   std::string output;
//   for (uint8_t byte : data) {
//     for (int8_t i = 0; i < 8; ++i) {
//       if (byte & (1 << i)) {
//         output += '1';
//       } else {
//         output += '0';
//       }
//     }
//   }
//   return output;
// }

std::vector<uint8_t> from_string(std::string data) {
  std::vector<uint8_t> output(data.size());
  for (int i = 0; i < data.size(); ++i) {
    output[i] = (data[i] == '1');
  }
  return output;
}

std::string to_string(std::vector<uint8_t>& data) {
  std::string output(data.size(), '0');
  for (int i = 0; i < data.size(); ++i) {
    output[i] = (data[i] == 0) ? '0' : '1';
  }
  return output;
}

int main(void) {
  HammingCode c;
  auto message = from_string("00110001110000011001010");
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
