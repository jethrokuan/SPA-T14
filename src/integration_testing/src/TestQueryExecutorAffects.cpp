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

TEST_CASE ("Test Query Executor on Affects queries") {
  auto ast =
      Simple::SimpleInterface::getAstFromFile("tests/Affects-Source1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test Affects(a1, a2)") {
    auto querystr =
        std::string("assign a1, a2; Select <a1, a2> such that Affects(a1, a2)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"1 3", "3 3"});
  }

  SECTION ("Test Affects(a1, 3)") {
    auto querystr =
        std::string("assign a1, a2; Select a1 such that Affects(a1, 3)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"1", "3"});
  }

  SECTION ("Test Affects(1, a1)") {
    auto querystr =
        std::string("assign a1, a2; Select a1 such that Affects(1, a1)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"3"});
  }

  SECTION ("Test Affects(1, 3)") {
    auto querystr =
        std::string("assign a1, a2; Select BOOLEAN such that Affects(1, 3)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test Affects(3, 3)") {
    auto querystr =
        std::string("assign a1, a2; Select BOOLEAN such that Affects(3, 3)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }
}

TEST_CASE ("Test Query Executor on Affects queries (source 2)") {
  auto ast =
      Simple::SimpleInterface::getAstFromFile("tests/Affects-Source2.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test Affects(a1, a2)") {
    auto querystr =
        std::string("assign a1, a2; Select <a1, a2> such that Affects(a1, a2)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"2 3", "3 2"});
  }

  SECTION ("Test Affects(a1, 3)") {
    auto querystr =
        std::string("assign a1, a2; Select a1 such that Affects(a1, 3)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"2"});
  }

  SECTION ("Test Affects(3, a1)") {
    auto querystr =
        std::string("assign a1, a2; Select a1 such that Affects(3, a1)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"2"});
  }
}
