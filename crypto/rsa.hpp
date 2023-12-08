#ifndef RSA_H_
#define RSA_H_

#include <tuple>

#include "common/big_int.hpp"
#include "common/code.hpp"
#include "common/modulo.hpp"

/**
 * @brief https://en.wikipedia.org/wiki/RSA_(cryptosystem)
 *
 */
template <typename T>
class RSA : public Code {
    T n;  // modulus
    T e;  // public key
    T d;  // private key

   public:
    // Create and RSA encoder and decoder, p, q, must be prime
    RSA(T p, T q, T e) : e{e} {
        n = p * q;
        // std::cout << "N: " << n << std::endl;
        T l = lcm(p - 1, q - 1);
        // std::cout << "L: " << l << std::endl;

        // find private key with extended Euclidean algorithm
        T a = e;
        T b = l;
        T s_0 = 1, s_1 = 0;
        T mul;

        int i = 0;
        while (a != 0 && b != 0) {
            if (a > b) {
                std::tie(mul, a) = divide(a, b);
                s_0 = s_0 - s_1 * mul;
            } else {
                std::tie(mul, b) = divide(b, a);
                s_1 = s_1 - s_0 * mul;
            }
            // std::cout << s_0 << " " << s_1 << ' ' << a << ' ' << b <<
            // std::endl;
        }
        d = s_0 + s_1 * b;
        // std::cout << "D: " << d << std::endl;
        // public key: (n, e)
        // private key (n, d)
    }

    std::pair<T, T> public_key() { return std::pair{n, e}; }

    std::pair<T, T> private_key() { return std::pair{n, d}; }

    T encode(T data) { return modular_exp(data, e, n); }
    std::vector<std::uint8_t> encode(std::vector<std::uint8_t>& data) {
        return modular_exp(T(data), e, n).vector();
    }

    T decode(T data) { return modular_exp(data, d, n); }
    std::vector<std::uint8_t> decode(std::vector<std::uint8_t>& data) {
        return modular_exp(T(data), d, n).vector();
    }
};

#endif  // RSA_H_
