#include <random>
#include <string>
#include <vector>
std::vector<std::string> generate_strings(int len, int count);

class string_generator {
  std::random_device rd;
  std::default_random_engine e1;
  std::uniform_int_distribution<int> uniform_dist;

public:
  string_generator();
  std::string operator()(size_t len);
};

std::vector<std::string> seed_strings(size_t to_add = 1000000,
                                      size_t to_not_add = 100000,
                                      size_t len = 10);
