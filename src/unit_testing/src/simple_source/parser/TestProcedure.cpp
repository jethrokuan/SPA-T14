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

  std::vector<StmtNode> stmtList;
  auto read = make_shared<ReadNode>(std::make_shared<VariableNode>("x"));
  stmtList.push_back(move(read));
  auto StmtList = make_shared<StmtListNode>(move(stmtList));
  auto expected = make_shared<ProcedureNode>("main", move(StmtList));

  REQUIRE(*ast == *expected);
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
