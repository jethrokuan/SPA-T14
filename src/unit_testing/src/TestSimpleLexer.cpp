#include "simple_parser/lexer.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>

using namespace Simple;

TEST_CASE("Test Lex Assign Statement") {
  std::string filename = "tests/simple_source/assign.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(input);
  std::vector<Token*> expected{
      new Token(1, TokenType::PROCEDURE),   new SymbolToken(1, "main"),
      new Token(1, TokenType::L_BRACE),     new SymbolToken(2, "i"),
      new Token(2, TokenType::EQUAL),       new NumberToken(2, "5"),
      new Token(3, TokenType::SEMI),        new Token(3, TokenType::R_BRACE),
      new Token(4, TokenType::END_OF_FILE),
  };

  // Expect that the vector of tokens are equivalent
  REQUIRE(std::equal(
      begin(lexer.tokens), end(lexer.tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
}

TEST_CASE("Test Lex While Statement") {
  std::string filename = "tests/simple_source/while.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(input);
  std::vector<Token*> expected{
      new Token(1, TokenType::PROCEDURE),
      new SymbolToken(1, "main"),
      new Token(1, TokenType::L_BRACE),
      new Token(2, TokenType::WHILE),
      new Token(2, TokenType::L_PAREN),
      new SymbolToken(2, "i"),
      new Token(2, TokenType::EQUALEQUAL),
      new NumberToken(2, "5"),
      new Token(2, TokenType::R_PAREN),
      new Token(2, TokenType::L_BRACE),
      new SymbolToken(3, "i"),
      new Token(3, TokenType::EQUAL),
      new SymbolToken(3, "i"),
      new Token(3, TokenType::PLUS),
      new NumberToken(3, "1"),
      new Token(3, TokenType::SEMI),
      new Token(4, TokenType::R_BRACE),
      new Token(5, TokenType::R_BRACE),
      new Token(5, TokenType::END_OF_FILE),

  };

  // Expect that the vector of tokens are equivalent
  REQUIRE(std::equal(
      begin(lexer.tokens), end(lexer.tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
}
