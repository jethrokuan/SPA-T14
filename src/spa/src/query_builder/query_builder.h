#pragma once
#include <memory>  // future use of smart pointers
#include <string>
#include "query_builder/pql/pql.h"

//! Contains classes, methods and functions for the PQL Query Engine (QE)
namespace QE {
/*!
 *
 *  \brief Contains main API for the PQL Query Engine (QE).
 *         Expects a PQL query string, and creates a Query object for
 *         use by the PKB.
 *
 */
class QueryBuilder {
 public:
  //! \brief Takes a PQL query string and returns a pointer to a Query object
  //! representing it. The caller is responsible for deleting the Query
  //! object
  Query* makePqlQuery(std::string&);
};
}  // namespace QE
