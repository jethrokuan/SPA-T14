#pragma once
#include <better_enums/enums.h>

#include <iostream>
#include <memory>
#include <string>

namespace Simple {
BETTER_ENUM(_TokenType, uint8_t, NUMBER, SYMBOL,

            // Reserved keywords
            PROCEDURE,

            // Conditionals
            IF, WHILE, THEN, ELSE,

            EQUAL, BANG,

            // Relations
            GREATER, GREATER_EQUAL, LESS, LESS_EQUAL, EQUAL_EQUAL, BANG_EQUAL,

            // Cond
            AND, OR,

            // Ops
            PLUS, MINUS, TIMES, DIVIDE, MOD,

            // Punctuation
            L_BRACE, R_BRACE, L_PAREN, R_PAREN, END_OF_FILE, SEMI, READ, PRINT,

            UNSET);

using TokenType = _TokenType;
class Token {
 public:
  int row;
  TokenType t = +TokenType::UNSET;

  Token(int r, const char* token) {
    row = r;
    t = TokenType::_from_string(token);
  }
  virtual bool operator==(const Token& other) const {
    return other.t == this->t;
  }
};

class SymbolToken : public Token {
 public:
  std::string name;
  SymbolToken(int r, const std::string& n) : Token(r, "SYMBOL"), name(n){};
  bool operator==(const Token& other) const {
    auto casted_other = dynamic_cast<const SymbolToken*>(&other);
    return casted_other != 0 && this->name.compare(casted_other->name) == 0;
  }
};

class NumberToken : public Token {
 public:
  std::string number;
  NumberToken(int r, const std::string& n) : Token(r, "NUMBER"), number(n){};
  bool operator==(const Token& other) const {
    auto casted_other = dynamic_cast<const NumberToken*>(&other);
    return casted_other != 0 && this->number.compare(casted_other->number) == 0;
  }
};
}  // namespace Simple
