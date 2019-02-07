#include "query_evaluator/query_evaluator.h"
#include <iostream>
#include "query_evaluator/core/query_preprocessor.h"
#include "query_evaluator/core/query_validator.h"

using namespace QE;

std::unique_ptr<QueryResult> QueryEvaluator::makePqlQuery(
    std::string& pql_query_string) {
  // Debug printing of the query receives
  std::cout << pql_query_string << std::endl;

  // Get the query object from the query
  // getQuery can throw PQLParseException / PQLTokenizeException
  auto qp = QueryPreprocessor();
  auto query = qp.getQuery(pql_query_string);

  // Check for semantic errors - can throw PQLValidationException
  auto qv = QueryValidator();
  qv.validateQuery(*query);

  // At this point - the query is constructed and valid
  // TODO: Optimization: Short-circuit queries that obviously return no result

  // The query needs to be fed into the PKB to get the QueryResult
  return std::unique_ptr<QueryResult>(nullptr);
}
