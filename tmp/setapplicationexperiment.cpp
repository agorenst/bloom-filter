#include "bloomfilterset.h"
#include "datagen.h"
#include "murmur/MurmurHash3.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <chrono>
#include <numeric>
size_t string_hash(uint32_t i, const std::string &v) {
  uint32_t hash;
  MurmurHash3_x86_32(v.c_str(), v.size(), i, &hash);
  return static_cast<size_t>(hash);
}
void experiment() {
  const size_t N = 1'000'000;
  const auto strings = create_strings(N*2, 10);
  const double desired_eps = 0.01;
  const double best_bits = -1.44 * std::log2(desired_eps);
  size_t m = N * best_bits;
  size_t k = size_t(-std::log2(desired_eps)) + 1;
  
  auto to_add_begin = std::begin(strings);
  auto to_add_end = to_add_begin + N;
  auto to_test_begin = to_add_end;
  auto to_test_end = std::end(strings);

  std::set<std::string> base_set;
  bloom_filtered_set<std::string, string_hash> filtered_set(m, k);
  for (auto it = to_add_begin; it != to_add_end; it++) {
    base_set.insert(*it);
    filtered_set.add(*it);
  }
  {
    auto start = std::chrono::steady_clock::now();
    size_t count = 0;
    for (auto it = to_test_begin; it != to_test_end; it++) {
      if (base_set.find(*it) != base_set.end()) {
        count++;
      }
    }
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> diff_time = end - start;
    std::cout << "Time to validate base " << count << "\t" << diff_time.count() << "s\n";
  }
  {
    auto start = std::chrono::steady_clock::now();
    size_t count = 0;
    for (auto it = to_test_begin; it != to_test_end; it++) {
      if (filtered_set.contains(*it)) {
        count++;
      }
    }
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> diff_time = end - start;
    std::cout << "Time to validate diff " << count << "\t" << diff_time.count() << "s\n";
  }
}
int main() { experiment(); }
