#pragma once
#include <string>
#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/pql/pql.h"

using namespace PKB;

class QueryManager {
 private:
  PKBManager* pkb;
  //! Gets the result of a select query (everything from a design entity)
  std::vector<std::string> getSelect(QE::DesignEntity);

 public:
  QueryManager(PKBManager* pkb) : pkb(pkb){};
  void makeQuery(QE::Query* query);
};