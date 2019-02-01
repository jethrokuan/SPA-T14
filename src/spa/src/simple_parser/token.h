#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace Simple {
enum class TokenType { NUMBER, SYMBOL, PUNCT, END_OF_FILE };

class Token {
 public:
  TokenType T;
  std::string Val;
  Token(TokenType t, std::string val);
  virtual bool operator==(const Token& other) const;
};

class SymbolToken : public Token {
 public:
  SymbolToken(std::string val);
};

class NumberToken : public Token {
 public:
  NumberToken(std::string val);
};

class PunctToken : public Token {
 public:
  PunctToken(std::string val);
};

class EndOfFileToken : public Token {
 public:
  EndOfFileToken();
};
}  // namespace Simple
