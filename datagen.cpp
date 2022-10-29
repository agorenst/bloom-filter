#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "datagen.h"

const static std::string letterSet =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
string_generator::string_generator()
    : e1(rd()), uniform_dist(0, letterSet.size()) {}

std::string string_generator::operator()(size_t len) {
  std::string next;
  next.reserve(len);
  for (size_t c = 0; c < len; c++) {
    next += letterSet[uniform_dist(e1)];
  }
  return next;
}

std::vector<std::string> generate_strings(int len, int count) {
  string_generator gen;
  std::vector<std::string> res;
  res.reserve(count);

  for (int i = 0; i < count; i++) {
    res.push_back(gen(len));
  }

  return res;
}

#ifdef _DATAGEN_STANDALONE_
int main() {
  auto strs = generate_strings(4, 4);
  for (const auto &s : strs) {
    std::cout << s << std::endl;
  }
}
#endif