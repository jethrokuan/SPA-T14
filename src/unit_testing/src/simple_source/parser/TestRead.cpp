#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::move;
using std::shared_ptr;

TEST_CASE ("Test valid read statement") {
  auto ast =
      SimpleInterface::getAstFromFile("tests/simple_source/read/valid.txt");

  std::vector<StmtNode> stmt_list;
  std::vector<std::shared_ptr<ProcedureNode>> proc_list;

  auto read_x = make_shared<ReadNode>(make_shared<VariableNode>("x"));

  stmt_list.push_back(std::move(read_x));

  auto proc_main =
      make_shared<ProcedureNode>("main", make_shared<StmtListNode>(stmt_list));
  proc_list.push_back(proc_main);
  auto root = make_shared<RootNode>(proc_list);

  REQUIRE(*ast == *root);
}

TEST_CASE ("Test invalid read statement") {
  SECTION ("missing semicolon") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/read/invalid/missing_semicolon.txt"),
        "Expected ';', got '}'.");
  }
  SECTION ("read number") {
    // In this case, parsing of the read statement fails, so it defers to
    // the parse of an assign statement, which expects an '=' in place of the
    // number.
    REQUIRE_THROWS_WITH(SimpleInterface::getAstFromFile(
                            "tests/simple_source/read/invalid/read_number.txt"),
                        "Expected '=', got '1'.");
  }

  SECTION ("read multiple variables") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/read/invalid/read_multiple.txt"),
        "Expected ';', got 'b'.");
  }
}
