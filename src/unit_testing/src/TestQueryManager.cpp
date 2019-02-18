#include "catch.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/core/exceptions.h"
#include "query_evaluator/pql/pql.h"
#include "query_evaluator/query_evaluator.h"
#include "query_manager/query_manager.h"
#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"

using namespace QE;

TEST_CASE ("Test Query Manager stub functionality") {
  std::string filename = "tests/simple_source/simple_1.txt";
  std::ifstream input(filename);
  Simple::Lexer lexer = Simple::Lexer(input);
  lexer.parse();

  // Load AST into PKB
  Simple::Parser parser = Simple::Parser(lexer.tokens);
  auto proc = parser.parse();

  // Store PKB variable in class for querying later
  auto pkb = new PKBManager(proc);
  auto qm = new QueryManager(pkb);

  auto querystr = std::string("assign a; Select a;");
  auto qe = QueryEvaluator();
  auto query = qe.makePqlQuery(querystr);
  REQUIRE_NOTHROW(qm->makeQuery(query));

  delete pkb;
  delete qm;
}