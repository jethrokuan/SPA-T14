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

TEST_CASE ("Test Query Executor FollowsT functionality - 1.txt") {
  auto ast =
      Simple::SimpleInterface::getAstFromFile("tests/system_tests/src/1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION (
      "Test select variable not constrained by such-that, where such-that is "
      "false (Parent)") {
    auto querystr =
        std::string("assign a; while w; Select w such that Parent(a, 24)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION (
      "Test select variable not constrained by such-that, where such-that is "
      "true (Parent)") {
    auto querystr =
        std::string("if ifs; while w; Select w such that Parent(ifs, 18)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"9"});
  }

  SECTION (
      "Test select variable not constrained by such-that, where such-that is "
      "false (Follows)") {
    auto querystr =
        std::string("read rd; assign a; Select a such that Follows(rd, 25)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION (
      "Test select variable not constrained by such-that, where such-that is "
      "true (Follows)") {
    auto querystr =
        std::string("read rd; assign a; Select a such that Follows(rd, 2)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "12", "13", "14", "15", "16",
                                     "21", "22"});
  }

  SECTION (
      "Test select variable not constrained by such-that, where such-that is "
      "Uses(w, _) that returns results") {
    auto querystr =
        std::string("while w; variable v; Select v such that Uses(w, _)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"v", "v1", "v2", "v3", "v4", "v5"});
  }

  SECTION (
      "Test select variable that is constrained by pattern, and one synonym in "
      "the pattern is constrained by the such-that (Uses)") {
    auto querystr = std::string(
        "assign a, a1; variable v; Select a such that Uses(a1, v) pattern a(v, "
        "_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "12", "13", "14", "15", "16",
                                     "21", "22"});
  }

  SECTION (
      "Test select variable that is constrained by pattern, and both synonyms "
      "in "
      "the pattern are constrained by the such-that (Uses)") {
    auto querystr = std::string(
        "assign a, a1; variable v; Select a such that Uses(a, v) pattern a(v, "
        "_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "16", "22"});
  }

  SECTION (
      "Test select variable that is constrained by pattern, and one synonym in "
      "the pattern is constrained by the such-that (Modifies)") {
    auto querystr = std::string(
        "assign a, a1; variable v; Select a such that Modifies(a1, v) pattern "
        "a(v, "
        "_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"10", "11", "12", "13", "14", "15", "16",
                                     "21", "22"});
  }

  SECTION (
      "Test select variable that is constrained by pattern with "
      "DoubleUnderscore RHS, and one synonym in "
      "the pattern is constrained by the such-that (Uses)") {
    auto querystr = std::string(
        "assign a, a1; variable v; Select a such that Uses(a1, v) pattern a(v, "
        "_\"v\"_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"10"});
  }
}