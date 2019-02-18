#include "spa_manager/spa_manager.h"
#include <sstream>
#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/core/exceptions.h"
#include "query_evaluator/pql/pql.h"
#include "query_evaluator/query_evaluator.h"
#include "query_manager/query_manager.h"
#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"

using namespace PKB;
using namespace Simple;
using namespace QE;

void SPAManager::loadSimpleSource(std::ifstream& source_stream) {
  // Complete parse of code to AST
  Simple::Lexer lexer = Simple::Lexer(source_stream);
  lexer.parse();

  // Load AST into PKB
  Simple::Parser parser = Simple::Parser(lexer.tokens);
  auto proc = parser.parse();

  // Store PKB variable in class for querying laster
  pkb = new PKBManager(proc);
  qm = new QueryManager(pkb);
}

void SPAManager::query(std::string& pql_query) {
  auto qe = QueryEvaluator();
  auto query = qe.makePqlQuery(pql_query);
  // Stub call - no result yet
  qm->makeQuery(query);
}

SPAManager::~SPAManager() {
  delete pkb;
  delete qm;
}