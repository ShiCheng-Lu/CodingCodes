#include <cstdint>
#include <iostream>
#include <vector>

#include "code.hpp"
#include "gf256.hpp"

/**
 * @brief https://en.wikipedia.org/wiki/Advanced_Encryption_Standard
 *
 * currently only implements AES-128
 *
 */
class AES : public Code {
  union AES_Column {
    uint8_t ui8[4];
    uint32_t ui32;
  };

 public:
  GF256 f;
  std::vector<uint8_t> key;

  std::vector<std::uint8_t> s_box;
  std::vector<std::uint8_t> s_box_inv;
  std::vector<std::vector<AES_Column>> mix_col;
  std::vector<std::vector<AES_Column>> mix_col_inv;

  void add_round_key(std::vector<std::uint8_t>& data,
                     const std::vector<std::uint8_t>& key) {
    for (int i = 0; i < data.size(); ++i) {
      data[i] = f.add(data[i], key[i]);
    }
  }

  void sub_bytes(std::vector<std::uint8_t>& data,
                 const std::vector<std::uint8_t>& sub) {
    for (int i = 0; i < data.size(); ++i) {
      data[i] = sub[data[i]];
    }
  }

  void shift_rows(std::vector<std::uint8_t>& data) {
    // shift
    std::uint8_t temp[4];
    for (int row = 1; row < 4; ++row) {
      for (int i = 0; i < 4; ++i) {
        temp[i] = data[(row + (i + row) * 4) % 16];
      }
      for (int i = 0; i < 4; ++i) {
        data[row + i * 4] = temp[i];
      }
    }
  }

  void round(std::vector<std::uint8_t>& data,
             const std::vector<std::uint8_t>& key) {
    std::vector<AES_Column> result(4);
    // perform SubBytes, ShiftRows, MixColumns with lookup table
    // each entry contributes to one resulting column, data[r + c * 4]
    // contributes to column (c - r) % 4. the value of data[r + c * 4] is
    // transformed with SubBytes then multiplied by the r-th column of the
    // MixColumns matrix
    sub_bytes(data, s_box);

    for (int i = 0; i < 16; ++i) {
      // contributes to one column            add
      result[(16 - i + i / 4) % 4].ui32 ^= mix_col[i % 4][data[i]].ui32;
    }
    // set data values from AES_Column back to data
    for (int i = 0; i < 16; ++i) {
      data[i] = result[i / 4].ui8[i % 4];
    }
    // AddRoundKey
    add_round_key(data, key);
  }

  void round_inv(std::vector<std::uint8_t>& data,
                 const std::vector<std::uint8_t>& key) {
    std::vector<AES_Column> result(4);

    // add in gf256 is the same as subtract
    add_round_key(data, key);

    // for (int i = 0; i < 16; ++i) {
    //   result[];
    // }
    // shift_rows();
  }

  void next_key(std::vector<std::uint8_t>& key, std::uint8_t& rc) {
    // first byte is additionally xor-ed by round constant
    key[0] = f.add(key[0], rc);
    // first 4 bytes of the new key
    for (int i = 0; i < 4; ++i) {
      key[i] = f.add(key[i], s_box[key[12 + (i + 1) % 4]]);
    }
    // rest of the keys
    for (int i = 4; i < 16; ++i) {
      key[i] = f.add(key[i - 4], key[i]);
    }
    // update round key
    rc = f.mul(rc, 2);
  }

 public:
  AES(std::string key_str)
      : AES(std::vector<std::uint8_t>(key_str.begin(), key_str.end())) {}
  AES(std::vector<uint8_t> key)
      : key{key}, mix_col(4), mix_col_inv(4), s_box(0x100), s_box_inv(0x100) {
    // matrix used for mix column
    uint8_t mix_mat[4][4] = {
        {2, 3, 1, 1},
        {1, 2, 3, 1},
        {1, 1, 2, 3},
        {3, 1, 1, 2},
    };
    uint8_t mix_mat_inv[4][4] = {
        {14, 11, 13, 9},
        {9, 14, 11, 13},
        {13, 9, 14, 11},
        {11, 13, 9, 14},
    };
    // setup s_box and
    for (int v = 0; v <= UINT8_MAX; ++v) {
      // find the s_box substitution result
      uint8_t s_box_res = 0;
      uint8_t inv = f.div(1, v);
      for (int i = 0; i < 5; ++i) {
        s_box_res = f.add(s_box_res, (inv << i) | (inv >> (8 - i)));
      }
      s_box_res = f.add(s_box_res, 0b01100011);
      s_box[v] = s_box_res;
      // inverse s_box
      s_box_inv[s_box_res] = v;

      // find mix_column result for v
      for (int row = 0; row < 4; ++row) {  // the row # contribute differently
                                           // to result due to the mix_mat
        AES_Column output;
        AES_Column output_inv;
        for (int i = 0; i < 4; ++i) {
          output.ui8[i] = f.mul(mix_mat[i][row], v);
          output_inv.ui8[i] = f.mul(mix_mat_inv[i][row], v);
        }
        mix_col[row].push_back(output);
        mix_col_inv[row].push_back(output);
      }
    }
  }

  std::vector<std::uint8_t> encode(std::vector<std::uint8_t>& data) {
    std::vector<std::uint8_t> subkey(key);
    std::uint8_t rc = 1;
    std::vector<std::uint8_t> output(data);
    output.insert(output.end(), 16 - std::min(output.size(), (std::size_t)16),
                  16 - output.size());  // pad end with pad length
    // 1. add round key
    for (int i = 0; i < 16; ++i) {
      output[i] = f.add(output[i], key[i]);
    }
    // return output;
    next_key(subkey, rc);
    // 2. 9 rounds of encryption
    for (int i = 0; i < 9; ++i) {
      round(output, subkey);
      next_key(subkey, rc);
    }
    // 3. final round
    sub_bytes(output, s_box);
    shift_rows(output);
    add_round_key(output, subkey);
    return output;
  }

  std::vector<std::uint8_t> decode(std::vector<std::uint8_t>& data) {
    std::vector<std::uint8_t> subkey(key);
    std::vector<std::vector<std::uint8_t>> subkeys{subkey};
    std::uint8_t rc = 1;
    for (int i = 0; i < 10; ++i) {
      next_key(subkey, rc);
      subkeys.push_back(subkey);
    }
    for (auto key : subkeys) {
      std::cout << std::hex << key << std::endl;
    }
    std::cout << "^ KEYS ^" << std::endl;

    std::vector<std::uint8_t> output(data);

    add_round_key(output, subkeys.back());
    // subkeys.pop_back();
    std::cout << std::hex << output << std::endl;

    shift_rows(output);
    shift_rows(output);
    shift_rows(output);

    sub_bytes(output, s_box_inv);

    return output;
  }
};