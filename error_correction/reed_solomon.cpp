#include <stdint.h>
#include <iostream>
#include <vector>

#include "reed_solomon.hpp"

using namespace std;

int main(void) {
  auto gf256 = GF256();
  auto field = Field<int>();

  auto rs = ReedSolomon<uint8_t>(gf256, 11, 1, 10, 6);

  vector<uint8_t> data{1, 2, 3, 4, 5, 6};
  vector<uint8_t> encoded = rs.encode(data);
  for (uint8_t i : encoded) {
    printf("%x ", i);
  }
  printf("\n");
  rs.decode(encoded);
}
