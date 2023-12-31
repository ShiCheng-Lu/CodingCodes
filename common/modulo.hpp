#ifndef MODULO_H_
#define MODULO_H_

#include <iostream>
// modular arithemetic
template <typename T>
T modular_exp(T base, T exp, T modulo) {
    T result = 1;
    while (exp != 0) {
        if ((exp & 1) != 0) {
            result = (result * base) % modulo;
        }
        base = (base * base) % modulo;
        exp = exp >> 1;
    }
    return result;
}

// find gcd with Euclidean algorithm
template <typename T>
T gcd(T a, T b) {
    while (a != 0 && b != 0) {
        if (a > b) {
            a = a % b;
        } else {
            b = b % a;
        }
    }
    return a + b;
}

// find lcm with gcd
template <typename T>
T lcm(T a, T b) {
    return (a * b) / gcd(a, b);
}

#endif  // MODULO_H_
