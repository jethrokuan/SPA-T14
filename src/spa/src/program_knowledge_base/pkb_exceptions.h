#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

namespace PKB {

//! Represents an error that occurs during parsing of PQL tokens into a Query
class PkbAstSemanticException : public std::runtime_error {
 public:
  PkbAstSemanticException(const std::string& msg = "")
      : std::runtime_error(msg) {}
};

}  // namespace PKB
