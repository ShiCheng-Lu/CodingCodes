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
