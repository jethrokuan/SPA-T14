// TODO remove print debug statements and iostream

#include "program_knowledge_base/pkb.h"
#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;

// TODO split up test cases into follows parent uses modifies
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

  // test follows
  auto follows_test_1 = pkb.testFollows(1, 2);
  REQUIRE(follows_test_1 == true);
  auto follows_test_2 = pkb.testFollows(1, 4);
  REQUIRE(follows_test_2 == false);
  auto follows_test_3 = pkb.testFollows(1, 5);
  REQUIRE(follows_test_3 == false);
  auto follows_test_4 = pkb.testFollows(2, 4);
  REQUIRE(follows_test_4 == true);
  auto follows_test_5 = pkb.testFollows(2, 5);
  REQUIRE(follows_test_5 == false);
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

  // test parent
  auto parent_test_1 = pkb.testParent(2, 3);
  REQUIRE(parent_test_1 == true);
  auto parent_test_2 = pkb.testParent(1, 2);
  REQUIRE(parent_test_2 == false);
  auto parent_test_3 = pkb.testParent(3, 3);
  REQUIRE(parent_test_3 == false);
  auto parent_test_4 = pkb.testParent(4, 3);
  REQUIRE(parent_test_4 == false);

  // test uses
  auto uses_test_1 = pkb.testUses(2, "i");
  REQUIRE(uses_test_1 == true);
  auto uses_test_2 = pkb.testUses(1, "i");
  REQUIRE(uses_test_2 == false);
  auto uses_test_3 = pkb.testUses(5, "i");
  REQUIRE(uses_test_3 == false);
  auto uses_test_4 = pkb.testUses(2, "alpha");
  REQUIRE(uses_test_4 == false);

  // test uses
  auto modifies_test_1 = pkb.testModifies(5, "i");
  REQUIRE(modifies_test_1 == true);
  auto modifies_test_2 = pkb.testModifies(1, "i");
  REQUIRE(modifies_test_2 == true);
  auto modifies_test_3 = pkb.testModifies(1, "beta");
  REQUIRE(modifies_test_3 == false);
  auto modifies_test_4 = pkb.testModifies(2, "alpha");
  REQUIRE(modifies_test_4 == false);
  auto modifies_test_5 = pkb.testModifies(22, "alpha");
  REQUIRE(modifies_test_5 == false);
  auto modifies_test_6 = pkb.testModifies(10, "i");
  REQUIRE(modifies_test_6 == false);
}
