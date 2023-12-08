#ifndef BIT_ARRAY_H_
#define BIT_ARRAY_H_

#include <cstdint>
#include <iostream>
#include <vector>

/**
 * @brief Bit array backed by a vector<std::uint8_t>
 *
 * implements indexing and iterator
 *
 */
class BitArray {
    struct Reference {
        std::uint8_t* ptr;
        std::uint8_t idx;

        Reference(std::uint8_t* ptr, std::uint8_t idx) : ptr{ptr}, idx{idx} {}

        Reference& operator=(bool value) {
            if (value) {
                *ptr |= (1 << idx);
            } else {
                *ptr &= ~(1 << idx);
            }
            return *this;
        }

        operator bool() { return (*ptr & (1 << idx)) != 0; }

        friend std::ostream& operator<<(std::ostream& stream, Reference& bit) {
            stream << (bool)bit;
            return stream;
        }
    };

    size_t data_size;

   public:
    std::vector<std::uint8_t> data;
    BitArray(std::vector<std::uint8_t> d) : data{d}, data_size{d.size() * 8} {}
    BitArray(size_t data_size)
        : data(data_size / 8 + 1), data_size{data_size} {}
    BitArray(std::string d) : data(d.size() / 8 + 1), data_size{d.size()} {
        for (size_t i = 0; i < data_size; ++i) {
            (*this)[i] = (d[i] == '1');
        }
    }

    size_t size() { return data_size; }

    Reference operator[](size_t idx) {
        return Reference{&data[idx / 8], (std::uint8_t)(idx % 8)};
    }
};

#endif  // BIT_ARRAY_H_
