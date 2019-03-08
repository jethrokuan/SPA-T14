#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using Simple::SimpleInterface;
using std::make_shared;
using std::shared_ptr;

bool exprEqual(Expr e1, Expr e2) {
  return std::visit([](const auto& a, const auto& b) { return *a == *b; }, e1,
                    e2);
}

TEST_CASE ("Test Expr parse works") {
  SECTION ("5") {
    Expr expr = SimpleInterface::parseExpression("5");
    Expr expected = make_shared<NumberNode>("5");

    REQUIRE(exprEqual(expr, expected));
  }

  SECTION ("2 + 5") {
    Expr expr = SimpleInterface::parseExpression("2 + 5");
    Expr expected = make_shared<BinOpNode>(make_shared<NumberNode>("2"),
                                           make_shared<NumberNode>("5"), "+");
    REQUIRE(exprEqual(expr, expected));
  }

  SECTION ("2 + 5 * j") {
    Expr expr = SimpleInterface::parseExpression("2 + 5 * j");

    Expr expected = make_shared<BinOpNode>(
        make_shared<NumberNode>("2"),
        make_shared<BinOpNode>(make_shared<NumberNode>("5"),
                               make_shared<VariableNode>("j"), "*"),
        "+");

    REQUIRE(exprEqual(expr, expected));
  }

  SECTION ("2 + (5 * j)") {
    Expr expr = SimpleInterface::parseExpression("2 + (5 * j)");

    Expr expected = make_shared<BinOpNode>(
        make_shared<NumberNode>("2"),
        make_shared<BinOpNode>(make_shared<NumberNode>("5"),
                               make_shared<VariableNode>("j"), "*"),
        "+");

    REQUIRE(exprEqual(expr, expected));
  }

  SECTION ("(2 + 5) * j") {
    Expr expr = SimpleInterface::parseExpression("(2 + 5) * j");
    Expr expected = make_shared<BinOpNode>(
        make_shared<BinOpNode>(make_shared<NumberNode>("2"),
                               make_shared<NumberNode>("5"), "+"),
        make_shared<VariableNode>("j"), "*");

    REQUIRE(exprEqual(expr, expected));
  }

  SECTION ("while (i > (2 + 5) * j)") {
    AST ast =
        SimpleInterface::getAstFromFile("tests/simple_source/arithmetic/6.txt");

    std::vector<StmtNode> stmt_list;
    std::vector<std::shared_ptr<ProcedureNode>> proc_list;

    std::vector<StmtNode> ws;
    auto assign = make_shared<AssignNode>(
        make_shared<VariableNode>("i"),
        make_shared<BinOpNode>(make_shared<VariableNode>("i"),
                               make_shared<NumberNode>("1"), "+"));

    ws.push_back(std::move(assign));

    auto w = std::make_shared<WhileNode>(
        make_shared<CondExprNode>(make_shared<RelExprNode>(
            make_shared<VariableNode>("i"), ">",
            make_shared<BinOpNode>(
                make_shared<BinOpNode>(make_shared<NumberNode>("2"),
                                       make_shared<NumberNode>("5"), "+"),
                make_shared<VariableNode>("j"), "*"))),
        make_shared<StmtListNode>(std::move(ws)));

    stmt_list.push_back(std::move(w));

    auto stmt_list_node = std::make_shared<StmtListNode>(std::move(stmt_list));

    auto proc_main =
        std::make_shared<ProcedureNode>("main", std::move(stmt_list_node));
    proc_list.push_back(proc_main);
    auto root = make_shared<RootNode>(proc_list);

    REQUIRE(*ast == *root);
  }

  SECTION ("valid assign statements") {
    REQUIRE_NOTHROW(SimpleInterface::getAstFromFile(
        "tests/simple_source/arithmetic/valid.txt"));
  }
}

TEST_CASE ("Test Expr parse throws when invalid") {
  SECTION ("consecutive operands") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/arithmetic/consecutive_operands.txt"),
        "Unexpected token '9'.");
  }

  SECTION ("double operators") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/arithmetic/double_operators.txt"),
        "Expected an expression, got '-'.");
  }

  SECTION ("empty parentheses") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/arithmetic/empty_parentheses.txt"),
        "Expected an expression, got ')'.");
  }

  SECTION ("incomplete parentheses") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/arithmetic/incomplete_parentheses.txt"),
        "Expected ')', got ';'.");
  }

  SECTION ("other invalid expressions") {
    int num_testcases = 21;
    for (int i = 0; i < num_testcases; i++) {
      std::string filename = "tests/simple_source/arithmetic/other_invalids/" +
                             std::to_string(i) + ".txt";

      REQUIRE_THROWS(SimpleInterface::getAstFromFile(filename));
    }
  }
}
