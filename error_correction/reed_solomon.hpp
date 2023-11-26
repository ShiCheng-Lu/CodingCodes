#ifndef REED_SOLOMON_HPP
#define REED_SOLOMON_HPP

#include "gf256.hpp"

template <typename T>
class ReedSolomon : public Code<T> {
 protected:
  Field<T>& f;                           // field of operation
  size_t n;                              // code word length
  size_t k;                              // message length
  std::vector<T> divisor;                // polynomial divisor
  std::vector<std::vector<T>> verifier;  // sequential polynomial roots

 public:
  /**
   * @brief Construct a new Reed Solomon object
   *
   * @param f field of operation
   * @param prim primitive element of the field
   * @param root the first consecutive roots of the generator polynomial
   * @param n code word length
   * @param k message length
   */
  ReedSolomon(Field<T>& f, T prim, T root, size_t n, size_t k)
      : f{f}, n{n}, k{k} {
    divisor.resize(n - k + 1);
    divisor[0] = 1;
    for (size_t root_num = 1; root_num < n - k + 1; ++root_num) {
      for (size_t i = root_num; i > 0; --i) {
        divisor[i] = f.add(f.mul(divisor[i - 1], root), divisor[i]);
      }

      // for decoding
      std::vector<T> poly(n);
      poly[0] = 1;
      for (size_t i = 1; i < n; ++i) {
        poly[i] = f.mul(poly[i - 1], root);
      }
      verifier.push_back(poly);

      // next root
      root = f.mul(root, prim);
    }
  }

  /**
   * @brief perform Reed-Solomon encoding on the data
   *
   * @param data input data
   * @return vector<T> encoded data
   */
  std::vector<T> encode(std::vector<T>& data) {
    // s(x) = q(x)g(x) + r(x)
    // polynomial long division to find the remainder
    std::vector<T> remainder(n - k);
    for (size_t i = 0; i < k; ++i) {
      T mul = f.add(data[i], remainder[0]);

      for (size_t x = 0; x < n - k - 1; ++x) {
        remainder[x] = f.sub(remainder[x + 1], f.mul(divisor[x + 1], mul));
      }
      remainder[n - k - 1] = f.sub(0, f.mul(divisor[n - k], mul));
    }
    for (size_t x = 0; x < n - k; ++x) {
      remainder[x] = f.sub(0, remainder[x]);
    }

    std::vector<T> output(data);
    output.insert(output.end(), remainder.begin(), remainder.end());
    return output;
  }

  /**
   * @brief perform Reed-Solomon decoding on the data
   *
   * @param data encoded input data
   * @return vector<T> decoded data
   */
  std::vector<T> decode(std::vector<T>& data) {
    // validate data
    bool valid = true;
    for (vector<T> poly : verifier) {
      T value = 0;
      for (size_t i = 0; i < n; ++i) {
        value = f.add(value, f.mul(poly[i], data[data.size() - 1 - i]));
      }
      if (value != 0) {
        valid = false;
        break;
      }
    }

    if (valid) {
      vector<T> output(data);
      return output;
    }

    // input data invalid
    return vector<T>();
  }
};

#endif  // REED_SOLOMON_HPP
