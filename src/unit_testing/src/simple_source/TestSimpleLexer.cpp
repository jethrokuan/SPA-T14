#include "simple_parser/exceptions.h"
#include "simple_parser/lexer.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>

using namespace Simple;
using Simple::TokenType;

TEST_CASE ("Test Lex Tokens") {
  std::string filename = "tests/simple_source/tokens.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(&input);
  auto tokens = lexer.lex();
  std::vector<Token*> expected{
      new SymbolToken("procedure"), new SymbolToken("main"),
      new NumberToken("5"),         new NumberToken("50"),
      new PunctToken("{"),          new PunctToken("}"),
      new PunctToken("("),          new PunctToken(")"),
      new PunctToken("="),          new PunctToken("!="),
      new PunctToken("=="),         new PunctToken("!"),
      new PunctToken("&&"),         new PunctToken("||"),
      new SymbolToken("read"),      new SymbolToken("print"),
      new PunctToken("+"),          new PunctToken("-"),
      new PunctToken("*"),          new PunctToken("/"),
      new PunctToken("%"),          new PunctToken(">"),
      new PunctToken(">="),         new PunctToken("<"),
      new PunctToken("<="),         new EndOfFileToken(),
  };

  REQUIRE(std::equal(
      begin(lexer.tokens), end(lexer.tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
};

TEST_CASE ("Test Lex Assign Statement") {
  std::string filename = "tests/simple_source/assign.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(&input);
  auto tokens = lexer.lex();
  std::vector<Token*> expected{
      new SymbolToken("procedure"), new SymbolToken("main"),
      new PunctToken("{"),          new SymbolToken("i"),
      new PunctToken("="),          new NumberToken("2"),
      new PunctToken("+"),          new NumberToken("5"),
      new PunctToken(";"),          new PunctToken("}"),
      new EndOfFileToken(),
  };

  // Expect that the vector of tokens are equivalent
  REQUIRE(std::equal(
      begin(tokens), end(tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
}

TEST_CASE ("Test Lex While Statement") {
  std::string filename = "tests/simple_source/while.txt";
  std::ifstream input(filename);

  auto lexer = Lexer(&input);
  auto tokens = lexer.lex();
  std::vector<Token*> expected{
      new SymbolToken("procedure"), new SymbolToken("main"),
      new PunctToken("{"),          new SymbolToken("while"),
      new PunctToken("("),          new SymbolToken("i"),
      new PunctToken("=="),         new NumberToken("5"),
      new PunctToken(")"),          new PunctToken("{"),
      new SymbolToken("i"),         new PunctToken("="),
      new SymbolToken("i"),         new PunctToken("+"),
      new NumberToken("1"),         new PunctToken(";"),
      new PunctToken("}"),          new PunctToken("}"),
      new EndOfFileToken(),
  };

  // Expect that the vector of tokens are equivalent
  REQUIRE(std::equal(
      begin(tokens), end(tokens), begin(expected), end(expected),
      [](const Token* lexed, const Token* expt) { return *lexed == *expt; }));
}

TEST_CASE ("Test Lex Errors") {
  SECTION ("Single & token") {
    std::string filename = "tests/simple_source/lexer/errors/single_amp.txt";
    std::ifstream input(filename);
    auto lexer = Lexer(&input);

    REQUIRE_THROWS_AS(lexer.lex(), SimpleLexException);
  }

  SECTION ("Single | token") {
    std::string filename = "tests/simple_source/lexer/errors/single_bar.txt";
    std::ifstream input(filename);
    auto lexer = Lexer(&input);

    REQUIRE_THROWS_AS(lexer.lex(), SimpleLexException);
  }

  // TODO: Test invalid tokens
  // If possible, also test if error message is correct.
  // - non-ascii
  // - invalid punctuations
  // - other weird combinations
}
