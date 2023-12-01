#include <stdint.h>
#include <iostream>
#include <vector>

#include "code.hpp"
#include "gf256.hpp"

class AES : public Code {
 public:
  Field<uint8_t>& f;

  std::vector<uint8_t> s_box;

  std::vector<std::vector<uint8_t>> mix_col_matrix;

  void SubBytes(std::vector<uint8_t>& data) {
    for (auto& byte : data) {
      uint8_t inv = f.div(1, byte);

      byte = inv;
      for (int ii = 1; ii < 5; ++ii) {
        byte = f.add(byte, (inv << ii) | (inv >> (8 - ii)));
      }
      byte = f.add(byte, 0b01100011);
    }
  }

  void ShiftRows(std::vector<uint8_t>& data) {
    uint8_t temp;
    // shift row 2 (left 1)
    temp = data[1];
    data[1] = data[5];
    data[5] = data[9];
    data[9] = data[13];
    data[13] = temp;
    // shift row 3 (left 2)
    std::swap(data[2], data[10]);
    std::swap(data[6], data[14]);
    // shift row 4 (right 1)
    temp = data[15];
    data[15] = data[11];
    data[11] = data[7];
    data[7] = data[3];
    data[3] = temp;
  }

  void MixColumns(std::vector<uint8_t>& data) {
    std::vector<uint8_t> result(4);

    for (int column = 0; column < 4; ++column) {
      for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
          result[x] += data[column * 4 + x] * mix_col_matrix[x][y];
        }
      }
    }
    // for (int i = 0; i <)
  }

  void AddRoundKey(std::vector<uint8_t>& data, std::vector<uint8_t>& key) {
    for (uint8_t i = 0; i < data.size(); ++i) {
      data[i] = f.add(data[i], key[i]);
    }
  }

 public:
  AES(Field<uint8_t>& f) : f{f} {}

  std::vector<uint8_t> encode(std::vector<uint8_t> data) { return data; }

  std::vector<uint8_t> decode(std::vector<uint8_t> data) { return data; }
};