#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::move;
using std::shared_ptr;

TEST_CASE ("Test valid if statement") {
  auto ast =
      SimpleInterface::getAstFromFile("tests/simple_source/if/valid.txt");

  std::vector<StmtNode> stmtList;
  std::vector<std::shared_ptr<ProcedureNode>> proc_list;

  // if (i == 0) then {
  //   read x;
  // } else {
  //   read y;
  // }

  std::vector<StmtNode> stmtListThen;
  std::vector<StmtNode> stmtListElse;

  auto read_x = make_shared<ReadNode>(make_shared<VariableNode>("x"));
  auto read_y = make_shared<ReadNode>(make_shared<VariableNode>("y"));

  stmtListThen.push_back(std::move(read_x));
  stmtListElse.push_back(std::move(read_y));

  auto if_node = make_shared<IfNode>(
      make_shared<CondExprNode>(make_shared<RelExprNode>(
          make_shared<VariableNode>("i"), "==", make_shared<NumberNode>("0"))),
      make_shared<StmtListNode>(std::move(stmtListThen)),
      make_shared<StmtListNode>(std::move(stmtListElse)));
  stmtList.push_back(std::move(if_node));

  auto proc_main =
      make_shared<ProcedureNode>("main", make_shared<StmtListNode>(stmtList));
  proc_list.push_back(proc_main);
  auto root = make_shared<RootNode>(proc_list);

  REQUIRE(*ast == *root);
}

TEST_CASE ("Test invalid if statement") {
  SECTION ("missing then") {
    REQUIRE_THROWS_WITH(SimpleInterface::getAstFromFile(
                            "tests/simple_source/if/invalid/missing_then.txt"),
                        "Expected 'then', got '{'.");
  }

  SECTION ("missing else") {
    REQUIRE_THROWS_WITH(SimpleInterface::getAstFromFile(
                            "tests/simple_source/if/invalid/missing_else.txt"),
                        "Expected 'else', got '{'.");
  }

  SECTION ("missing else") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/if/invalid/missing_else_2.txt"),
        "Expected 'else', got '}'.");
  }
}
