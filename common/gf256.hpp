#ifndef _GF256_H_
#define _GF256_H_

#include <stdint.h>
#include <iostream>
#include <vector>

#include "code.hpp"

using namespace std;

/**
 * @brief a set on which addition, subtraction, multiplication, and division are
 * defined and behave as the corresponding operations on rational and real
 * numbers do.
 *
 * https://en.wikipedia.org/wiki/Field_(mathematics)
 *
 * @tparam T the type for the numbers in the field
 */
template <typename T>
class Field {
 public:
  virtual T add(T a, T b) { return a + b; }
  virtual T sub(T a, T b) { return a - b; }
  virtual T mul(T a, T b) { return a * b; }
  virtual T div(T a, T b) { return a / b; }
};

class GF256 : public Field<std::uint8_t> {
  std::uint8_t inv[256] = {0};

 public:
  // std::uint8_t reducing_poly;
  GF256() {
    for (std::uint8_t a = 1; a > 0; a++) {
      if (inv[a] != 0) {
        continue;
      }
      for (std::uint8_t b = 1; b > 0; b++) {
        if (mul(a, b) == 1) {
          inv[a] = b;
          inv[b] = a;
        }
      }
    }
  }

  std::uint8_t add(std::uint8_t a, std::uint8_t b) { return a ^ b; }
  std::uint8_t sub(std::uint8_t a, std::uint8_t b) { return a ^ b; }
  std::uint8_t mul(std::uint8_t a, std::uint8_t b) {
    std::uint8_t p = 0;
    for (std::uint8_t i = 0; i < 8; ++i) {
      p ^= a * (b & 1);
      b >>= 1;
      a = (a << 1) ^ (0x1b * ((a & 0x80) >> 7));
    }
    return p;
  }
  std::uint8_t div(std::uint8_t a, std::uint8_t b) {
    if (a == 0 || b == 0) {
      return 0;
    } else if (a == 1) {
      return inv[b];
    } else {
      return mul(a, inv[b]);
    }
  }
};

#endif  // _GF256_H_
