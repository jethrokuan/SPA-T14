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

    std::vector<StmtNode> while_stmt_list;

    auto read = make_shared<ReadNode>(make_shared<VariableNode>("x"));
    while_stmt_list.push_back(move(read));

    auto cond_expr = make_shared<CondExprNode>(make_shared<RelExprNode>(
        make_shared<VariableNode>("i"), "==", make_shared<NumberNode>("0")));

    auto while_node = make_shared<WhileNode>(
        std::move(cond_expr),
        make_shared<StmtListNode>(std::move(while_stmt_list)));

    std::vector<StmtNode> stmt_list;
    std::vector<std::shared_ptr<ProcedureNode>> proc_list;

    stmt_list.push_back(while_node);

    auto stmt_list_node = make_shared<StmtListNode>(move(stmt_list));
    auto proc_main = make_shared<ProcedureNode>("main", move(stmt_list_node));

    proc_list.push_back(proc_main);
    auto root = make_shared<RootNode>(move(proc_list));

    REQUIRE(*ast == *root);
  }

  SECTION ("!(i == 0)") {
    // procedure main {
    //   while (!(i == 0)) {
    //     read x;
    //   }
    // }
    auto ast = SimpleInterface::getAstFromFile(
        "tests/simple_source/cond_expr/valid_2.txt");

    std::vector<StmtNode> while_stmt_list;

    auto read = make_shared<ReadNode>(make_shared<VariableNode>("x"));
    while_stmt_list.push_back(move(read));

    auto cond_expr = make_shared<CondExprNode>(make_shared<CondExprNode>(
        make_shared<RelExprNode>(make_shared<VariableNode>("i"),
                                 "==", make_shared<NumberNode>("0"))));

    auto while_node = make_shared<WhileNode>(
        std::move(cond_expr),
        make_shared<StmtListNode>(std::move(while_stmt_list)));

    std::vector<StmtNode> stmt_list;
    std::vector<std::shared_ptr<ProcedureNode>> proc_list;

    stmt_list.push_back(while_node);

    auto stmt_list_node = make_shared<StmtListNode>(move(stmt_list));
    auto proc_main = make_shared<ProcedureNode>("main", move(stmt_list_node));

    proc_list.push_back(move(proc_main));

    auto root = make_shared<RootNode>(move(proc_list));

    REQUIRE(*ast == *root);
  }

  SECTION ("(i == 0) && (j >= 1)") {
    // procedure main {
    //   while ((i == 0) && (j >= 1)) {
    //     read x;
    //   }
    // }
    auto ast = SimpleInterface::getAstFromFile(
        "tests/simple_source/cond_expr/valid_3.txt");

    std::vector<StmtNode> while_stmt_list;

    auto read = make_shared<ReadNode>(make_shared<VariableNode>("x"));
    while_stmt_list.push_back(move(read));

    auto cond_expr = make_shared<CondExprNode>(
        make_shared<CondExprNode>(
            make_shared<RelExprNode>(make_shared<VariableNode>("i"),
                                     "==", make_shared<NumberNode>("0"))),
        "&&",
        make_shared<CondExprNode>(
            make_shared<RelExprNode>(make_shared<VariableNode>("j"),
                                     ">=", make_shared<NumberNode>("1"))));

    auto while_node = make_shared<WhileNode>(
        std::move(cond_expr),
        make_shared<StmtListNode>(std::move(while_stmt_list)));

    std::vector<StmtNode> stmt_list;
    std::vector<std::shared_ptr<ProcedureNode>> proc_list;

    stmt_list.push_back(while_node);

    auto stmt_list_node = make_shared<StmtListNode>(move(stmt_list));
    auto proc_main = make_shared<ProcedureNode>("main", move(stmt_list_node));

    proc_list.push_back(proc_main);

    auto root = make_shared<RootNode>(proc_list);

    REQUIRE(*ast == *root);
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
