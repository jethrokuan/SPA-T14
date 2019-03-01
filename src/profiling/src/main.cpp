
#include <fstream>
#include <iostream>
#include <vector>

#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/core/exceptions.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"
#include "query_executor/query_executor.h"
#include "simple_parser/interface.h"

int main(void) {
  // Store PKB variable in class for querying later
  auto ast =
      Simple::SimpleInterface::getAstFromFile("tests/system_tests/src/2.txt");
  auto pkb = new PKBManager(ast);
  auto qm = new QueryExecutor(pkb);
  auto qe = QueryBuilder();
  auto querystr = std::string("assign a; Select a;");
  auto query = qe.makePqlQuery(querystr);
  std::cout << "Profiling run complete.\n";
}