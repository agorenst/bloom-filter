#include "datagen.h"

#include <unordered_set>
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
std::vector<std::string> create_strings(size_t count, size_t len) {
  std::unordered_set<std::string> unique_strings;
  string_generator g;
  while (unique_strings.size() < count) {
    unique_strings.insert(g(len));
  }
  std::vector<std::string> data(std::begin(unique_strings),
                                std::end(unique_strings));
  return data;
}
