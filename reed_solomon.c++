#include <stdint.h>
#include <iostream>
#include <vector>

#include "code.h"

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

template <typename T>
class ReedSolomon : public Code<T> {
  //  protected:
  Field<T>& f;
  size_t n;
  size_t k;
  vector<T> divisor;
  vector<vector<T>> verifier;

 public:
  /**
   * f : a field of operation
   * prim : primitive element of the field
   * root : the first consecutive roots of the generator polynomial
   * n : code word length
   * k : message length
   */
  ReedSolomon(Field<T>& f, T prim, T root, size_t n, size_t k)
      : f{f}, n{n}, k{k} {
    divisor.resize(n - k + 1);
    divisor[0] = 1;
    for (size_t root_num = 1; root_num < n - k + 1; ++root_num) {
      for (size_t i = root_num; i > 0; --i) {
        divisor[i] = f.add(f.mul(divisor[i - 1], root), divisor[i]);
      }

      // for decoding
      vector<T> poly(n);
      poly[0] = 1;
      for (size_t i = 1; i < n; ++i) {
        poly[i] = f.mul(poly[i - 1], root);
      }
      verifier.push_back(poly);

      // next root
      root = f.mul(root, prim);
    }
  }

  vector<T> encode(vector<T>& data) {
    // s(x) = q(x)g(x) + r(x)
    // polynomial long division to find the remainder
    vector<T> remainder(n - k);
    for (size_t i = 0; i < k; ++i) {
      T mul = f.add(data[i], remainder[0]);

      for (size_t x = 0; x < n - k - 1; ++x) {
        remainder[x] = f.sub(remainder[x + 1], f.mul(divisor[x + 1], mul));
      }
      remainder[n - k - 1] = f.sub(0, f.mul(divisor[n - k], mul));
    }
    for (size_t x = 0; x < n - k; ++x) {
      remainder[x] = f.sub(0, remainder[x]);
    }

    vector<T> output(data);
    output.insert(output.end(), remainder.begin(), remainder.end());
    return output;
  }

  vector<T> decode(vector<T>& data) {
    // validate data
    bool valid = true;
    for (vector<T> poly : verifier) {
      T value = 0;
      for (size_t i = 0; i < n; ++i) {
        value = f.add(value, f.mul(poly[i], data[data.size() - 1 - i]));
      }
      if (value != 0) {
        printf("\nMSG INVALID %d %d\n", poly[0], value);
        valid = false;
        break;
      }
    }

    if (valid) {
      vector<T> output(data);
      return output;
    }

    printf("MSG INVALID\n");
    return vector<T>();
  }
};

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
