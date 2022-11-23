#include <random>
#include <string>
#include <vector>
class string_generator {
  std::random_device rd;
  std::default_random_engine e1;
  std::uniform_int_distribution<int> uniform_dist;
public:
  string_generator();
  std::string operator()(size_t len);
};
std::vector<std::string> create_strings(size_t count, size_t len);
