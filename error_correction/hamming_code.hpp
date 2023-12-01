#ifndef HAMMING_CODE_HPP
#define HAMMING_CODE_HPP

#include <stdint.h>
#include "code.hpp"

#include <iostream>

/**
 * @brief https://en.wikipedia.org/wiki/Hamming_code
 * 
 */
class HammingCode : public Code {
  // return bits needed to represent the value
  int num_parity_bits(uint64_t value) {
    int bits = 0;
    for (int bit_test = 32; bit_test > 0; bit_test >>= 1) {
      if (value >> bit_test != 0) {
        bits += bit_test;
        value >>= bit_test;
      }
    }
    return bits + value;
  }

 public:
  /**
   * @brief Perform Hamming code encoding.
   *
   * @tparam T  a container type, must implement .size(), initialization with
   * size, and operator[] indexing
   * @param data input data to be encoded.
   * @return T encoded data stored in the same container type.
   */
  template <typename T>
  T encode(T& data) {
    uint64_t parity = 0;
    uint64_t index = 3;  // starting index into the output
    bool total_parity = false;
    //
    // create output, size is data size + # parity bits + 1 (total parity)
    T output(data.size() + num_parity_bits(data.size()) + 1);
    for (size_t i = 0; i < data.size(); ++i) {
      if (data[i]) {
        output[index] = true;
        parity ^= index;
        total_parity ^= 1;
      }

      index++;
      if ((index & (index - 1)) == 0) {  // is a power of 2, parity bit
        index++;
      }
    }
    // set parity bits
    index = 1;
    while (parity != 0) {
      if (parity & 1) {
        output[index] = true;
        total_parity ^= 1;
      }
      parity >>= 1;  // shift right by 1
      index *= 2;    // multiply by 2
    }
    // set overall parity bit (for 2 bit error detection)
    output[0] = total_parity;

    return output;
  }

  /**
   * @brief Perform Hamming code decoding.
   *
   * @tparam T a container type, must implement .size(), initialization with
   * size, and operator[] indexing
   * @param data encoded data.
   * @return T decoded data stored in the same container type.
   */
  template <typename T>
  T decode(T& data) {
    uint64_t parity = 0;
    bool total_parity = false;

    size_t index = 0;  // index of the output byte
    // output size is data size - # parity bits - 1 (total parity)
    T output(data.size() - num_parity_bits(data.size()) - 1);
    // check parity
    for (size_t i = 0; i < data.size(); ++i) {
      if (data[i]) {
        parity ^= i;
        total_parity ^= 1;
      }

      if ((i & (i - 1)) == 0) {
        continue;  // power of 2, this is a parity bit
      }
      output[index] = (bool)data[i];
      index++;
    }

    // correct errors
    if (parity != 0) {
      if (total_parity == 0) {
        // detected 2 bit error
        throw CodeError("Detected 2 bit error, unable to correct");
      }
      // 1 bit error, the error index is the parity value
      // if the error index is a not parity bit, output need to be corrected
      if ((parity & (parity - 1)) != 0) {
        // error index is parity - # parity bits stuffed before the index
        index = parity - num_parity_bits(parity) - 1;
        std::cout << "correcting error at " << index << std::endl;
        output[index] = (output[index] ^ 1);
      }
    }
    return output;
  }
};

#endif  // HAMMING_CODE_HPP
