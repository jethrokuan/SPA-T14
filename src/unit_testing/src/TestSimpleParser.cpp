#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_unique;
using std::unique_ptr;

TEST_CASE("Test Parser works") {
  std::string filename = "tests/simple_source/simple_1.txt";
  std::ifstream input(filename);

  Simple::Lexer lexer = Simple::Lexer(input);
  Simple::Parser parser = Simple::Parser(lexer.tokens);
  auto proc = parser.parse();
  std::vector<unique_ptr<Expr>> stmtList;

  // i = 5;
  auto assign = std::make_unique<AssignExpr>(
      std::make_unique<VariableExpr>("i"), std::make_unique<NumberExpr>("5"));

  // read i;
  auto read = make_unique<ReadExpr>(std::make_unique<VariableExpr>("i"));
  stmtList.push_back(std::move(assign));
  stmtList.push_back(std::move(read));

  auto expected = std::make_unique<ProcedureExpr>(
      std::make_unique<VariableExpr>("main"), std::move(stmtList));

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
    auto a1 = make_unique<AssignExpr>(make_unique<VariableExpr>("i"),
                                      make_unique<NumberExpr>("1"));
    auto a2 = make_unique<AssignExpr>(make_unique<VariableExpr>("i"),
                                      make_unique<NumberExpr>("1"));
    auto a3 = make_unique<AssignExpr>(make_unique<VariableExpr>("i"),
                                      make_unique<NumberExpr>("2"));

    std::vector<unique_ptr<Expr>> s1;
    std::vector<unique_ptr<Expr>> s2;
    std::vector<unique_ptr<Expr>> s3;

    s1.push_back(std::move(a1));
    s2.push_back(std::move(a2));
    s3.push_back(std::move(a3));

    auto p1 = make_unique<ProcedureExpr>(make_unique<VariableExpr>("main"),
                                         std::move(s1));
    auto p2 = make_unique<ProcedureExpr>(make_unique<VariableExpr>("main"),
                                         std::move(s2));
    auto p3 = make_unique<ProcedureExpr>(make_unique<VariableExpr>("main2"),
                                         std::move(s2));
    auto p4 = make_unique<ProcedureExpr>(make_unique<VariableExpr>("main"),
                                         std::move(s3));

    REQUIRE(*p1 == *p2);
    REQUIRE(!(*p1 == *p3));
    REQUIRE(!(*p1 == *p4));
  }
}
