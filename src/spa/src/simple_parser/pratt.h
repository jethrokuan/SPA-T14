/*!
 * \file   pratt.h
 * \brief  Pratt parser for arithmetic expressions.
 *
 * Specifies the Pratt parser used to parse an Expr.
 *
 * \date   2019-02-20
 */

#pragma once

#include <unordered_set>
#include <vector>
#include "simple_parser/token.h"
#include "structs/node.h"

namespace Simple {
//! Pratt parser for SIMPLE arithmetic expressions
class ExprParser {
 private:
  //! The current index in tokens. Takens in a reference, so
  //! the parser can be embedded in the recursive-descent parser
  int* current;

  //! The vector of lexical tokens. Takens in a reference, so
  //! the parser can be embedded in the recursive-descent parser
  const std::vector<Token*>& tokens;

  //! A list of delimiters that signify the end of the arithemtic expression.
  //! These tokens are not consumed by the parser.
  const std::unordered_set<std::string> delimiters;

  // Helper functions
  Token* advance();
  Token* peek();
  Token* previous();
  bool isAtEnd();
  bool check(std::string s);
  bool match(std::string s);
  bool expect(std::string s);

  // The core of the Pratt parser
  Expr nud(Token* t);
  int lbp(Token* t);
  Expr led(Token* t, Expr left);

 public:
  ExprParser(const std::vector<Token*>& _tokens, int* _current,
             const std::unordered_set<std::string> _delimiters);
  Expr parse(int rbp);
};
}  // namespace Simple
