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

    std::vector<StmtNode> stmt_list;
    std::vector<std::shared_ptr<ProcedureNode>> proc_list;

    auto assign = make_shared<AssignNode>(make_shared<VariableNode>("x"),
                                          make_shared<NumberNode>("1"));
    stmt_list.push_back(std::move(assign));

    auto proc_main = make_shared<ProcedureNode>("main", stmt_list);

    proc_list.push_back(std::move(proc_main));

    auto root = make_shared<RootNode>(std::move(proc_list));

    REQUIRE(*ast == *root);
  }

  SECTION ("if = while") {
    auto ast = SimpleInterface::getAstFromFile(
        "tests/simple_source/assign/valid_2.txt");

    std::vector<StmtNode> stmt_list;
    std::vector<std::shared_ptr<ProcedureNode>> proc_list;

    auto assign = make_shared<AssignNode>(make_shared<VariableNode>("if"),
                                          make_shared<VariableNode>("while"));
    stmt_list.push_back(std::move(assign));

    auto proc_main = make_shared<ProcedureNode>("main", stmt_list);
    proc_list.push_back(std::move(proc_main));

    auto root = make_shared<RootNode>(std::move(proc_list));

    REQUIRE(*ast == *root);
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
