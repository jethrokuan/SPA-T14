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

TEST_CASE ("Test Query Executor on BOOLEAN queries") {
  auto ast = Simple::SimpleInterface::getAstFromFile(
      "tests/simple_source/simple_1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();

  SECTION ("Test BOOLEAN without any declarations") {
    auto querystr = std::string("Select BOOLEAN such that Follows*(1, 5)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test BOOLEAN without any declarations (FALSE)") {
    auto querystr = std::string("Select BOOLEAN such that Follows*(1, 1)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"FALSE"});
  }

  SECTION ("Test BOOLEAN with one-var such-that clause") {
    auto querystr =
        std::string("stmt s; Select BOOLEAN such that Follows*(s, 5)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test BOOLEAN with one-var such-that clause (FALSE)") {
    auto querystr =
        std::string("stmt s; Select BOOLEAN such that Follows*(s, 1)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"FALSE"});
  }

  SECTION ("Test BOOLEAN with multi-var such-that clause") {
    auto querystr =
        std::string("stmt s, s1; Select BOOLEAN such that Follows*(s, s1)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test BOOLEAN with multi-var such-that clause (FALSE)") {
    auto querystr =
        std::string("stmt s, s1; Select BOOLEAN such that Follows*(s, s)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"FALSE"});
  }

  SECTION ("Test BOOLEAN with multi-var such-that clause") {
    auto querystr =
        std::string("stmt s, s1; Select BOOLEAN such that Next(2, 3)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"TRUE"});
  }

  SECTION ("Test BOOLEAN with multi-var such-that clause (FALSE)") {
    auto querystr =
        std::string("stmt s, s1; Select BOOLEAN such that Next(1, 1)");
    auto query = qe.makePqlQuery(querystr);
    REQUIRE(qm->makeQuery(&query) == std::vector<std::string>{"FALSE"});
  }
}
