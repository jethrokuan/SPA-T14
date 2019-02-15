#pragma once
#include <string>
#include "program_knowledge_base/pkb.h"
#include "query_manager/query_manager.h"

using namespace KB;

class SPAManager {
 private:
  PKB* pkb;
  QueryManager* qm;

 public:
  void loadSimpleSource(std::ifstream& source_stream);
  void query(std::string& pql_query);
  ~SPAManager();
};