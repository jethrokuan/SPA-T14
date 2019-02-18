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

TEST_CASE ("Test Query Manager stub functionality") {
  auto ast = Simple::SimpleInterface::getAstFromFile(
      "tests/simple_source/simple_1.txt");

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(ast);
  auto qm = new QueryManager(pkb);

  auto querystr = std::string("assign a; Select a;");
  auto qe = QueryEvaluator();
  auto query = qe.makePqlQuery(querystr);
  REQUIRE_NOTHROW(qm->makeQuery(query));

  delete pkb;
  delete qm;
}
