#include "bit_array.hpp"
#include <bitset>

using namespace std;

int main() {
  vector<uint8_t> d = {0b100101};
  BitArray a("0110011");

  //   std::cout << b << std::endl;
  //   std::cout << a.test() << std::endl;
  a[3] = ~a[3];

  cout << "array values" << endl;
  for (auto i : a) {
    cout << i << endl;
  }
}