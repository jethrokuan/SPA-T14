#pragma once
#include <map>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Utils {
struct pair_hash {
 public:
  inline std::size_t operator()(
      const std::pair<std::string, std::string>& v) const {
    std::hash<std::string> hash_str;
    size_t hash_val = 0;
    hash_val = hash_str(v.first) + 1;
    hash_val = hash_val ^ (hash_val >> 6);
    hash_val = hash_val * 31;
    hash_val = hash_val ^ (hash_val << 2);
    hash_val = hash_val + hash_str(v.second) + 1;
    hash_val = hash_val ^ (hash_val >> 7);
    hash_val = hash_val * 29;
    hash_val = hash_val ^ (hash_val << 1);

    return hash_val;
  }
};
void addToVectorMap(
    std::unordered_map<std::string, std::vector<std::string>> umap,
    std::string index, std::string data);

template <class T>
struct streamer {
  const T& val;
};
template <class T>
streamer(T)->streamer<T>;

template <class T>
std::ostream& operator<<(std::ostream& os, streamer<T> s) {
  os << s.val;
  return os;
}

template <class... Ts>
std::ostream& operator<<(std::ostream& os, streamer<std::variant<Ts...>> sv) {
  std::visit([&os](const auto& v) { os << streamer{v}; }, sv.val);
  return os;
}

// Generic template for swapping keys and value of a map into a new map
template <class T1, class T2>
std::map<T2, T1> swapPairs(std::map<T1, T2> m) {
  std::map<T2, T1> m1;
  for (auto&& item : m) {
    m1.emplace(item.second, item.first);
  }
  return m1;
};

bool has_only_digits(const std::string);

std::string& ltrim(std::string&, const std::string& chars = "\t ");
std::string& rtrim(std::string&, const std::string& chars = "\t ");
std::string& trim(std::string&, const std::string& chars = "\t ");
}  // namespace Utils
