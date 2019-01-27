#include "simple_parser/lexer.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>

using namespace Simple;

TEST_CASE("Test Lexer works") {
  std::string filename = "tests/simple_source/assign.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(input);
  std::vector<Token *> expected{
      new Token(1, TokenType::PROCEDURE),   new SymbolToken(1, "main"),
      new Token(1, TokenType::L_BRACE),     new SymbolToken(2, "i"),
      new Token(2, TokenType::EQUAL),       new NumberToken(2, "5"),
      new Token(3, TokenType::SEMI),        new Token(3, TokenType::R_BRACE),
      new Token(4, TokenType::END_OF_FILE),

  };

  // Expect that the vector of tokens are equivalent
  REQUIRE(std::equal(
      begin(lexer.tokens), end(lexer.tokens), begin(expected), end(expected),
      [](const Token *lexed, const Token *expt) { return *lexed == *expt; }));
}
