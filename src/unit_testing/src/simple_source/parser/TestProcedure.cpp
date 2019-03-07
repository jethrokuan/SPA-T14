#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::move;
using std::shared_ptr;

TEST_CASE ("Test Valid Procedure") {
  auto ast = SimpleInterface::getAstFromFile(
      "tests/simple_source/procedure/valid.txt");

  std::vector<StmtNode> stmt_list;
  std::vector<std::shared_ptr<ProcedureNode>> proc_list;

  auto read = make_shared<ReadNode>(std::make_shared<VariableNode>("x"));
  stmt_list.push_back(move(read));
  auto stmt_list_node = make_shared<StmtListNode>(move(stmt_list));
  auto proc_main = make_shared<ProcedureNode>("main", move(stmt_list_node));
  proc_list.push_back(proc_main);
  auto root = make_shared<RootNode>(proc_list);

  REQUIRE(*ast == *root);
}

TEST_CASE ("Test Invalid Procedures") {
  SECTION ("Invalid case") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/procedure/invalid/Procedure.txt"),
        "Expected 'procedure', got 'Procedure'.");
  }

  SECTION ("Invalid procedure specification") {
    REQUIRE_THROWS_WITH(SimpleInterface::getAstFromFile(
                            "tests/simple_source/procedure/invalid/proc.txt"),
                        "Expected 'procedure', got 'proc'.");
  }

  SECTION ("procedure with number as name") {
    REQUIRE_THROWS_WITH(SimpleInterface::getAstFromFile(
                            "tests/simple_source/procedure/invalid/number.txt"),
                        "Expected a valid procedure name, got '1'.");
  }

  SECTION ("procedure with missing name") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/procedure/invalid/missing_name.txt"),
        "Expected a valid procedure name, got '{'.");
  }
}
