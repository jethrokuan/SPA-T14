#pragma once
#include <memory>  // future use of smart pointers
#include <string>
#include "query_evaluator/pql/pql.h"

/*
 * Utility class that takes in a PQL query string and returns a Query object
 * for further processing by the Query system
 *
 */

namespace QE {
//! \brief Calls QueryTokenizer, and parses those tokens into the appropriate
//!        PQL clauses in the Query object
class QueryPreprocessor {
 private:
  void parseDeclarations(Query*, std::vector<std::string>*);
  void parseSelect(Query*, std::vector<std::string>*);
  void parseSuchThat(Query*, std::vector<std::string>*);
  void parsePattern(Query*, std::vector<std::string>*);

  // Utility methods
  static Declaration* findDeclaration(std::vector<Declaration>* declarations,
                                      std::string synonym_to_match);
  static std::optional<StmtOrEntRef> argToStmtOrEntRef(std::string, RefType);
  static std::optional<StmtRef> argToStmtRef(std::string arg);
  static std::optional<EntRef> argToEntRef(std::string arg);
  static std::optional<ExpressionSpec> argToExprSpec(std::string arg);

 public:
  // Given a string containing PQL,
  // returns the Query object representing the query's structure
  Query* getQuery(std::string&);
};
}  // namespace QE