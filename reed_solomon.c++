#include <stdint.h>
#include <vector>

using namespace std;

template <typename T>
class Field {
 public:
  virtual T add(T a, T b) = 0;
  virtual T sub(T a, T b) = 0;
  virtual T mul(T a, T b) = 0;
  virtual T div(T a, T b) = 0;
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

template <typename T>
class ReedSolomon {
  Field<T> f;
  vector<T> divisor;

 public:
  /**
   * f : a field of operation
   * prim : primitive element of the field
   * root : the first consecutive roots of the generator polynomial
   * n : code word length
   * k : message length
   */
  ReedSolomon(Field<T> f, T prim, T root, size_t n, size_t k) {
    divisor.resize(n - k + 1);
    divisor[0] = 1;
    for (uint8_t root_num = 1; root_num < n - k + 1; ++root_num) {
      for (uint8_t i = root_num; i > 0; --i) {
        divisor[i] = f.add(f.mul(divisor[i - 1], root), divisor[i]);
      }
      root = f.mul(root, prim);
    }
  }

  void encode(T* data) {
    // s(x) = q(x)g(x) + r(x)
    // polynomial long division to find the remainder
    T* remainder = data + k;  // use end of data as the remainder buffer
    for (size_t i = 0; i < k; ++i) {
      T mul = f.add(data[i], remainder[0]);

      for (size_t x = 0; x < n - k - 1; ++x) {
        remainder[x] = f.sub(remainder[x + 1], f.mul(divisor[x + 1], mul));
      }
      remainder[n - k - 1] = f.sub(0, f.mul(divisor[n - k], mul));
    }
    for (size_t x = 0; x < n - k) {
      remainder[x] = f.sub(0, remainder[x]);
    }
  }
};

#include <stdio.h>
int main(void) {
  auto gf256 = GF256();

  auto rs = ReedSolomon<uint8_t>(gf256, 11, 1, 10, 6);
}
