#include "code.hpp"

#include "modulo.hpp"

/**
 * @brief https://en.wikipedia.org/wiki/RSA_(cryptosystem)
 *
 */
class RSA : public Code {
  int n;  // modulus
  int e;  // public key
  int d;  // private key

 public:
  // Create and RSA encoder and decoder, p, q, must be prime
  RSA(int p, int q, int e) : e{e} {
    n = p * q;
    int l = lcm(p - 1, q - 1);

    // find private key with extended Euclidean algorithm
    int a = e;
    int b = l;
    int s_0 = 1, s_1 = 0;
    int div = 0;

    while (a != b) {
      if (a > b) {
        a = a - b;
        s_0 -= s_1;
      } else {
        b = b - a;
        s_1 -= s_0;
      }
    }
    d = s_0;

    // public key: (n, e)
    // private key (n, d)
  }

  std::pair<int, int> public_key() { return std::pair{n, e}; }

  std::pair<int, int> private_key() { return std::pair{n, d}; }

  int encode(int data) { return modular_exp(data, e, n); }

  int decode(int data) { return modular_exp(data, d, n); }
};
