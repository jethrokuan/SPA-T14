#pragma once
#include <string>
#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/pql/pql.h"

using namespace PKB;

class QueryManager {
 private:
  PKBManager* pkb;

 public:
  QueryManager(PKBManager* pkb) : pkb(pkb){};
  void makeQuery(QE::Query* query);
};