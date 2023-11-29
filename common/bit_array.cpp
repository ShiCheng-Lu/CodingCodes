
#include <bitset>
#include "bit_array.hpp"

int main() {
  std::vector<uint8_t> d = {0b100101};
  BitArray a("0110011");

  //   std::cout << b << std::endl;
  //   std::cout << a.test() << std::endl;
  a[3] = ~a[3];

  std::cout << "array values" << std::endl;
  for (auto i : a) {
    std::cout << i << std::endl;
  }
}