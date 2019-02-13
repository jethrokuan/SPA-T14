#include "utils/utils.h"

namespace Utils {

bool has_only_digits(const std::string s) {
  return s.find_first_not_of("0123456789") == std::string::npos;
}

std::string& ltrim(std::string& str, const std::string& chars) {
  str.erase(0, str.find_first_not_of(chars));
  return str;
}

std::string& rtrim(std::string& str, const std::string& chars) {
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}

std::string& trim(std::string& str, const std::string& chars) {
  return ltrim(rtrim(str, chars), chars);
}

}  // namespace Utils
