#pragma once

#include <stdexcept>
#include <string>

namespace Simple {
class SimpleLexException : public std::runtime_error {
 private:
  std::string msg;

 public:
  SimpleLexException(const std::string& msg_ = "") : std::runtime_error(msg_) {}
  SimpleLexException(int lineno, int colno, char expectedChar,
                     char obtainedChar)
      : std::runtime_error(std::to_string(lineno) + ":" +
                           std::to_string(colno) + " Expected '" +
                           expectedChar + "', got '" + obtainedChar + "'"){};
  SimpleLexException(int lineno, int colno, char obtainedChar)
      : std::runtime_error(std::to_string(lineno) + ":" +
                           std::to_string(colno) + " Unexpected token '" +
                           obtainedChar + "'"){};
};
}  // namespace Simple
