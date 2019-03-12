#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
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
std::unordered_map<T2, T1> swapPairs(std::unordered_map<T1, T2> m) {
  std::unordered_map<T2, T1> m1;
  for (auto&& item : m) {
    m1.emplace(item.second, item.first);
  }
  return m1;
};

// Generic template for finding a set intersection for unordered sets
template <class T>
std::unordered_set<T> unorderedSetIntersection(const std::unordered_set<T> m1,
                                               const std::unordered_set<T> m2) {
  // Keep invariant that first set is smaller than second
  if (m1.size() > m2.size()) return unorderedSetIntersection(m2, m1);
  std::unordered_set<T> m_out;
  for (auto&& item : m1) {
    if (m2.find(item) != m2.end()) {
      m_out.insert(item);
    }
  }
  return m_out;
};

// Generic template for finding a set intersection for unordered sets
template <class T, class H>
std::unordered_set<T, H> unorderedSetIntersection(
    const std::unordered_set<T, H> m1, const std::unordered_set<T, H> m2) {
  // Keep invariant that first set is smaller than second
  if (m1.size() > m2.size()) return unorderedSetIntersection(m2, m1);
  std::unordered_set<T, H> m_out;
  for (auto&& item : m1) {
    if (m2.find(item) != m2.end()) {
      m_out.insert(item);
    }
  }
  return m_out;
};

bool has_only_digits(const std::string);

std::string& ltrim(std::string&, const std::string& chars = "\t ");
std::string& rtrim(std::string&, const std::string& chars = "\t ");
std::string& trim(std::string&, const std::string& chars = "\t ");

// Utility templates for std::visit
template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
overload(Ts...)->overload<Ts...>;

std::vector<std::string> cartesianProduct(const std::vector<std::string> v1,
                                          const std::vector<std::string> v2,
                                          const char sep = ' ');

//! Finds the cartesian-product of multiple vectors of strings
std::vector<std::string> cartesianProduct(
    const std::vector<std::vector<std::string>> vecs, char sep = ' ');

}  // namespace Utils
