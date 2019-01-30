#include "simple_parser/lexer.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>

using namespace Simple;
using Simple::TokenType;

TEST_CASE ("Test Lex Tokens") {
  std::string filename = "tests/simple_source/tokens.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(input);
  std::vector<Token*> expected{
      new Token(1, "PROCEDURE"),   new SymbolToken(1, "main"),
      new NumberToken(1, "5"),     new NumberToken(1, "50"),
      new Token(1, "L_BRACE"),     new Token(1, "R_BRACE"),
      new Token(1, "L_PAREN"),     new Token(1, "R_PAREN"),
      new Token(1, "EQUAL"),       new Token(1, "BANG_EQUAL"),
      new Token(1, "BANG"),        new Token(1, "AND"),
      new Token(1, "OR"),          new Token(1, "READ"),
      new Token(1, "PRINT"),       new Token(1, "PLUS"),
      new Token(1, "MINUS"),       new Token(1, "TIMES"),
      new Token(1, "DIVIDE"),      new Token(1, "MOD"),
      new Token(1, "GREATER"),     new Token(1, "GREATER_EQUAL"),
      new Token(1, "LESS"),        new Token(1, "LESS_EQUAL"),
      new Token(1, "END_OF_FILE"),
  };

  REQUIRE(std::equal(
      begin(lexer.tokens), end(lexer.tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
};

TEST_CASE ("Test Lex Assign Statement") {
  std::string filename = "tests/simple_source/assign.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(input);
  std::vector<Token*> expected{
      new Token(1, "PROCEDURE"),   new SymbolToken(1, "main"),
      new Token(1, "L_BRACE"),     new SymbolToken(2, "i"),
      new Token(2, "EQUAL"),       new NumberToken(2, "5"),
      new Token(3, "SEMI"),        new Token(3, "R_BRACE"),
      new Token(4, "END_OF_FILE"),
  };

  // Expect that the vector of tokens are equivalent
  REQUIRE(std::equal(
      begin(lexer.tokens), end(lexer.tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
}

TEST_CASE ("Test Lex While Statement") {
  std::string filename = "tests/simple_source/while.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(input);
  std::vector<Token*> expected{
      new Token(1, "PROCEDURE"),   new SymbolToken(1, "main"),
      new Token(1, "L_BRACE"),     new Token(2, "WHILE"),
      new Token(2, "L_PAREN"),     new SymbolToken(2, "i"),
      new Token(2, "EQUAL_EQUAL"), new NumberToken(2, "5"),
      new Token(2, "R_PAREN"),     new Token(2, "L_BRACE"),
      new SymbolToken(3, "i"),     new Token(3, "EQUAL"),
      new SymbolToken(3, "i"),     new Token(3, "PLUS"),
      new NumberToken(3, "1"),     new Token(3, "SEMI"),
      new Token(4, "R_BRACE"),     new Token(5, "R_BRACE"),
      new Token(5, "END_OF_FILE"),

  };

  // Expect that the vector of tokens are equivalent
  REQUIRE(std::equal(
      begin(lexer.tokens), end(lexer.tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
}
