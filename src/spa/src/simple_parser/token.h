/*!
  \file   token.h
  \brief  Lexical tokens for the SIMPLE language.

  \date   2019-02-05
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace Simple {

enum class TokenType { NUMBER, SYMBOL, PUNCT, END_OF_FILE };

//! Abstract class representing lexical tokens in the SIMPLE language
class Token {
 public:
  TokenType T;
  std::string Val;
  Token(TokenType t, std::string val);
  virtual bool operator==(const Token& other) const;
};

//! Represents a symbol in the SIMPLE language
/*!
  These symbols represent variables, and functional keywords in SIMPLE
  such as while, if, call, and read.
*/
class SymbolToken : public Token {
 public:
  explicit SymbolToken(std::string val);
};

//! Represents a number in the SIMPLE language
/*!
  Numbers in the SIMPLE language are limited to integers. They are stored in
  string format.
*/
class NumberToken : public Token {
 public:
  explicit NumberToken(std::string val);
};

//! Represents a punctuation in the SIMPLE language
/*!
  These punctuations include relational and arithmetic operations, such as
  != and +.
*/
class PunctToken : public Token {
 public:
  explicit PunctToken(std::string val);
};

//! Represents the end of file in the SIMPLE language
/*!
  Provides a means for the parser to determine the end of the SIMPLE
  program.
*/
class EndOfFileToken : public Token {
 public:
  explicit EndOfFileToken();
};
}  // namespace Simple
