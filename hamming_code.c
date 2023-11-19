#include <stdint.h>
#include <stdio.h>

void encode_block(uint8_t* data, uint8_t* result, size_t size) {
  size_t parity = 0;
  uint8_t total_parity = 0;

  size_t i_data = 0;  // index of data,
  for (size_t i = 1; i < size; ++i) {
    if ((i & (i - 1)) == 0) {
      // is a power of 2, parity bit
      continue;
    }
    // not a power of 2, data bit
    if (data[i_data / 8] & (1 << (i_data % 8))) {
      // update parity and set in the result array if the bit is set
      result[i / 8] |= (1 << (i % 8));
      parity ^= i;
      total_parity ^= 1;
    }
    i_data++;
  }

  // set parity bits
  size_t i = 1;
  while (parity != 0) {
    if (parity & 1) {
      result[i / 8] |= 1 << (i % 8);
      total_parity ^= 1;
    }
    parity = parity >> 1;
    i = i << 1;
  }

  // set total parity, used to detect 2 bit errors
  result[0] |= total_parity;
}

void encode();

void decode_block(uint8_t *encoded, uint8_t *data, size_t len) {
    size_t 
}

void compress(uint8_t* data, size_t len) {
  for (size_t i = 1; i < len; ++i) {
    if (i % 8 == 0) {
      data[i / 8] = 0;
    }
    data[i / 8] |= (data[i] << (i % 8));
  }
}

void print_bits(uint8_t* data, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    printf("%d", (data[i / 8] >> (i % 8)) & 1);
  }
  printf("\n");
}

void main(void) {
  uint8_t data[] = {0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0};
  compress(data, sizeof(data));
  uint8_t result[2];

  encode_block((uint8_t*)&data, (uint8_t*)&result, 16);
  printf("\n");
  print_bits(result, 16);
}
