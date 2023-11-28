#ifndef _CODE_H_
#define _CODE_H_

#include <string>
#include <vector>

class CodeError {
 public:
  std::string message;

  CodeError(std::string message) : message{message} {}
};

class Code {
 public:
  template <typename T>
  T encode(T& data) {
    throw CodeError("encoding not implemented");
  }

  template <typename T>
  T decode(T& data) {
    throw CodeError("decoding not implemented");
  }
};

#endif  // _CODE_H_
