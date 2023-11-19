#include <stdint.h>
#include <stdio.h>

template <typename T>
class A {
 public:
  virtual void f(T a) = 0;
};

template <typename T>
class B : public A<T> {
 public:
  B() {}
  void f(T a) { printf("%d\n", a); }
};

template <typename T>
void f(A<T> a) {
  a.f(6);
}

int main(void) {
  B<uint8_t> b;

  f(b);
}
