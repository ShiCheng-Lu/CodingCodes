#include "rsa.hpp"

#include <iostream>

using namespace std;

int main() {
  RSA rsa{61, 53, 17};

  cout << rsa.public_key().first << ' ' << rsa.public_key().second << endl;

  cout << rsa.encode(65) << endl;

  cout << rsa.decode(2790) << endl;
}
