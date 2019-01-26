#pragma once

#include <fstream>
#include <vector>

#include "simple_parser/token.h"

namespace Simple {
class Lexer {
 public:
  std::vector<Token*> tokens;
  Lexer(std::istream& stream);
};
}  // namespace Simple
