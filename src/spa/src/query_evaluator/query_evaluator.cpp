#include "query_evaluator/query_evaluator.h"
#include <iostream>
#include "query_evaluator/core/query_preprocessor.h"

using namespace QE;

std::unique_ptr<QueryResult> QueryEvaluator::makePqlQuery(
    std::string& pql_query_string) {
  std::cout << pql_query_string << std::endl;
  auto qp = QueryPreprocessor();
  auto query = qp.getQuery(pql_query_string);
  // The query needs to be fed into the PKB to get the QueryResult
  return std::unique_ptr<QueryResult>(nullptr);
}
