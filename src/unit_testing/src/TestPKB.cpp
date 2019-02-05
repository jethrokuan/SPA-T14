// TODO remove print debug statements and iostream

#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"
#include "program_knowledge_base/pkb.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;

TEST_CASE ("Test PKB works") {
  std::string filename = "tests/simple_source/simple_1.txt";
  std::ifstream input(filename);

  Simple::Lexer lexer = Simple::Lexer(input);
  lexer.parse();

  Simple::Parser parser = Simple::Parser(lexer.tokens);
  auto proc = parser.parse();

  PKB pkb = PKB(proc);

  // using temp test method
  // TODO deprecate and update test cases
  auto follows_test_1 = pkb.testFollows(1, 2);
  REQUIRE(follows_test_1 == true);
  auto follows_test_2 = pkb.testFollows(1, 4);
  REQUIRE(follows_test_2 == true);
  auto follows_test_3 = pkb.testFollows(1, 5);
  REQUIRE(follows_test_3 == true);
  auto follows_test_4 = pkb.testFollows(2, 4);
  REQUIRE(follows_test_4 == true);
  auto follows_test_5 = pkb.testFollows(2, 5);
  REQUIRE(follows_test_5 == true);
  auto follows_test_6 = pkb.testFollows(4, 5);
  REQUIRE(follows_test_6 == true);
  auto follows_test_7 = pkb.testFollows(1, 9);
  REQUIRE(follows_test_7 == false);
  auto follows_test_8 = pkb.testFollows(2, 3);
  REQUIRE(follows_test_8 == false);
  auto follows_test_9 = pkb.testFollows(4, 2);
  REQUIRE(follows_test_9 == false);
  auto follows_test_10 = pkb.testFollows(8, 0);
  REQUIRE(follows_test_10 == false);
}