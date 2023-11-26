#ifndef HAMMING_CODE_HPP
#define HAMMING_CODE_HPP

#include <stdint.h>
#include "code.hpp"

class HammingCode : public Code<uint8_t> {
 protected:
  uint8_t block_size = 64;  //
 public:
  /**
   * @brief Construct a new Hamming Code encoder/decoder
   *
   */
  HammingCode();

  /**
   * @brief perform Reed-Solomon encoding on the data
   *
   * @param data input data
   * @return vector<T> encoded data
   */
  std::vector<uint8_t> encode(std::vector<uint8_t>& data);

  /**
   * @brief perform Reed-Solomon decoding on the data
   *
   * @param data encoded input data
   * @return vector<T> decoded data
   */
  std::vector<uint8_t> decode(std::vector<uint8_t>& data);

  std::vector<uint8_t> from_string(std::string data);
  std::string to_string(std::vector<uint8_t>& data);
};

#endif  // HAMMING_CODE_HPP
