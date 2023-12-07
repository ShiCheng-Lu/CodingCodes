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
  AES aes{"extraterresteral"};  // std::vector(key.begin(), key.end())};

  std::cout << std::hex << aes.key << std::endl;
  // print_block(aes.s_box, 16);

  std::string v_str = "hello world";
  std::vector<uint8_t> v(v_str.begin(), v_str.end());
  // std::vector<uint8_t> v(16, 0x10);
  // v[0] = 'a';
  vector<uint8_t> res = aes.encode(v);
  std::cout << std::hex << v << std::endl;

  std::cout << std::hex << res << std::endl;
  std::cout << "DECODE: " << std::endl;

  std::vector<uint8_t> decoded = aes.decode(res);
  std::cout << std::hex << decoded << std::endl;
}
// 8b1ee0dd8dc939214c378cf406ac0fb8