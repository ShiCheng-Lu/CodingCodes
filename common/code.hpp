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

#endif  // _CODE_H_
