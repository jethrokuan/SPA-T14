#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

namespace QE {

class PQLParseException : public std::runtime_error {
 public:
  PQLParseException(const std::string& msg = "") : std::runtime_error(msg) {}
};

class PQLTokenizeException : public std::runtime_error {
 public:
  PQLTokenizeException(const std::string& msg = "") : std::runtime_error(msg) {}
};

}  // namespace QE
