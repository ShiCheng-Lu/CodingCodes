#include "hamming_code.hpp"
#include <stdint.h>
#include <iostream>

HammingCode::HammingCode() {}

std::vector<uint8_t> HammingCode::encode(std::vector<uint8_t>& data) {
  uint64_t parity = 0;
  uint64_t index = 3;  // index of the output byte
  bool total_parity = false;

  std::vector<uint8_t> output(1);
  for (uint8_t byte : data) {
    for (uint8_t i = 0; i < 8; ++i) {
      if (byte & (1 << i)) {  // bit i is set
        output[index / 8] |= 1 << (index % 8);
        parity ^= index;
        total_parity ^= 1;
      }

      index += 1;
      if (index % 8 == 0) {  // a new byte for the output is needed
        output.push_back(0);
      }
      if ((index & (index - 1)) == 0) {  // is a power of 2, parity bit
        index += 1;
      }
    }
  }

  // set parity bits
  index = 1;
  while (parity != 0) {
    if (parity & 1) {
      output[index / 8] |= 1 << (index % 8);
      total_parity ^= 1;
    }
    parity >>= 1;  // shift right by 1
    index *= 2;    // multiply by 2
  }
  // set total parity bit
  output[0] |= total_parity;

  return output;
}

std::vector<uint8_t> HammingCode::decode(std::vector<uint8_t>& data) {
  uint64_t parity = 0;
  bool total_parity = false;

  uint64_t index = 0;  // index of the output byte
  // check parity
  for (uint8_t byte : data) {
    for (uint8_t i = 0; i < 8; ++i) {
      if (byte & (1 << i)) {  // bit i is set
        parity ^= index;
        total_parity ^= 1;
      }
      index += 1;
    }
  }

  // correct errors
  if (parity != 0) {
    if (total_parity == 0) {
      // detected 2 bit error
      throw DecodeError("Detected 2 bit error, unable to correct");
    }
    // 1 bit error index is the parity
    data[parity / 8] ^= 1 << (parity % 8);
  }

  // extract message
  std::vector<uint8_t> output;
  index = 0;
  for (uint64_t i = 0; i < data.size() * 8; ++i) {
    if ((i & (i - 1)) == 0) {
      continue;  // power of 2, this is a parity bit
    }
    if (index % 8 == 0) {
      output.push_back(0);
    }
    if (data[i / 8] & 1 << (i % 8)) {
      output[index / 8] |= 1 << (index % 8);
    }
    index++;
  }
  return output;
}

std::vector<uint8_t> HammingCode::from_string(std::string data) {
  uint64_t index = 0;
  std::vector<uint8_t> output;
  for (char c : data) {
    if (index % 8 == 0) {  // a new byte for the output is needed
      output.push_back(0);
    }
    if (c == '1') {
      output[index / 8] |= 1 << (index % 8);
    }
    index += 1;
  }
  return output;
}

std::string HammingCode::to_string(std::vector<uint8_t>& data) {
  uint64_t index = 0;
  std::string output;
  for (uint8_t byte : data) {
    for (int8_t i = 0; i < 8; ++i) {
      if (byte & (1 << i)) {
        output += '1';
      } else {
        output += '0';
      }
    }
  }
  return output;
}

int main(void) {
  HammingCode c;
  auto message =
      c.from_string("00110011001100110011001100110011001100110011001100110011");
  std::cout << c.to_string(message) << std::endl;

  auto encoded = c.encode(message);
  std::cout << c.to_string(encoded) << std::endl;

  // flip bit
  encoded[5] ^= 1 << 5;

  auto decoded = c.decode(encoded);
  std::cout << c.to_string(decoded) << std::endl;
}
