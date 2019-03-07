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

  std::vector<StmtNode> stmt_list;
  std::vector<std::shared_ptr<ProcedureNode>> proc_list;

  // while (i == 0) {
  //   read x;
  // }

  std::vector<StmtNode> while_stmt_list;

  auto read_x = make_shared<ReadNode>(make_shared<VariableNode>("x"));

  while_stmt_list.push_back(std::move(read_x));

  auto while_node = make_shared<WhileNode>(
      make_shared<CondExprNode>(make_shared<RelExprNode>(
          make_shared<VariableNode>("i"), "==", make_shared<NumberNode>("0"))),
      make_shared<StmtListNode>(std::move(while_stmt_list)));
  stmt_list.push_back(std::move(while_node));

  auto proc_main =
      make_shared<ProcedureNode>("main", make_shared<StmtListNode>(stmt_list));
  proc_list.push_back(proc_main);
  auto root = make_shared<RootNode>(proc_list);

  REQUIRE(*ast == *root);
}

TEST_CASE ("Test invalid while statement") {
  SECTION ("missing then") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/while/invalid/missing_stmtlist.txt"),
        "Expected '{', got ';'.");
  }
}
