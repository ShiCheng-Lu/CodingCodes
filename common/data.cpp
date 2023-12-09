#include "data.hpp"

#include <iomanip>

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
