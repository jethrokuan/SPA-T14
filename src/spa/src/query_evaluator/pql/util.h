#pragma once
#include <iostream>
#include <map>
#include <string>
#include <variant>

namespace QE {
//! Utility templates for streaming nested variants
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

}  // namespace QE