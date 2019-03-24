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

TEST_CASE (
    "Test Query Executor Pattern functionality - system_tests/src/1.txt") {
  auto ast =
      Simple::SimpleInterface::getAstFromFile("tests/system_tests/src/1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test basic pattern a (_, _)") {
    auto querystr = std::string("assign a; Select a pattern a (_, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "12", "13", "14", "15", "16",
                                     "21", "22"});
  }

  SECTION ("Test doubleunderscorefactor with none: pattern a (_, _\"x\"_)") {
    auto querystr = std::string("assign a; Select a pattern a(_, _\"x\"_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION (
      "Test doubleunderscorefactor with some results: pattern a (_, _\"x\"_)") {
    auto querystr = std::string("assign a; Select a pattern a(_, _\"v5\"_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"10"});
  }

  SECTION (
      "Test pattern constraining unselected variable that returns no results: "
      "Issue 231") {
    auto querystr =
        std::string("assign a; variable v; Select v pattern a(_, _\"v6\"_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION (
      "Test pattern constraining unselected variable that returns no results: "
      "Issue 231 + LHS pattern as string") {
    auto querystr = std::string(
        "assign a; variable v; Select v pattern a(\"x\", _\"v6\"_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION ("Test exact pattern match - only one result") {
    auto querystr = std::string(
        "assign a; variable v1, v2; Select a pattern a (v1, \"v1+v2\")");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"11"});
  }

  SECTION ("Test exact pattern match - no results") {
    auto querystr = std::string(
        "assign a; variable v1, v2; Select a pattern a (v1, \"v1+v3\")");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION ("Test if pattern match on multiple control vars") {
    auto querystr = std::string(
        "if ifs; variable v1; Select ifs pattern ifs (\"v2\", _, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"32", "35"});
  }

  SECTION ("Test if pattern match on multiple control vars and single vars") {
    auto querystr = std::string(
        "if ifs; variable v1; Select ifs pattern ifs (\"v1\", _, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"17", "20", "23", "26", "29", "32", "35"});
  }

  SECTION ("Test pattern match - select all if control vars") {
    auto querystr =
        std::string("if ifs; variable v; Select v pattern ifs (v, _, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"v1", "v2"});
  }

  SECTION ("Test pattern match - select all if stmts") {
    auto querystr =
        std::string("if ifs; variable v; Select ifs pattern ifs (_, _, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"17", "20", "23", "26", "29", "32", "35"});
  }

  SECTION (
      "Test while pattern match on multiple control vars and single vars") {
    auto querystr =
        std::string("while w; variable v1; Select w pattern w (\"v1\", _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"9"});
  }

  SECTION ("Test pattern match - select all while control vars") {
    auto querystr =
        std::string("while w; variable v1; Select v1 pattern w (v1, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"v1"});
  }

  SECTION ("Test pattern match - select all while stmts") {
    auto querystr =
        std::string("while w; variable v1; Select w pattern w (v1, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"9"});
  }

  delete pkb;
  delete qm;
}
