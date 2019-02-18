#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::move;
using std::shared_ptr;

TEST_CASE ("Test valid conditional expression") {
  SECTION ("i == 0") {
    // procedure main {
    //   while (i == 0) {
    //     read x;
    //   }
    // }
    auto ast = SimpleInterface::getAstFromFile(
        "tests/simple_source/cond_expr/valid_1.txt");

    std::vector<StmtNode> whileStmtList;

    auto read = make_shared<ReadNode>(make_shared<VariableNode>("x"));
    whileStmtList.push_back(move(read));

    auto condExpr = make_shared<CondExprNode>(make_shared<RelExprNode>(
        make_shared<VariableNode>("i"), "==", make_shared<NumberNode>("0")));

    auto whileNode = make_shared<WhileNode>(
        std::move(condExpr),
        make_shared<StmtListNode>(std::move(whileStmtList)));

    std::vector<StmtNode> stmtList;
    stmtList.push_back(whileNode);

    auto StmtList = make_shared<StmtListNode>(move(stmtList));
    auto expected = make_shared<ProcedureNode>("main", move(StmtList));

    REQUIRE(*ast == *expected);
  }

  SECTION ("!(i == 0)") {
    // procedure main {
    //   while (!(i == 0)) {
    //     read x;
    //   }
    // }
    auto ast = SimpleInterface::getAstFromFile(
        "tests/simple_source/cond_expr/valid_2.txt");

    std::vector<StmtNode> whileStmtList;

    auto read = make_shared<ReadNode>(make_shared<VariableNode>("x"));
    whileStmtList.push_back(move(read));

    auto condExpr = make_shared<CondExprNode>(make_shared<CondExprNode>(
        make_shared<RelExprNode>(make_shared<VariableNode>("i"),
                                 "==", make_shared<NumberNode>("0"))));

    auto whileNode = make_shared<WhileNode>(
        std::move(condExpr),
        make_shared<StmtListNode>(std::move(whileStmtList)));

    std::vector<StmtNode> stmtList;
    stmtList.push_back(whileNode);

    auto StmtList = make_shared<StmtListNode>(move(stmtList));
    auto expected = make_shared<ProcedureNode>("main", move(StmtList));

    REQUIRE(*ast == *expected);
  }

  SECTION ("(i == 0) && (j >= 1)") {
    // procedure main {
    //   while ((i == 0) && (j >= 1)) {
    //     read x;
    //   }
    // }
    auto ast = SimpleInterface::getAstFromFile(
        "tests/simple_source/cond_expr/valid_3.txt");

    std::vector<StmtNode> whileStmtList;

    auto read = make_shared<ReadNode>(make_shared<VariableNode>("x"));
    whileStmtList.push_back(move(read));

    auto condExpr = make_shared<CondExprNode>(
        make_shared<CondExprNode>(
            make_shared<RelExprNode>(make_shared<VariableNode>("i"),
                                     "==", make_shared<NumberNode>("0"))),
        "&&",
        make_shared<CondExprNode>(
            make_shared<RelExprNode>(make_shared<VariableNode>("j"),
                                     ">=", make_shared<NumberNode>("1"))));

    auto whileNode = make_shared<WhileNode>(
        std::move(condExpr),
        make_shared<StmtListNode>(std::move(whileStmtList)));

    std::vector<StmtNode> stmtList;
    stmtList.push_back(whileNode);

    auto StmtList = make_shared<StmtListNode>(move(stmtList));
    auto expected = make_shared<ProcedureNode>("main", move(StmtList));

    REQUIRE(*ast == *expected);
  }
}

TEST_CASE ("Test invalid conditional expression") {
  SECTION ("while(i = 0)") {
    // In this case the parser thinks the expression is yet to be fully parsed
    // and was expecting an operator or semicolon.
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/cond_expr/invalid/invalid_rel_operator.txt"),
        "Unexpected token '='.");
  }
  SECTION ("while()") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/cond_expr/invalid/missing.txt"),
        "Expected an expression, got ')'.");
  }
  SECTION ("while(i)") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/cond_expr/invalid/missing_op.txt"),
        "Expected a comparator, got ')'.");
  }
  SECTION ("while ((i == 0) (a == j))") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/cond_expr/invalid/missing_op_2.txt"),
        "Expected '||' or '&&', got '('.");
  }
  SECTION ("while ((i == 0) && a == j)") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/cond_expr/invalid/missing_paren.txt"),
        "Expected '(', got 'a'.");
  }
}
