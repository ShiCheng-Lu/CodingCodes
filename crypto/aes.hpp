#include <cstdint>
#include <iostream>
#include <vector>

#include "code.hpp"
#include "gf256.hpp"

/**
 * @brief https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
 *
 */
class AES : public Code {
 public:
  Field<std::uint8_t>& f;

  std::vector<std::uint8_t> s_box;

  std::vector<std::vector<std::uint8_t>> mix_col_matrix;

  void SubBytes(std::vector<std::uint8_t>& data) {
    for (auto& byte : data) {
      std::uint8_t inv = f.div(1, byte);

      byte = inv;
      for (int ii = 1; ii < 5; ++ii) {
        byte = f.add(byte, (inv << ii) | (inv >> (8 - ii)));
      }
      byte = f.add(byte, 0b01100011);
    }
  }

  void ShiftRows(std::vector<std::uint8_t>& data) {
    std::uint8_t temp;
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

  void MixColumns(std::vector<std::uint8_t>& data) {
    std::uint8_t mix_mat[4][4] = {
        {2, 3, 1, 1},
        {1, 2, 3, 1},
        {1, 1, 2, 3},
        {3, 1, 1, 2},
    };
    std::vector<std::uint8_t> result(4);

    for (int column = 0; column < 4; ++column) {
      for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
          std::uint8_t product = f.mul(data[column * 4 + x], mix_mat[x][y]);
          result[x] = f.add(result[x], product);
        }
      }
    }
  }

  void AddRoundKey(std::vector<std::uint8_t>& data, std::vector<std::uint8_t>& key) {
    for (std::uint8_t i = 0; i < data.size(); ++i) {
      data[i] = f.add(data[i], key[i]);
    }
  }

 public:
  AES(Field<std::uint8_t>& f) : f{f} {}

  std::vector<std::uint8_t> encode(std::vector<std::uint8_t> data) { return data; }

  std::vector<std::uint8_t> decode(std::vector<std::uint8_t> data) { return data; }
};