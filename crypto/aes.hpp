#include <stdint.h>
#include <vector>

#include "code.hpp"
#include "gf256.hpp"

class AES : public Code {
  Field<uint8_t> f;

  std::vector<uint8_t> sub_bytes_lookup;

  void SubBytes(std::vector<uint8_t>& data) {
    for (uint8_t i = 0; i < 16; ++i) {
      data[i] = sub_bytes_lookup[data[i]];
    }
  }

  void ShiftRows(std::vector<uint8_t>& data) {}

  void MixColumns(std::vector<uint8_t>& data) {
    std::vector<uint8_t> result(4);

    for (int x = 0; x < 4; ++x) {
      for (int y = 0; y < 4; ++y) {
      }
    }
  }

  void AddRoundKey(std::vector<uint8_t>& data, std::vector<uint8_t>& key) {
    for (uint8_t i = 0; i < 16; ++i) {
      data[i] = data[i] ^ key[i];
    }
  }

 public:
  std::vector<uint8_t> encode(std::vector<uint8_t> data) {}

  std::vector<uint8_t> decode(std::vector<uint8_t> data) {}
};