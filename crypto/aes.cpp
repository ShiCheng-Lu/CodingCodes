#include "aes.hpp"
#include <bitset>

void print_block(std::vector<uint8_t>& data, size_t block_size) {
  for (int y = 0; y < block_size; ++y) {
    for (int x = 0; x < block_size; ++x) {
      std::cout << std::hex << (int)data[x * block_size + y] << ' ';
    }
    std::cout << std::endl;
  }
}
void print_block(std::vector<uint8_t>& data) {
  print_block(data, 4);
}

union AES_Column {
  uint8_t ui8[4];
  uint32_t ui32;
};

std::vector<std::vector<AES_Column>> lookup(4);
void setup_aes() {
  GF256 f;

  uint8_t mix_mat[4][4] = {
      {2, 3, 1, 1},
      {1, 2, 3, 1},
      {1, 1, 2, 3},
      {3, 1, 1, 2},
  };

  for (int v = 0; v < UINT8_MAX; ++v) {
    // find the s_box substitution result
    uint8_t s_box_res;
    uint8_t inv = f.div(1, v);
    for (int i = 1; i < 5; ++i) {
      s_box_res = f.add(s_box_res, (inv << i) | (inv >> (8 - i)));
    }
    s_box_res = f.add(s_box_res, 0b01100011);

    for (int row = 0; row < 4; ++row) {  // the row # contribute differently
                                         // to result due to the mix_mat
      AES_Column output;
      for (int i = 0; i < 4; ++i) {
        output.ui8[i] = f.mul(mix_mat[i][row], s_box_res);
      }
      lookup[row].push_back(output);
    }
  }
}

std::vector<uint8_t> optimized_aes(std::vector<uint8_t>& data,
                                   std::vector<uint8_t>& key) {
  GF256 f;
  std::vector<AES_Column> result(4);
  // perform SubBytes, ShiftRows, MixColumns with lookup table
  // each entry contributes to one resulting column, data[r + c * 4] contributes
  // to column (c - r) % 4. the value of data[r + c * 4] is transformed with
  // SubBytes then multiplied by the r-th column of the MixColumns matrix
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result[(j - 1) % 4].ui32 ^= lookup[i][data[i + 4 * j]].ui32;
    }
  }
  // AddRoundKey
  std::vector<uint8_t> output;
  for (int i = 0; i < 16; ++i) {
    output.push_back(f.add(result[i / 4].ui8[i % 4], key[i]));
  }
  return output;
}

std::vector<uint8_t> aes_key_gen(std::vector<uint8_t>& key, int round) {
    

    return key;
}

int main() {
  std::vector<uint8_t> d;
  for (int i = 0; i < 16 * 16; ++i) {
    d.push_back(i);
  }

  GF256 f;
  AES aes{f};

  //   print_block(aes.s_box);
  aes.SubBytes(d);
  print_block(d, 16);
}
