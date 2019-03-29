#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/core/exceptions.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"
#include "query_executor/query_executor.h"
#include "simple_parser/interface.h"

using namespace QE;

TEST_CASE (
    "Test Selecting SynAttrs (eg. Select c.procName) instead of Select c") {
  auto ast = Simple::SimpleInterface::getAstFromFile("tests/Test1-Source.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Select call procName") {
    auto querystr = std::string("call c; Select c.procName");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"swap", "auxiliary"});
  }

  SECTION ("Select call stmt#") {
    auto querystr = std::string("call c; Select c.stmt#");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"4", "9"});
  }

  SECTION ("Select read varName") {
    auto querystr = std::string("read r; Select r.varName");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"a", "x", "b", "c"});
  }

  SECTION ("Select print varName") {
    auto querystr = std::string("print pn; Select pn.varName");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"k"});
  }

  delete pkb;
  delete qm;
}
