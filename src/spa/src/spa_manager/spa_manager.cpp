#include "spa_manager/spa_manager.h"
#include <sstream>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/core/exceptions.h"
#include "query_evaluator/pql/pql.h"
#include "query_evaluator/query_evaluator.h"
#include "query_manager/query_manager.h"
#include "simple_parser/interface.h"

using namespace PKB;
using namespace Simple;
using namespace QE;

void SPAManager::loadSimpleSource(std::string filename) {
  // Complete parse of code to AST
  auto ast = Simple::SimpleInterface::getAstFromFile(filename);

  // Store PKB variable in class for querying laster
  pkb = new PKBManager(ast);
  qm = new QueryManager(pkb);
}

std::vector<std::string> SPAManager::query(std::string& pql_query) {
  auto qe = QueryEvaluator();
  auto query = qe.makePqlQuery(pql_query);
  auto query_results = qm->makeQuery(query);
  return query_results;
}

SPAManager::~SPAManager() {
  delete pkb;
  delete qm;
}
