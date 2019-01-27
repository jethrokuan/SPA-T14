#include "query_evaluator/query_evaluator.h"
#include <iostream>

using namespace QE;

std::unique_ptr<QueryResult> QueryEvaluator::makePqlQuery(
    std::string &pql_query_string) {
  std::cout << pql_query_string << std::endl;
  return std::unique_ptr<QueryResult>(nullptr);
}
