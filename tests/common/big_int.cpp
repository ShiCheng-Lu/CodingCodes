#include "big_int.hpp"

// evil macro to test the operators
#define TEST_OPERATOR(op)                                              \
    for (int i = 0; i < 100000; ++i) {                                 \
        int a = rand() % 213513279;                                    \
        int b = rand() % 127380 + 1;                                   \
                                                                       \
        BigInt big_a(a), big_b(b);                                     \
                                                                       \
        if ((big_a op big_b) != (a op b)) {                            \
            std::cout << "WRONG RESULT FOR: " << a << " " #op " " << b \
                      << std::endl;                                    \
        }                                                              \
    }

int main() {
    //   TEST_OPERATOR(+);
    //   TEST_OPERATOR(-);
    //   TEST_OPERATOR(*);
    //   TEST_OPERATOR(/);
    //   TEST_OPERATOR(%);
    std::cout << (BigInt(0x100) >> 8) << std::endl;
    std::cout << (BigInt(10) >> 0) << std::endl;
}