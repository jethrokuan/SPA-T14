#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace Simple {
enum TokenType {
  NUMBER,
  SYMBOL,

  // Reserved keywords
  PROCEDURE,

  EQUAL,

  // Punctuation
  L_BRACE,
  R_BRACE,
  EOL,
  END_OF_FILE,
  SEMI
};

class Token {
 public:
  int row;
  TokenType t;
  Token(int r, TokenType token_t) {
    row = r;
    t = token_t;
  }
  virtual bool operator==(const Token &other) const {
    return other.t == this->t;
  }
};

class SymbolToken : public Token {
 public:
  std::string name;
  SymbolToken(int r, const std::string &n)
      : Token(r, TokenType::SYMBOL), name(n){};
  bool operator==(const Token &other) const {
    auto casted_other = dynamic_cast<const SymbolToken *>(&other);
    return casted_other != 0 && this->name.compare(casted_other->name) == 0;
  }
};

class NumberToken : public Token {
 public:
  std::string number;
  NumberToken(int r, const std::string &n)
      : Token(r, TokenType::NUMBER), number(n){};
  bool operator==(const Token &other) const {
    auto casted_other = dynamic_cast<const NumberToken *>(&other);
    return casted_other != 0 && this->number.compare(casted_other->number) == 0;
  }
};
}  // namespace Simple
