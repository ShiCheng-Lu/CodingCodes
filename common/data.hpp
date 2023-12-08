#ifndef DATA_H_
#define DATA_H_

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& out,
                         const std::vector<std::uint8_t>& data) {
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

#endif  // DATA_H_
