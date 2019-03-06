#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/core/exceptions.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"
#include "query_executor/query_executor.h"
#include "simple_parser/interface.h"

using namespace QE;

TEST_CASE ("Test Query Manager FollowsT functionality - 1.txt") {
  auto ast =
      Simple::SimpleInterface::getAstFromFile("tests/system_tests/src/1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test select all assignments") {
    auto querystr =
        std::string("assign a; while w; Select w such that Parent(a, 24);");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{});
  }
}