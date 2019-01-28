#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_unique;
using std::unique_ptr;

TEST_CASE("Test Assign Expr") {
  std::string filename = "tests/simple_source/assign.txt";
  std::ifstream input(filename);

  Simple::Lexer lexer = Simple::Lexer(input);
  Simple::Parser parser = Simple::Parser(lexer.tokens);
  auto proc = parser.parse();

  auto expected = std::make_unique<ProcedureExpr>(
      std::make_unique<VariableExpr>("main"),
      std::make_unique<AssignExpr>(std::make_unique<VariableExpr>("i"),
                                   std::make_unique<NumberExpr>("5")));

  REQUIRE(*proc == *expected);
}

TEST_CASE("Test Read Expr") {
  std::string filename = "tests/simple_source/read.txt";
  std::ifstream input(filename);

  Simple::Lexer lexer = Simple::Lexer(input);
  Simple::Parser parser = Simple::Parser(lexer.tokens);
  auto proc = parser.parse();

  auto expected = std::make_unique<ProcedureExpr>(
      std::make_unique<VariableExpr>("main"),
      std::make_unique<ReadExpr>(std::make_unique<VariableExpr>("i")));

  REQUIRE(*proc == *expected);
}

TEST_CASE("Test Print Expr") {
  std::string filename = "tests/simple_source/print.txt";
  std::ifstream input(filename);

  Simple::Lexer lexer = Simple::Lexer(input);
  Simple::Parser parser = Simple::Parser(lexer.tokens);
  auto proc = parser.parse();

  auto expected = std::make_unique<ProcedureExpr>(
      std::make_unique<VariableExpr>("main"),
      std::make_unique<PrintExpr>(std::make_unique<VariableExpr>("i")));

  REQUIRE(*proc == *expected);
}

TEST_CASE("Expr equality comparisons") {
  SECTION("VariableExpr") {
    auto v1 = make_unique<VariableExpr>("i");
    auto v2 = make_unique<VariableExpr>("i");
    auto v3 = make_unique<VariableExpr>("j");

    REQUIRE(*v1 == *v2);
    REQUIRE(!(*v1 == *v3));
  }

  SECTION("NumberExpr") {
    auto n1 = make_unique<NumberExpr>("1");
    auto n2 = make_unique<NumberExpr>("1");
    auto n3 = make_unique<NumberExpr>("2");

    REQUIRE(*n1 == *n2);
    REQUIRE(!(*n1 == *n3));
  }

  SECTION("AssignExpr") {
    auto a1 = make_unique<AssignExpr>(make_unique<VariableExpr>("i"),
                                      make_unique<NumberExpr>("1"));
    auto a2 = make_unique<AssignExpr>(make_unique<VariableExpr>("i"),
                                      make_unique<NumberExpr>("1"));
    auto a3 = make_unique<AssignExpr>(make_unique<VariableExpr>("i"),
                                      make_unique<NumberExpr>("2"));
    auto a4 = make_unique<AssignExpr>(make_unique<VariableExpr>("j"),
                                      make_unique<NumberExpr>("1"));

    REQUIRE(*a1 == *a2);
    REQUIRE(!(*a1 == *a3));
    REQUIRE(!(*a1 == *a4));
  }

  SECTION("ProcedureExpr") {
    auto p1 = make_unique<ProcedureExpr>(
        make_unique<VariableExpr>("main"),
        make_unique<AssignExpr>(make_unique<VariableExpr>("i"),
                                make_unique<NumberExpr>("5")));
    auto p2 = make_unique<ProcedureExpr>(
        make_unique<VariableExpr>("main"),
        make_unique<AssignExpr>(make_unique<VariableExpr>("i"),
                                make_unique<NumberExpr>("5")));
    auto p3 = make_unique<ProcedureExpr>(
        make_unique<VariableExpr>("main2"),
        make_unique<AssignExpr>(make_unique<VariableExpr>("i"),
                                make_unique<NumberExpr>("5")));

    REQUIRE(*p1 == *p2);
    REQUIRE(!(*p1 == *p3));
  }
}
