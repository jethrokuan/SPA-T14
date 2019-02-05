#include "catch.hpp"

#include <iostream>
#include <vector>
#include "query_evaluator/core/query_tokenizer.h"

TEST_CASE ("Test Empty Query Tokenize") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct = qt.getTokens("");
  REQUIRE(tokenStruct.declaration_tokens == nullptr);
  REQUIRE(tokenStruct.select_tokens == nullptr);
  REQUIRE(tokenStruct.such_that_tokens == nullptr);
  REQUIRE(tokenStruct.pattern_tokens == nullptr);
}

TEST_CASE ("Test one assignment one select Tokenization") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct = qt.getTokens("assign p;Select p");
  REQUIRE(*(tokenStruct.declaration_tokens) ==
          std::vector<std::string>{"assign", "p"});
  REQUIRE(*(tokenStruct.select_tokens) ==
          std::vector<std::string>{"Select", "p"});
  REQUIRE(tokenStruct.such_that_tokens == nullptr);
  REQUIRE(tokenStruct.pattern_tokens == nullptr);
}

TEST_CASE ("Test two assignments one select Tokenization") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct = qt.getTokens("assign p;statement w;Select p");
  REQUIRE(*(tokenStruct.declaration_tokens) ==
          std::vector<std::string>{"assign", "p", "statement", "w"});
  REQUIRE(*(tokenStruct.select_tokens) ==
          std::vector<std::string>{"Select", "p"});
  REQUIRE(tokenStruct.such_that_tokens == nullptr);
  REQUIRE(tokenStruct.pattern_tokens == nullptr);
}

TEST_CASE ("Test one assignment one select one such that Tokenization") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct = qt.getTokens("assign p;Select p such that x y z");
  REQUIRE(*(tokenStruct.declaration_tokens) ==
          std::vector<std::string>{"assign", "p"});
  REQUIRE(*(tokenStruct.select_tokens) ==
          std::vector<std::string>{"Select", "p"});
  REQUIRE(*(tokenStruct.such_that_tokens) ==
          std::vector<std::string>{"such", "that", "x", "y", "z"});
  REQUIRE(tokenStruct.pattern_tokens == nullptr);
}

TEST_CASE ("Test one assignment one select with newline Tokenization") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct = qt.getTokens("assign p;\nSelect p");
  REQUIRE(*(tokenStruct.declaration_tokens) ==
          std::vector<std::string>{"assign", "p"});
  REQUIRE(*(tokenStruct.select_tokens) ==
          std::vector<std::string>{"Select", "p"});
  REQUIRE(tokenStruct.such_that_tokens == nullptr);
  REQUIRE(tokenStruct.pattern_tokens == nullptr);
}

TEST_CASE ("Test one assignment one select with 2x newline Tokenization") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct = qt.getTokens("assign p;\nSelect p\n");
  REQUIRE(*(tokenStruct.declaration_tokens) ==
          std::vector<std::string>{"assign", "p"});
  REQUIRE(*(tokenStruct.select_tokens) ==
          std::vector<std::string>{"Select", "p"});
  REQUIRE(tokenStruct.such_that_tokens == nullptr);
  REQUIRE(tokenStruct.pattern_tokens == nullptr);
}

TEST_CASE (
    "Test one assignment one select one such that one pattern Tokenization") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct =
      qt.getTokens("assign p;Select p such that x y z pattern a b c d");
  REQUIRE(*(tokenStruct.declaration_tokens) ==
          std::vector<std::string>{"assign", "p"});
  REQUIRE(*(tokenStruct.select_tokens) ==
          std::vector<std::string>{"Select", "p"});
  REQUIRE(*(tokenStruct.such_that_tokens) ==
          std::vector<std::string>{"such", "that", "x", "y", "z"});
  REQUIRE(*(tokenStruct.pattern_tokens) ==
          std::vector<std::string>{"pattern", "a", "b", "c", "d"});
}

TEST_CASE ("Test one assignment one select one pattern Tokenization") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct = qt.getTokens("assign p;Select p pattern a b c d");
  REQUIRE(*(tokenStruct.declaration_tokens) ==
          std::vector<std::string>{"assign", "p"});
  REQUIRE(*(tokenStruct.select_tokens) ==
          std::vector<std::string>{"Select", "p"});
  REQUIRE(tokenStruct.such_that_tokens == nullptr);
  REQUIRE(*(tokenStruct.pattern_tokens) ==
          std::vector<std::string>{"pattern", "a", "b", "c", "d"});
}

TEST_CASE ("Test two assignment one select one such that Tokenization") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct =
      qt.getTokens("assign p;statement w;Select p such that x y z");
  REQUIRE(*(tokenStruct.declaration_tokens) ==
          std::vector<std::string>{"assign", "p", "statement", "w"});
  REQUIRE(*(tokenStruct.select_tokens) ==
          std::vector<std::string>{"Select", "p"});
  REQUIRE(*(tokenStruct.such_that_tokens) ==
          std::vector<std::string>{"such", "that", "x", "y", "z"});
  REQUIRE(tokenStruct.pattern_tokens == nullptr);
}

TEST_CASE (
    "Test two assignment one select one such that one pattern Tokenization") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct = qt.getTokens(
      "assign p;statement w;Select p such that x y z pattern a b c d");
  REQUIRE(*(tokenStruct.declaration_tokens) ==
          std::vector<std::string>{"assign", "p", "statement", "w"});
  REQUIRE(*(tokenStruct.select_tokens) ==
          std::vector<std::string>{"Select", "p"});
  REQUIRE(*(tokenStruct.such_that_tokens) ==
          std::vector<std::string>{"such", "that", "x", "y", "z"});
  REQUIRE(*(tokenStruct.pattern_tokens) ==
          std::vector<std::string>{"pattern", "a", "b", "c", "d"});
}

TEST_CASE ("Test two assignment one select one pattern Tokenization") {
  auto qt = QE::QueryTokenizer();
  auto tokenStruct =
      qt.getTokens("assign p;statement w;Select p pattern a b c d");
  REQUIRE(*(tokenStruct.declaration_tokens) ==
          std::vector<std::string>{"assign", "p", "statement", "w"});
  REQUIRE(*(tokenStruct.select_tokens) ==
          std::vector<std::string>{"Select", "p"});
  REQUIRE(tokenStruct.such_that_tokens == nullptr);
  REQUIRE(*(tokenStruct.pattern_tokens) ==
          std::vector<std::string>{"pattern", "a", "b", "c", "d"});
}