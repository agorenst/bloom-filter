
#include <chrono>
#include <set>

#include "bloom_filter.h"
#include "datagen.h"

template <class V, size_t (*F)(const V &, uint32_t)> class bloom_filtered_set {
  std::set<V> core_data;

public:
  bloom_filter<V, F> filter;
  mutable size_t filter_hit = 0;

  bloom_filtered_set(size_t size, size_t k) : filter(size, k) {}

  void add(V &v) {
    filter.set(v);
    core_data.insert(v);
  }
  int contains(V &v) const {
    if (!filter.test(v)) {
      filter_hit++;
      return false;
    }
    return core_data.find(v) != core_data.end();
  }
};

const double ln2 = 0.693147181;
double best_bits(double eps) { return -1.44 * std::log2(eps); }

void experiment1() {
  auto data = seed_strings();

  // m = number of bits (bloom filter size)
  // n = number of elements (in this case, 1,000,000)
  // k = number of hashes
  // from wiki, optimal k = (m/n)ln2.
  // so optimal m = (k*n)/ln2
  auto to_add_begin = std::begin(data);
  auto to_add_end = to_add_begin + 1000000;
  auto to_test_begin = to_add_end;
  auto to_test_end = std::end(data);

  const auto n = std::distance(to_add_begin, to_add_end);
  const auto k = 7;
  std::cout << "n = " << n << std::endl;
  std::cout << "k = " << k << std::endl;
  std::cout << "m = " << (k * n) / ln2 << std::endl;
  size_t m = size_t((k * n) / ln2);
  std::cout << "m = " << m << std::endl;

  bloom_filter<std::string, string_hash> bf(m, k);
  std::for_each(to_add_begin, to_add_end, [&bf](auto &v) { bf.set(v); });

  auto false_positives = std::count_if(to_test_begin, to_test_end,
                                       [&bf](auto &v) { return bf.test(v); });
  std::cout << "false positives = " << false_positives << std::endl;
  double fp_rate = double(false_positives) /
                   double(std::distance(to_test_begin, to_test_end));
  std::cout << "false positive rate = " << fp_rate << std::endl;
}
void experiment2() {
  auto data = seed_strings();

  // m = number of bits (bloom filter size)
  // n = number of elements (in this case, 1,000,000)
  // k = number of hashes
  // from wiki, optimal k = (m/n)ln2.
  // so optimal m = (k*n)/ln2
  auto to_add_begin = std::begin(data);
  auto to_add_end = to_add_begin + 1000000;
  auto to_test_begin = to_add_end;
  auto to_test_end = std::end(data);

  const auto n = std::distance(to_add_begin, to_add_end);
  const double eps = 0.01;
  const double bb = best_bits(eps);
  size_t m = n * bb;
  std::cout << "best bits = " << bb << std::endl;
  std::cout << "size      = " << m << std::endl;
  size_t k = size_t(-std::log2(eps)) + 1;
  std::cout << "k         = " << k << std::endl;

  bloom_filter<std::string, string_hash> bf(m, k);
  std::for_each(to_add_begin, to_add_end, [&bf](auto &v) { bf.set(v); });

  auto false_positives = std::count_if(to_test_begin, to_test_end,
                                       [&bf](auto &v) { return bf.test(v); });
  std::cout << "false positives = " << false_positives << std::endl;
  double fp_rate = double(false_positives) /
                   double(std::distance(to_test_begin, to_test_end));
  std::cout << "false positive rate = " << fp_rate << std::endl;
  std::cout << "density = " << bf.density() << std::endl;

  std::set<std::string> base;
  bloom_filtered_set<std::string, string_hash> diff(m, k);
  for (auto it = to_add_begin; it != to_add_end; it++) {
    base.insert(*it);
    diff.add(*it);
  }

  const auto iterCount = 20;
  {
    auto start = std::chrono::steady_clock::now();
    size_t count = 0;
    for (size_t i = 0; i < iterCount; i++) {
      for (auto it = to_test_begin; it != to_test_end; it++) {
        if (base.find(*it) != base.end()) {
          count++;
        }
      }
    }
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> diffTime = end - start;
    std::cout << "Time to validate base " << count << "\t" << diffTime.count()
              << " s\n";
  }
  {
    auto start = std::chrono::steady_clock::now();
    size_t count = 0;
    for (size_t i = 0; i < iterCount; i++) {
      for (auto it = to_test_begin; it != to_test_end; it++) {
        if (diff.contains(*it)) {
          count++;
        }
      }
    }
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> diffTime = end - start;
    std::cout << "Time to validate diff " << count << "\t" << diffTime.count()
              << "\t"
              << (static_cast<double>(diff.filter_hit) /
                  static_cast<double>(
                      iterCount * std::distance(to_test_begin, to_test_end)))
              << " s\n";
  }
}

int main(int argc, char *argv[]) { experiment2(); }