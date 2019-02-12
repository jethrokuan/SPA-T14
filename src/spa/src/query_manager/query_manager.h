#pragma once
#include <string>
#include "program_knowledge_base/pkb.h"
#include "query_evaluator/pql/pql.h"

class QueryManager {
 private:
  PKB* pkb;

 public:
  QueryManager(PKB* pkb) : pkb(pkb){};
  void makeQuery(QE::Query* query);
};