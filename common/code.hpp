#ifndef _CODE_H_
#define _CODE_H_

#include <iomanip>
#include <string>
#include <vector>

class CodeError {
 public:
  std::string message;

  CodeError(std::string message) : message{message} {}
};

class Code {
 public:
  virtual std::vector<std::uint8_t> encode(std::vector<std::uint8_t>& data) {
    throw CodeError("encoding not implemented");
  }
  virtual std::vector<std::uint8_t> decode(std::vector<std::uint8_t>& data) {
    throw CodeError("decoding not implemented");
  }
};

std::ostream& operator<<(std::ostream& out, const std::vector<uint8_t>& data) {
  bool output_hex = out.flags() & out.hex;
  for (uint8_t byte : data) {
    if (output_hex) {
      out << std::hex << std::setfill('0') << std::setw(2) << (int)byte;
    } else {
      out << (char)byte;
    }
  }
  return out;
}

#endif  // _CODE_H_
