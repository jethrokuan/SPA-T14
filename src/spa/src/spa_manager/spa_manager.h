#pragma once
#include <string>
#include "program_knowledge_base/pkb.h"

class SPAManager {
 private:
  PKB* pkb;

 public:
  void loadSimpleSource(std::string& source_code);
  void query(std::string& pql_query);
  ~SPAManager();
};