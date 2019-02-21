/*!
  \file   exceptions.h
  \brief  Exceptions concerning the SIMPLE language.

  \date   2019-02-05
*/

#pragma once

#include <stdexcept>
#include <string>

namespace Simple {

//! Exception thrown when SIMPLE Lexer fails to lex.
class SimpleLexException : public std::runtime_error {
 private:
  std::string msg;

 public:
  explicit SimpleLexException(int lineno, int colno, char expectedChar,
                              char obtainedChar)
      : std::runtime_error(std::to_string(lineno) + ":" +
                           std::to_string(colno) + " Expected '" +
                           expectedChar + "', got '" + obtainedChar + "'"){};
  explicit SimpleLexException(int lineno, int colno, char obtainedChar)
      : std::runtime_error(std::to_string(lineno) + ":" +
                           std::to_string(colno) + " Unexpected token '" +
                           obtainedChar + "'"){};
};
class SimpleParseException : public std::runtime_error {
 public:
  explicit SimpleParseException(const std::string& msg_)
      : std::runtime_error(msg_){};
};
}  // namespace Simple
