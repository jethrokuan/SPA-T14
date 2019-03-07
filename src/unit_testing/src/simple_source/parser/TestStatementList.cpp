#include "simple_parser/interface.h"

#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <memory>

using namespace Simple;
using std::make_shared;
using std::move;
using std::shared_ptr;

TEST_CASE ("Test valid statement list") {
  auto ast = SimpleInterface::getAstFromFile(
      "tests/simple_source/statement_list/valid.txt");

  REQUIRE(ast->ProcList[0]->StmtList->StmtList.size() == 3);
}

TEST_CASE ("Test invalid statement list") {
  SECTION ("Empty statement list") {
    REQUIRE_THROWS_WITH(
        SimpleInterface::getAstFromFile(
            "tests/simple_source/statement_list/invalid/empty.txt"),
        "Statement list cannot be empty.");
  }
}
