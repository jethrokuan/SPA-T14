#include "catch.hpp"

#include <iostream>

#include <vector>

#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/core/exceptions.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"
#include "query_executor/clause_prioritizer.h"
#include "query_executor/query_executor.h"
#include "simple_parser/interface.h"

TEST_CASE ("Test Clause Prioritizer on multiple numbers of clauses") {
  auto ast = Simple::SimpleInterface::getAstFromFile(
      "tests/simple_source/simple_1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qb = QueryBuilder();

  SECTION ("Test clause prioritizer for no clauses") {
    auto querystr = std::string("assign a; variable v; Select a");
    auto query = qb.makePqlQuery(querystr);
    auto clause_prioritizer = ClausePrioritizer(&query);
    REQUIRE(clause_prioritizer.getClauses().size() == 0);
  }

  SECTION ("Test clause prioritizer for 1 clause") {
    auto querystr =
        std::string("assign a; variable v; Select a such that Follows(a, a)");
    auto query = qb.makePqlQuery(querystr);
    auto clause_prioritizer = ClausePrioritizer(&query);
    REQUIRE(clause_prioritizer.getClauses().size() == 1);
  }

  SECTION ("Test clause prioritizer for 3 boolean clauses") {
    auto querystr = std::string(
        "assign a; Select BOOLEAN such that Follows(1, 2) pattern a (\"x\", "
        "\"x\") with "
        "2 = 3");
    auto query = qb.makePqlQuery(querystr);
    auto clause_prioritizer = ClausePrioritizer(&query);
    auto clauses_order = clause_prioritizer.getClauses();
    REQUIRE(
        (std::get<WithCond*>(clauses_order.at(0)) == (query.with_cond->at(0))));
    REQUIRE(
        (std::get<RelCond*>(clauses_order.at(1)) == (query.rel_cond->at(0))));
    REQUIRE(
        (std::get<PatternB*>(clauses_order.at(2)) == (query.patternb->at(0))));
  }
}