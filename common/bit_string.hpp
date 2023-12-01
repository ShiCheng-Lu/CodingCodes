#ifndef BIT_STRING_H_
#define BIT_STRING_H_

#include <string>

class BitString {
  std::string data;

  struct Reference {
    char& c;

    Reference(char& c) : c{c} {}

    Reference& operator=(bool value) {
      c = value ? '1' : '0';
      return *this;
    }

    operator bool() { return c == '1'; }
  };

 public:
  BitString(std::size_t len) : data(len, '0') {}
  BitString(std::string str) : data(str) {}

  std::size_t size() { return data.size(); }

  Reference operator[](std::size_t idx) { return Reference(data[idx]); }
};

#endif  // BIT_STRING_H_