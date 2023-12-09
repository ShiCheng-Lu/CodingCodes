#ifndef DATA_H_
#define DATA_H_

#include <cstdint>
#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& out,
                         const std::vector<std::uint8_t>& data);

#endif  // DATA_H_
