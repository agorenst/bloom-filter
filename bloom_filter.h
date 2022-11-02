#include <algorithm>
#include <bitset>
#include <forward_list>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "murmur/MurmurHash3.h"

class simple_bit_set {
private:
  static const int pageSize = 64;
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

size_t string_hash(const std::string &v, uint32_t i) {
  uint32_t hash;
  MurmurHash3_x86_32(v.c_str(), v.size(), i, &hash);
  return static_cast<size_t>(hash);
}

template <class V, size_t (*F)(const V &, uint32_t s)> class bloom_filter {
private:
  simple_bit_set data;

public:
  const size_t size;
  const size_t k;
  bloom_filter(const size_t size, const size_t k)
      : data(size), size(size), k(k) {}
  void set(const V &v) {
    for (size_t i = 0; i < k; i++) {
      data.set(F(v, i) % size);
    }
  }
  bool test(const V &v) const {
    for (size_t i = 0; i < k; i++) {
      if (!data.test(F(v, i) % size)) {
        return false;
      }
    }
    return true;
  }
  size_t count() const { return data.count(); }
  double density() const {
    return static_cast<double>(count()) / static_cast<double>(size);
  }
  std::string to_string() const { return data.to_string(); }
};