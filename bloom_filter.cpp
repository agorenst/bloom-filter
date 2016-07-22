#include <vector>
#include <bitset>
#include <forward_list>
#include <iostream>
using namespace std;

class simple_bit_set {
    private:
    static const int pageSize = 32;
    typedef std::bitset<pageSize> page;
    std::vector<page> data;
    public:
    simple_bit_set(const int size): 
        data((size/pageSize)+1, 0) {}
    bool test(unsigned int i) const {
        return data[(i / pageSize)].test(i % pageSize);
    }
    void set(unsigned int i) {
        data[(i / pageSize)].set(i % pageSize);
    }

    // The following are diagnostics stuff
    void clear() { data.clear(); }
    int count() const {
        int sum = 0;
        for (auto&& p : data) {
            sum += p.count();
        }
        return sum;
    }
    int size() const {
        return data.size()*pageSize;
    }
};

// Takes a value, and a hashing function for it.
// The question remains about how to have multiple hash values...
template<class V, class H>
class bloom_filter {
    private:
    simple_bit_set data;
    std::forward_list<H> hashes;
    public:
    // also pass in some container of hash functions?
    bloom_filter(const int size, const std::forward_list<H> hashes):
        data(size), hashes(hashes) {}
    void set(const V& v) {
        for (auto&& h : hashes) {
            data.set(h(v));
        }
    }
    bool test(const V& v) const {
        for (auto&& h : hashes) {
            if (!data.test(h(v))) {
                return false;
            }
        }
        return true;
    }
};

using namespace std;

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <utility>
#include <functional>

using namespace std;
int ipow(int x, int y) {
    auto res = x;
    for (int i = 0; i < y; ++i) {
        res *= x;
    }
    return res;
}

// This basic one will guard a map over a set of strings.
int main(int argc, char* argv[]) {
    //int size = 1024*32;
    int size = 31511; // nearby prime.
    auto hash1 = [size](const std::string& s) {
        return ((unsigned int) ipow(std::hash<std::string>{}(s) % size, 3)) % size;
    };
    auto hash2 = [size](const std::string& s) {
        return ((unsigned int) ipow(std::hash<std::string>{}(s) % size, 5)) % size;
    };
    auto hash3 = [size](const std::string& s) {
        return ((unsigned int) ipow(std::hash<std::string>{}(s) % size, 7)) % size;
    };
    auto hash4 = [size](const std::string& s) {
        return ((unsigned int) ipow(std::hash<std::string>{}(s) % size, 11)) % size;
    };
    std::forward_list<function<unsigned int(const std::string&)>> hashes;
    hashes.push_front(hash1);
    hashes.push_front(hash2);
    hashes.push_front(hash3);
    hashes.push_front(hash4);
    bloom_filter<std::string, function<unsigned int(const std::string&)>> bf(size, hashes);
    bf.set("hello");
    bf.set("goodbye");
    bf.set("how are you");
    for (auto&& s : {"hello", "goodbye", "how are you", "what the who now what"}) {
        cout << s << " " << bf.test(s) << endl;
    }
}
