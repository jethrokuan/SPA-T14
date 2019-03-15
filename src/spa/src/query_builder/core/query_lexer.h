#pragma once

#include <string>
#include <vector>

namespace QE {
class QueryLexer {
 private:
  unsigned int Curr;
  std::string Query;
  std::string Str;  // Represents the current string
  char peek();
  char advance();

  void readNumber();
  void readSymbol();
  bool isAtEnd();

 public:
  std::vector<std::string> Tokens;
  explicit QueryLexer(std::string query);
  void lex();
};
}  // namespace QE
