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

TEST_CASE ("Test Query Executor multiple select") {
  auto ast = Simple::SimpleInterface::getAstFromFile(
      "tests/simple_source/simple_1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test select all assignments") {
    auto querystr = std::string("assign a; variable v; Select a;");
    auto query = qe.makePqlQuery(querystr);
    query->selected_declarations->push_back(&query->declarations->at(1));
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"1 i", "1 j"});
  }

  SECTION ("Test select all <a, v>") {
    auto querystr = std::string("assign a; variable v; Select v;");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"i", "j"});
  }

  SECTION ("Test select all variables") {
    auto querystr = std::string("assign a; variable v; Select a;");
    auto query = qe.makePqlQuery(querystr);
    query->selected_declarations->insert(query->selected_declarations->begin(),
                                         &query->declarations->at(1));
    REQUIRE(qm->makeQuery(query) == std::vector<std::string>{"i 1", "j 1"});
  }
}