#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/core/exceptions.h"
#include "query_evaluator/pql/pql.h"
#include "query_evaluator/query_evaluator.h"
#include "query_manager/query_manager.h"
#include "simple_parser/interface.h"

using namespace QE;

TEST_CASE ("Test Query Manager functionality - simple_1") {
  auto ast = Simple::SimpleInterface::getAstFromFile(
      "tests/simple_source/simple_1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryManager(pkb);
  auto qe = QueryEvaluator();

  SECTION ("Test select all assignments") {
    auto querystr = std::string("assign a; Select a;");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"1"});
  }

  SECTION ("Test select all prints") {
    auto querystr = std::string("print p; Select p;");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"4", "3"});
  }

  SECTION ("Test select all whiles") {
    auto querystr = std::string("while w; Select w;");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"2"});
  }

  SECTION ("Test select all reads") {
    auto querystr = std::string("read r; Select r;");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"5"});
  }

  SECTION ("Test select all procedures") {
    auto querystr = std::string("procedure p; Select p;");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"main"});
  }

  SECTION ("Test select all variables") {
    auto querystr = std::string("variable v; Select v;");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  SECTION ("Test select all variables with true boolean such_that Follows") {
    auto querystr = std::string("variable v; Select v such that Follows(1,2)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  SECTION ("Test select all variables with true boolean such_that Follows*") {
    auto querystr = std::string("variable v; Select v such that Follows*(1,2)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  SECTION ("Test select all variables with true boolean such_that Parent") {
    auto querystr = std::string("variable v; Select v such that Parent(2,3)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  SECTION ("Test select all variables with true boolean such_that Parent*") {
    auto querystr = std::string("variable v; Select v such that Parent*(2,3)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  SECTION ("Test select all variables with true boolean such_that Modifies") {
    auto querystr =
        std::string("variable v; Select v such that Modifies(1,\"i\")");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  SECTION (
      "Test select all variables with true boolean such_that Modifies read") {
    auto querystr =
        std::string("variable v; Select v such that Modifies(5,\"i\")");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  SECTION ("Test select all variables with true boolean such_that Uses while") {
    auto querystr = std::string("variable v; Select v such that Uses(2,\"i\")");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  SECTION ("Test select all variables with true boolean such_that Uses print") {
    auto querystr = std::string("variable v; Select v such that Uses(3,\"j\")");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  SECTION ("Test select all variables with false boolean such_that") {
    auto querystr = std::string("variable v; Select v such that Follows(1,3)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{});
  }

  SECTION (
      "Test 2 variable declare + select all variables with true boolean "
      "such_that") {
    auto querystr =
        std::string("variable v1, v2; Select v1 such that Follows(1,2)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  SECTION (
      "Test 2 variable declare + select second all variables with true boolean "
      "such_that") {
    auto querystr =
        std::string("variable v1, v2; Select v2 such that Follows(1,2)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"j", "i"});
  }

  delete pkb;
  delete qm;
}
