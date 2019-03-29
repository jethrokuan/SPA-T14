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
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"17", "6", "7"});
  }

  SECTION ("Select v such that Parent (9, s) and Uses (s, v)") {
    querystr += std::string("Select v such that Parent (9, s) and Uses (s, v)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{});
  }

  SECTION ("Select s1 such that Parent* (s, s1) and Uses (s, \"x\")") {
    querystr +=
        std::string("Select s1 such that Parent* (s, s1) and Uses (s, \"x\")");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"13", "14", "15", "16", "17", "18"});
  }

  SECTION ("Select v1 such that Follows* (ifs, a) pattern a (v, _\"x\"_)") {
    querystr += std::string(
        "Select v1 such that Follows* (ifs, a) pattern a (v, _\"x\"_)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"a", "b", "c", "k", "x"});
  }

  SECTION ("Select p such that Calls* (p, q) and Modifies (p, \"x\")") {
    querystr +=
        std::string("Select p such that Calls* (p, q) and Modifies (p, \"x\")");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"main", "swap"});
  }

  SECTION ("Select s such that Follows* (s, s1) and Next (14, s1)") {
    querystr +=
        std::string("Select s such that Follows* (s, s1) and Next (14, s1)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"14"});
  }

  SECTION ("Select s such that Follows (s, s1) and Next* (14, s1)") {
    querystr +=
        std::string("Select s such that Follows (s, s1) and Next* (14, s1)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"12", "14", "19", "20"});
  }

  SECTION ("Select s such that Parent (s, n) with n = c.value") {
    querystr +=
        std::string("Select s such that Parent (s, n) with n = c.value");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"12", "13"});
  }

  SECTION ("Select <cl, v> such that Modifies (w, v)") {
    querystr += std::string("Select <cl, v> such that Modifies (w, v)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"4 a", "4 c", "4 x", "9 a", "9 c", "9 x"});
  }

  SECTION ("Select <v, w.stmt#> such that Parent (w, s) and Uses (s, v)") {
    querystr += std::string(
        "Select <v, w.stmt#> such that Parent (w, s) and Uses (s, v)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"a 13", "a 16", "b 16", "c 16", "x 13"});
  }

  SECTION (
      "Select a pattern w (v, _) such that Next (w, a) and Modifies (a, v)") {
    querystr += std::string(
        "Select a pattern w (v, _) such that Next (w, a) and Modifies (a, v)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) ==
            std::vector<std::string>{"14", "17", "19"});
  }

  delete pkb;
  delete qm;
}
