#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::move;
using std::shared_ptr;

TEST_CASE ("Test valid assign statement") {
  SECTION ("x = 1") {
    auto ast = SimpleInterface::getAstFromFile(
        "tests/simple_source/assign/valid_1.txt");

    std::vector<StmtNode> stmtList;

    auto assign = make_shared<AssignNode>(make_shared<VariableNode>("x"),
                                          make_shared<NumberNode>("1"));
    stmtList.push_back(std::move(assign));

    auto expected =
        make_shared<ProcedureNode>("main", make_shared<StmtListNode>(stmtList));

    REQUIRE(*ast == *expected);
  }

  SECTION ("if = while") {
    auto ast = SimpleInterface::getAstFromFile(
        "tests/simple_source/assign/valid_2.txt");

    std::vector<StmtNode> stmtList;

    auto assign = make_shared<AssignNode>(make_shared<VariableNode>("if"),
                                          make_shared<VariableNode>("while"));
    stmtList.push_back(std::move(assign));

    auto expected =
        make_shared<ProcedureNode>("main", make_shared<StmtListNode>(stmtList));

    REQUIRE(*ast == *expected);
  }
}

TEST_CASE ("Test invalid assign statement") {
  SECTION ("1 = 2") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/assign/invalid/assign_to_const.txt"),
        "Expected a variable name for assign statement, got '1'.");
  }

  SECTION ("missing semicolon") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/assign/invalid/missing_semicolon.txt"),
        "Unexpected token '}'.");
  }

  SECTION ("invalid expr") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/assign/invalid/invalid_expr.txt"),
        "Expected an expression, got '='.");
  }
}
