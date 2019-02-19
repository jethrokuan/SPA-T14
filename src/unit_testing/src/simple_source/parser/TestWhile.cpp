#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::move;
using std::shared_ptr;

TEST_CASE ("Test valid while statement") {
  auto ast =
      SimpleInterface::getAstFromFile("tests/simple_source/while/valid.txt");

  std::vector<StmtNode> stmtList;

  // while (i == 0) {
  //   read x;
  // }

  std::vector<StmtNode> whileStmtList;

  auto read_x = make_shared<ReadNode>(make_shared<VariableNode>("x"));

  whileStmtList.push_back(std::move(read_x));

  auto whileNode = make_shared<WhileNode>(
      make_shared<CondExprNode>(make_shared<RelExprNode>(
          make_shared<VariableNode>("i"), "==", make_shared<NumberNode>("0"))),
      make_shared<StmtListNode>(std::move(whileStmtList)));
  stmtList.push_back(std::move(whileNode));

  auto expected =
      make_shared<ProcedureNode>("main", make_shared<StmtListNode>(stmtList));

  REQUIRE(*ast == *expected);
}

TEST_CASE ("Test invalid while statement") {
  SECTION ("missing then") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/while/invalid/missing_stmtlist.txt"),
        "Expected '{', got ';'.");
  }
}
