#pragma once
#include <algorithm>
#include <cctype>
#include <locale>
#include <iostream>
#include <iterator>
#include <sstream>
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

// Generic template for finding a set intersection for unordered sets
template <class T>
std::unordered_set<std::pair<T, T>, pair_hash>
unorderedSetIntersectionToPairedSet(const std::unordered_set<T> m1,
                                    const std::unordered_set<T> m2) {
  // Keep invariant that first set is smaller than second
  if (m1.size() > m2.size()) return unorderedSetIntersectionToPairedSet(m2, m1);
  std::unordered_set<std::pair<T, T>, pair_hash> m_out;
  for (auto&& item : m1) {
    if (m2.find(item) != m2.end()) {
      m_out.insert({item, item});
    }
  }
  return m_out;
};

enum class MapFromPairSetting { MAP_LEFT_TO_RIGHT, MAP_RIGHT_TO_LEFT };
// Transform a set of pairs into a map of one side to the other
// Can have a vector as the result since may have duplicate keys
// (this was a paired hash)
template <class T>
std::unordered_map<T, std::vector<T>> getMapFromPairSet(
    const std::unordered_set<std::pair<T, T>, pair_hash>& in_set,
    MapFromPairSetting setting) {
  std::unordered_map<T, std::vector<T>> out_map;
  for (const auto& [left, right] : in_set) {
    std::string key, value;
    if (setting == MapFromPairSetting::MAP_LEFT_TO_RIGHT) {
      key = left;
      value = right;
    } else {
      key = right;
      value = left;
    }

    // Either create new vector for this key, or add to it
    if (out_map.find(key) == out_map.end()) {
      out_map.insert({key, {value}});
    } else {
      out_map[key].push_back(value);
    }
  }

  return out_map;
};

bool has_only_digits(const std::string);
bool is_valid_name(const std::string);

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

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

template <typename InputIt>
std::string join(InputIt first, InputIt last,
                 const std::string& separator = " ",
                 const std::string& concluder = "") {
  if (first == last) {
    return concluder;
  }

  std::stringstream ss;
  ss << *first;
  ++first;

  while (first != last) {
    ss << separator;
    ss << *first;
    ++first;
  }

  ss << concluder;

  return ss.str();
}

}  // namespace Utils
