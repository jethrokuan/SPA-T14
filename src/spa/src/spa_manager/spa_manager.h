#pragma once
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_executor/query_executor.h"

using namespace PKB;

class SPAManager {
 private:
  PKBManager* pkb;
  QueryExecutor* qm;

 public:
  void loadSimpleSource(std::string filename);
  std::vector<std::string> query(std::string& pql_query);
  ~SPAManager();
};
