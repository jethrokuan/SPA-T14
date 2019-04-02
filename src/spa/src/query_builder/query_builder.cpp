#include "query_builder/query_builder.h"
#include <iostream>
#include "query_builder/core/query_lexer.h"
#include "query_builder/core/query_parser.h"
#include "query_builder/core/query_validator.h"

using namespace QE;

Query QueryBuilder::makePqlQuery(std::string& pql_query_string) {
  // Debug printing of the query receives
  // std::cout << pql_query_string << std::endl;

  // Get the query object from the query
  // getQuery can throw PQLParseException / PQLTokenizeException
  QueryLexer lexer = QueryLexer(pql_query_string);
  auto tokens = lexer.lex();
  QueryParser parser = QueryParser(tokens);
  Query query = parser.parse();
  // Check for semantic errors - can throw PQLValidationException
  QueryValidator validator = QueryValidator();
  validator.validateQuery(query);

  // At this point - the query is constructed and valid
  // TODO: Optimization: Short-circuit queries that obviously return no result

  // Caller is responsible for de-allocating query
  return query;
}
