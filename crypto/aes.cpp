#include "aes.hpp"
#include <bitset>
#include <iomanip>
#include <iostream>

void print_block(std::vector<uint8_t>& data, size_t block_size) {
  for (int x = 0; x < block_size; ++x) {
    for (int y = 0; y < block_size; ++y) {
      std::cout << std::hex << (int)data[x * block_size + y] << ' ';
    }
    std::cout << std::endl;
  }
}
void print_block(std::vector<uint8_t>& data) {
  print_block(data, 4);
}

int main() {
  GF256 f;
  std::string key_str = "extraterresteral";
  std::vector<uint8_t> key(key_str.begin(), key_str.end());  //(16, 0);
  AES aes{f, key};  // std::vector(key.begin(), key.end())};

  // print_block(aes.s_box, 16);

  vector<uint8_t> v;
  for (int i = 0; i < 16; ++i) {
    v.push_back(i + 1);
  }
  // pad v to 16
  // while (v.size() < 16) {
  //   v.push_back(0);
  // }
  vector<uint8_t> res = aes.encode(v);

  std::cout << std::hex << res << std::endl;
  // hello: d82c0f58df913d63a8b420611d258c41
  //   print_block(aes.s_box);
  // aes.SubBytes(d);
  // print_block(d, 16);

  // std::vector<uint8_t> ve = {
  //     0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
  // };
  // std::cout << std::hex << ve << std::endl;
  // aes.shift_rows(ve);
  // std::cout << std::hex << ve << std::endl;
}
/**
 * 0  4  8  c
 * 5  9  d  1
 * a  e  2  6
 * f  3  7  b
 *
 */