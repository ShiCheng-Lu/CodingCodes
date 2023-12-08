#include "rsa.hpp"

#include <iostream>

using namespace std;

int main() {
    RSA<BigInt> rsa("40094690950920881030683735292761468389214899724061",
                    "37975227936943673922808872755445627854565536638199", "3");

    cout << rsa.encode("0x46ad7ce88") << endl;

    cout << rsa.decode("0x56322c1f41c63bdd848030e200") << endl;
}
