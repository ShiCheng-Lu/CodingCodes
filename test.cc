#include <stdint.h>
#include <stdio.h>

template <typename T>
class A {
 public:
  virtual void f(T a) {}
};

class B : public A<uint8_t> {
 public:
  B() {}
  void f(uint8_t a) { printf("%d\n", a); }
};

void f(A<uint8_t>& a) {
  a.f(6);
}

int main(void) {
  B b;

  f(b);
}
