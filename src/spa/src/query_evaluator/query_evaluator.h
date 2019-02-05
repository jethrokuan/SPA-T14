#pragma once
#include <memory>  // future use of smart pointers
#include <string>
#include "query_evaluator/pql/pql.h"

//! Contains classes, methods and functions for the PQL Query Engine (QE)
namespace QE {
//! Stub class that represents the result of a Query passed to the PKB
class QueryResult {
  // Stub class
};

/*!
 *
 *  \brief Contains main API for the PQL Query Engine (QE).
 *         Expects a PQL query string, and calls the PKB with a Query object.
 *         Returns a QueryResult.
 *
 */
class QueryEvaluator {
 public:
  //! Takes a PQL query string, calls PKB, returns the QueryResult
  std::unique_ptr<QueryResult> makePqlQuery(std::string&);
};
}  // namespace QE
