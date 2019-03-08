#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::move;
using std::shared_ptr;

TEST_CASE ("Test valid call statement") {
  SECTION ("call foo;") {
    auto ast =
        SimpleInterface::getAstFromFile("tests/simple_source/call/valid.txt");

    std::vector<StmtNode> stmt_list;
    std::vector<std::shared_ptr<ProcedureNode>> proc_list;

    auto call = make_shared<CallNode>("foo");
    stmt_list.push_back(std::move(call));

    auto proc_main = make_shared<ProcedureNode>(
        "main", stmt_list);

    proc_list.push_back(std::move(proc_main));

    auto root = make_shared<RootNode>(std::move(proc_list));

    REQUIRE(*ast == *root);
  }
}

TEST_CASE ("Test invalid call statement") {
  SECTION ("missing semicolon") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/call/invalid/missing_semicolon.txt"),
        "Expected ';', got '}'.");
  }

  SECTION ("call 1;") {
    REQUIRE_THROWS_WITH(SimpleInterface::getAstFromFile(
                            "tests/simple_source/call/invalid/call_number.txt"),
                        "Expected '=', got '1'.");
  }
}
