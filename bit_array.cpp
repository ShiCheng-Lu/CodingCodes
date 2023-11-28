
#include <iostream>
#include <vector>

class BitArray {
  struct Reference {
    uint8_t* ptr;
    uint8_t idx;

    Reference(uint8_t* ptr, uint8_t idx) : ptr{ptr}, idx{idx} {}

    Reference& operator=(bool v) {
      if (v) {
        *ptr |= (1 << idx);
      } else {
        *ptr &= ~(1 << idx);
      }
      return *this;
    }

    bool operator^(bool v) {
      return (bool)(*this) ^ v;
    }

    operator bool() { return (*ptr & (1 << idx)) != 0; }

    friend std::ostream& operator<<(std::ostream& stream, Reference& bit) {
      stream << (bool)bit;
      return stream;
    }
  };

  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    Reference value;

    Iterator(uint8_t* ptr, uint8_t idx) : value{ptr, idx} {}

    Reference& operator*() { return value; }
    Reference* operator->() { return &value; }

    // Prefix increment
    Iterator& operator++() {
      value.idx++;
      if (value.idx == 8) {
        value.idx = 0;
        value.ptr++;
      }
      return *this;
    }

    // Postfix increment
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator& a, const Iterator& b) {
      return a.value.ptr == b.value.ptr && a.value.idx == b.value.idx;
    };
    friend bool operator!=(const Iterator& a, const Iterator& b) {
      return a.value.ptr != b.value.ptr || a.value.idx != b.value.idx;
    };
  };

  std::vector<uint8_t> data;
  size_t data_size;

 public:
  BitArray(std::vector<uint8_t> d) : data{d}, data_size{d.size() * 8} {}
  BitArray(size_t data_size) : data(data_size / 8 + 1), data_size{data_size} {}
  BitArray(std::string d) : data(d.size() / 8 + 1), data_size{d.size()} {
    for (size_t i = 0; i < data_size; ++i) {
      (*this)[i] = (d[i] == '1');
    }
  }

  Iterator begin() { return Iterator(&data[0], 0); }

  Iterator end() { return Iterator(&data[data_size / 8], data_size % 8); }

  size_t size() { return data_size; }

  Reference operator[](size_t idx) {
    return Reference{&data[idx / 8], (uint8_t)idx % 8};
  }
};

#include <bitset>

int main() {
  std::vector<uint8_t> d = {0b100101};
  BitArray a("0110011");

  //   std::cout << b << std::endl;
  //   std::cout << a.test() << std::endl;
  a[3] = ~a[3];

  std::cout << "array values" << std::endl;
  for (auto i : a) {
    std::cout << i << std::endl;
  }
}