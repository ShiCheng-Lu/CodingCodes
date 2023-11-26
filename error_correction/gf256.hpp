#ifndef _GF256_H_
#define _GF256_H_

#include <stdint.h>
#include <iostream>
#include <vector>

#include "code.hpp"

using namespace std;

template <typename T>
class Field {
 public:
  virtual T add(T a, T b) { return a + b; }
  virtual T sub(T a, T b) { return a - b; }
  virtual T mul(T a, T b) { return a * b; }
  virtual T div(T a, T b) { return a / b; }
};

class GF256 : public Field<uint8_t> {
  uint8_t inv[256];

 public:
  GF256() {
    for (uint8_t a = 1; a > 0; a++) {
      if (inv[a] != 0) {
        continue;
      }
      for (uint8_t b = 1; b > 0; b++) {
        if (mul(a, b) == 1) {
          inv[a] = b;
          inv[b] = a;
        }
      }
    }
  }

  uint8_t add(uint8_t a, uint8_t b) { return a ^ b; }
  uint8_t sub(uint8_t a, uint8_t b) { return a ^ b; }
  uint8_t mul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (uint8_t i = 0; i < 8; ++i) {
      p ^= a * (b & 1);
      b >>= 1;
      a = (a << 1) ^ (0x1b * (a & 0x80) >> 7);
    }
    return p;
  }
  uint8_t div(uint8_t a, uint8_t b) {
    if (b != 0) {
      return mul(a, inv[b]);
    }
    return 0;
  }
};

#endif  // _GF256_H_
