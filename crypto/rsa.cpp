#include "rsa.hpp"

#include <iostream>

int main() {
    RSA rsa{61, 53, 17};

    std::cout << rsa.public_key().first << ' ' << rsa.public_key().second << std::endl;

    std::cout << rsa.encode(65) << std::endl;

    std::cout << rsa.decode(2790) << std::endl;
}
