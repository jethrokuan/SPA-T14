#include "spa_manager/spa_manager.h"
#include <sstream>
#include "program_knowledge_base/pkb.h"
#include "query_evaluator/core/exceptions.h"
#include "query_evaluator/pql/pql.h"
#include "query_evaluator/query_evaluator.h"
#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"

using namespace Simple;
using namespace QE;

void SPAManager::loadSimpleSource(std::string& source_code) {
  // Lexer expects a stream
  std::istringstream iss(source_code);

  // Complete parse of code to AST
  Simple::Lexer lexer = Simple::Lexer(iss);
  lexer.parse();

  // Load AST into PKB
  Simple::Parser parser = Simple::Parser(lexer.tokens);
  auto proc = parser.parse();

  // Store PKB variable in class for querying laster
  pkb = new PKB(proc);
}

void SPAManager::query(std::string& pql_query) {
  auto qe = QueryEvaluator();
  auto query = qe.makePqlQuery(pql_query);

  // Make the query call to a QueryManager1
}

SPAManager::~SPAManager() { delete pkb; }