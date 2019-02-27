#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/core/exceptions.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"
#include "query_manager/query_manager.h"
#include "simple_parser/interface.h"

using namespace QE;

TEST_CASE (
    "Test Query Manager FollowsT functionality - system_tests/src/1.txt") {
  auto ast =
      Simple::SimpleInterface::getAstFromFile("tests/system_tests/src/1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryManager(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test basic pattern a (_, _)") {
    auto querystr = std::string("assign a; Select a pattern a (_, _);");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"10", "11", "12",
                                                             "13", "14", "15",
                                                             "16", "21", "22"});
  }

  SECTION ("Test doubleunderscorefactor with none: pattern a (_, _\"x\"_)") {
    auto querystr = std::string("assign a; Select a pattern a(_, _\"x\"_);");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{});
  }

  SECTION (
      "Test doubleunderscorefactor with some results: pattern a (_, _\"x\"_)") {
    auto querystr = std::string("assign a; Select a pattern a(_, _\"v5\"_);");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"10"});
  }

  delete pkb;
  delete qm;
}
