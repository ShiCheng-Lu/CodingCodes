#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "code.hpp"
#include "crypto/aes.hpp"
#include "crypto/rsa.hpp"
#include "error_correction/hamming_code.hpp"
#include "error_correction/reed_solomon.hpp"

using namespace std;

string help = "usage: [code] [encode|decode] message";

std::ostream& operator<<(std::ostream& out, std::vector<uint8_t>& data) {
  bool output_hex = out.flags() & out.hex;
  for (uint8_t byte : data) {
    if (output_hex) {
      out << std::hex << std::setfill('0') << std::setw(2) << (int)byte;
    } else {
      out << byte;
    }
  }
  return out;
}

std::vector<uint8_t> from_hex(std::string data) {
  std::vector<uint8_t> output;
  char str[3] = "00";
  for (int i = 0; i < data.size(); i += 2) {
    str[0] = data[i];
    str[1] = data[i + 1];
    output.push_back(strtol(str, NULL, 16));
  }
  return output;
}

int main(int argc, char* argv[]) {
  if (argc < 4) {
    cout << help << endl;
    return 0;
  }

  string code = argv[1], cmd = argv[2], message = argv[3];

  vector<uint8_t> message_v;

  if (cmd == "decode" || cmd == "-d") {
    message_v = from_hex(message);
  } else if (cmd == "encode" || cmd == "-e") {
    message_v = vector<uint8_t>(message.begin(), message.end());
  }

  Code* encoder;
  auto gf256 = GF256();

  if (code == "rsa") {
    encoder = new RSA(5, 7, 2);
  } else if (code == "aes") {
    encoder = new AES(gf256);
  } else if (code == "hamming") {
    encoder = new HammingCode();
  } else if (code == "rs" || code == "reedsolomon") {
    encoder = new ReedSolomon<uint8_t>(gf256, 11, 1, 255, 223);
  }
  std::vector<uint8_t> out;
  if (cmd == "decode" || cmd == "-d") {
    out = encoder->decode(message_v);
  } else if (cmd == "encode" || cmd == "-e") {
    out = encoder->encode(message_v);
    std::cout << std::hex;
  }
  std::cout << out << std::endl;

  return 0;
}
