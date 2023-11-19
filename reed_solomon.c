/**
 * An implementation of Reed-Soloman error correction code
 *
 * References:
 * https://en.wikipedia.org/wiki/Reed%E2%80%93Solomon_error_correction
 * https://en.wikipedia.org/wiki/Finite_field_arithmetic#Multiplication
 * https://tomverbeure.github.io/2022/08/07/Reed-Solomon.html#reed-solomon-encoding-through-polynomial-evaluation
 *
 */

#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// GF(256) finite field arithmetics
static uint8_t gf256_inv[256];

uint8_t gf256_add(uint8_t a, uint8_t b) {
  return a ^ b;
}

uint8_t gf256_sub(uint8_t a, uint8_t b) {
  return a ^ b;
}

/**
 * multiply two numbers in GF(256) finite field,
 * with reducing polynomial 0x11b - 0b100011011.
 * https://en.wikipedia.org/wiki/Finite_field_arithmetic#Multiplication
 */
uint8_t gf256_mul(uint8_t a, uint8_t b) {
  uint8_t p = 0;
  for (uint8_t i = 0; i < 8; ++i) {
    p ^= a * (b & 1);
    b >>= 1;
    a = (a << 1) ^ (0b00011011 * (a & 0x80) >> 7);
    // a = (a << 1) ^ (0b10000111 * (a & 0x80) >> 7); // 0b110000111 reduce poly
  }
  return p;
}

/**
 * a / b in GF(256) finite field
 */
uint8_t gf256_div(uint8_t a, uint8_t b) {
  return gf256_mul(a, gf256_inv[b]);
}

/**
 * perform encoding with reed-soloman error correcting code
 * uses a systematic encoding with s = 8, k = len, and n = len + t
 */
void encode(uint8_t* data, size_t len, uint8_t t) {
  for (uint8_t i = 0; i < t; ++i) {
    data[len + i] = 0;
  }

  uint8_t* coef = malloc(len);

  for (uint8_t i = 0; i < len; ++i) {
    for (uint8_t x = 0; x < t; ++x) {
      data[len + x] = gf256_mul(data[len + x], len + x - i);
    }

    for (uint8_t ii = 0; ii < len; ++ii) {
      coef[ii] = gf256_mul(coef[ii], (i != ii) ? (i - ii) : gf256_inv[data[i]]);
    }
  }

  for (uint8_t x = 0; x < t; ++x) {
    for (uint8_t i = 0; i < len; ++i) {
      data[len + x] =
          gf256_mul(coef[i], gf256_mul(data[len + x], gf256_inv[x - i]));
    }
  }

  free(coef);
}

void encode_BCH(uint8_t* data, size_t n, size_t k) {
  // construct g(x)
  uint8_t* divisor = malloc(n - k + 1);
  uint8_t a = 11;  // primitive element of gf256
  uint8_t a_n = 1;
  divisor[0] = 1;
  for (uint8_t root_num = 1; root_num < n - k + 1; ++root_num) {
    for (uint8_t i = root_num; i > 0; --i) {
      divisor[i] = gf256_add(gf256_mul(divisor[i - 1], a_n), divisor[i]);
    }
    a_n = gf256_mul(a_n, a);
  }

  // s(x) = q(x)g(x) + r(x)
  // polynomial long division to find the remainder
  uint8_t* remainder = data + k;  // use end of data as the remainder buffer
  for (uint8_t i = 0; i < k; ++i) {
    uint8_t mul = gf256_add(data[i], remainder[0]);

    for (uint8_t x = 0; x < n - k - 1; ++x) {
      remainder[x] =
          gf256_sub(remainder[x + 1], gf256_mul(divisor[x + 1], mul));
    }
    remainder[n - k - 1] = gf256_sub(0, gf256_mul(divisor[n - k], mul));
  }
  free(divisor);
  // negate the remainder for the encoding.
  // not needed for GF256 since xor 0 does nothing.
  // for (uint8_t x = 0; x < n - k; ++x) {
  //   remainder[x] = gf256_sub(0, remainder[x]);
  // }
}

void init_gf256() {
  //   printf("%d\n", gf256_mul(0x53, 0xCA));
  for (uint8_t a = 1; a > 0; a++) {
    if (gf256_inv[a] != 0) {
      continue;
    }
    for (uint8_t b = 1; b > 0; b++) {
      if (gf256_mul(a, b) == 1) {
        gf256_inv[a] = b;
        gf256_inv[b] = a;
      }
    }
  }
  for (int i = 0; i < 256; ++i) {
    if (gf256_inv[i] == 0) {
      printf("inverse not found for %d\n", i);
    }
  }

  bool passed[256];
  uint8_t n = 1;
  for (int i = 0; i < 256; ++i) {
    passed[n] = true;
    n = gf256_mul(n, 11);
  }
  for (int i = 0; i < 256; ++i) {
    if (!passed[n]) {
      printf("%d\n", n);
    }
  }
}

void main(void) {
  init_gf256();

  uint8_t data[11] = {1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 1};

  encode_BCH(data, 10, 6);

  for (int i = 0; i < 10; ++i) {
    printf("%x ", data[i]);
  }
  // 1 2 3 4 5 7 e8 9 a8 79
}
