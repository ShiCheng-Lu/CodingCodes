#ifndef _CODE_H_
#define _CODE_H_

#include <vector>

template <typename T>
class Code {
 public:
  virtual std::vector<T> encode(std::vector<T>& data) { return data; }
  virtual std::vector<T> decode(std::vector<T>& data) { return data; }
};

#endif  // _CODE_H_
