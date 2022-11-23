#include "bloomfilter.h"
#include "datagen.h"
#include "murmur/MurmurHash3.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <numeric>
size_t string_hash(uint32_t i, const std::string &v) {
  uint32_t hash;
  MurmurHash3_x86_32(v.c_str(), v.size(), i, &hash);
  return static_cast<size_t>(hash);
}
void experiment() {
    const size_t N = 1'000'000;
    const auto strings = create_strings(N, 10);
    const double desired_eps = 0.01;
    const double best_bits = -1.44 * std::log2(desired_eps);
    size_t m = N * best_bits;
    size_t k = size_t(-std::log2(desired_eps)) + 1;
    bloom_filter<std::string, string_hash> bf(m, k);
    std::for_each(std::begin(strings), std::end(strings),
                  [&bf](auto &s) { bf.set(s); });
  const double density =
      static_cast<double>(bf.count()) / static_cast<double>(bf.n);
  printf("density: %f\n", density);
}
int main() { experiment(); }
