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
    "Test 1 Integration Testing since we have the answers :) "
    "(tests/Test1-Source.txt)") {
  auto ast = Simple::SimpleInterface::getAstFromFile("tests/Test1-Source.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();
  auto querystr = std::string(
      "stmt s, s1; assign a, a1; while w; if ifs; variable v, v1; procedure p, "
      "q; constant c; prog_line n; call cl; read re; print pn;");

  SECTION ("Select s such that Follows (18, s)") {
    querystr += std::string("Select s such that Follows (18, s)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION ("Select s such that Next (18, s)") {
    querystr += std::string("Select s such that Next (18, s)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"16"});
  }

  SECTION ("Select v such that Uses (_, v)") {
    querystr += std::string("Select v such that Uses (_, v)");
    REQUIRE_THROWS_AS(qe.makePqlQuery(querystr), PQLValidationException);
  }

  SECTION ("Select a pattern a (_, _\"a-b\"_)") {
    querystr += std::string("Select a pattern a (_, _\"a-b\"_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"6, 7, 17"});
  }

  SECTION ("Select a pattern a (_, _\"a-b\"_)") {
    querystr += std::string("Select a pattern a (_, _\"a-b\"_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"17", "6, 7"});
  }

  delete pkb;
  delete qm;
}
