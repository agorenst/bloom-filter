#include <cassert>

#include "bloom_filter.h"
#include "datagen.h"

// Given n, consider dimensions f = m/n and k

void empirical_epsilon() {
  // 1,000,000 strings to add to our set,
  // 100,000 strings explicitly not in our set,
  // all strings exactly length 10
  const size_t n = 1000000;
  const size_t test_size = 1000000;
  auto data = seed_strings(n, test_size, 10);

  auto to_add_begin = std::begin(data);
  auto to_add_end = to_add_begin + test_size;
  auto to_test_begin = to_add_end;
  auto to_test_end = std::end(data);

  for (size_t k = 1; k < 11; k++) {
    for (size_t f = 1; f < 11; f++) {
      auto m = n * f;
      bloom_filter<std::string> bf(m, k);
      std::for_each(to_add_begin, to_add_end, [&bf](auto &s) { bf.set(s); });
      // Validate:
      assert(std::all_of(to_add_begin, to_add_end,
                         [&bf](const auto &s) { return bf.test(s); }));
      auto fp = std::count_if(to_test_begin, to_test_end,
                              [&bf](const auto &s) { return bf.test(s); });
      auto fp_rate = static_cast<double>(fp) / static_cast<double>(test_size);
      std::cout << "k=" << k << "\tm=" << m << "\tfpr=%" << (fp_rate * 100.0)
                << "\td=%" << (bf.density() * 100.0) << std::endl;
    }
  }
}

int main() { empirical_epsilon(); }