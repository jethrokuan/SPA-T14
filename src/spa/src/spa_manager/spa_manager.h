#pragma once
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_manager/query_manager.h"

using namespace PKB;

class SPAManager {
 private:
  PKBManager* pkb;
  QueryManager* qm;

 public:
  void loadSimpleSource(std::ifstream& source_stream);
  std::vector<std::string> query(std::string& pql_query);
  ~SPAManager();
};