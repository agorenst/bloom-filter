#include <algorithm>
#include <bitset>
#include <forward_list>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "datagen.h"
#include "murmur/MurmurHash3.h"

class simple_bit_set {
private:
  static const int pageSize = 32;
  typedef std::bitset<pageSize> page;
  std::vector<page> data;

public:
  simple_bit_set(const int size) : data((size / pageSize) + 1, 0) {}
  bool test(unsigned int i) const {
    return data[(i / pageSize)].test(i % pageSize);
  }
  void set(unsigned int i) { data[(i / pageSize)].set(i % pageSize); }
  int count() const {
    int sum = 0;
    for (auto &&p : data) {
      sum += p.count();
    }
    return sum;
  }
  int size() const { return data.size() * pageSize; }
  std::string to_string() const {
    std::string r;
    for (auto &&p : data) {
      r += p.to_string();
    }
    return r;
  }
};

template <class V> class hash_scheme {
  size_t k;

public:
  hash_scheme(size_t k) : k(k) {}
  std::vector<size_t> operator()(const V &v) const {
    std::vector<size_t> res(k);
    for (size_t i = 0; i < k; i++) {
      uint32_t hash;
      MurmurHash3_x86_32(v.c_str(), v.size(), i, &hash);
      res[i] = static_cast<size_t>(hash);
    }
    return res;
  }
};

// Takes a value, and a hashing function for it.
// The question remains about how to have multiple hash values...
template <class V> class bloom_filter {
public:
  simple_bit_set data;
  size_t size;
  hash_scheme<V> hash;

public:
  // also pass in some container of hash functions?
  bloom_filter(const size_t size, const size_t k)
      : data(size), size(size), hash(k) {}
  void set(const V &v) {
    auto hashes = hash(v);
    for (auto &&b : hashes) {
      data.set(b % size);
    }
  }
  bool test(const V &v) const {
    auto hashes = hash(v);
    for (auto &&b : hashes) {
      if (!data.test(b % size)) {
        return false;
      }
    }
    return true;
  }
  std::string to_string() const { return data.to_string(); }
  size_t count() const { return data.count(); }
  double density() const {
    return static_cast<double>(count()) / static_cast<double>(size);
  }
};

template <class V, size_t K> class bloom_filtered_set {
  std::unordered_set<V> core_data;

public:
  bloom_filter<V> filter;

  bloom_filtered_set(size_t size, size_t k) : filter(size, k) {}

  void add(V &v) {
    filter.set(v);
    core_data.insert(v);
  }
  bool contains(V &v) const {
    if (!filter.test(v)) {
      return false;
    }
    return core_data.find(v) != core_data.end();
  }
};

const double ln2 = 0.693147181;

void experiment1() {
  std::unordered_set<std::string> unique_strings;
  string_generator g;
  while (unique_strings.size() < 1100000) {
    unique_strings.insert(g(10));
  }
  std::vector<std::string> data(std::begin(unique_strings),
                                std::end(unique_strings));

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

  bloom_filter<std::string> bf(m, k);
  std::for_each(to_add_begin, to_add_end, [&bf](auto &v) { bf.set(v); });

  auto false_positives = std::count_if(to_test_begin, to_test_end,
                                       [&bf](auto &v) { return bf.test(v); });
  std::cout << "false positives = " << false_positives << std::endl;
  double fp_rate = double(false_positives) /
                   double(std::distance(to_test_begin, to_test_end));
  std::cout << "false positive rate = " << fp_rate << std::endl;
}
void experiment2() {
  std::unordered_set<std::string> unique_strings;
  string_generator g;
  while (unique_strings.size() < 1100000) {
    unique_strings.insert(g(10));
  }
  std::vector<std::string> data(std::begin(unique_strings),
                                std::end(unique_strings));

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
  const double eps = 0.2;
  const double best_bits = -1.44 * std::log2(eps);
  size_t m = n * best_bits;
  std::cout << "best bits = " << best_bits << std::endl;
  std::cout << "size      = " << m << std::endl;
  size_t k = size_t(-std::log2(eps)) + 1;
  std::cout << "k         = " << k << std::endl;

  // m = 9,999,991; // prime?
  // m = 10000000; // prime?
  bloom_filter<std::string> bf(m, k);
  std::for_each(to_add_begin, to_add_end, [&bf](auto &v) { bf.set(v); });

  auto false_positives = std::count_if(to_test_begin, to_test_end,
                                       [&bf](auto &v) { return bf.test(v); });
  std::cout << "false positives = " << false_positives << std::endl;
  double fp_rate = double(false_positives) /
                   double(std::distance(to_test_begin, to_test_end));
  std::cout << "false positive rate = " << fp_rate << std::endl;
  std::cout << "density = " << bf.density() << std::endl;
}

int main(int argc, char *argv[]) { experiment2(); }