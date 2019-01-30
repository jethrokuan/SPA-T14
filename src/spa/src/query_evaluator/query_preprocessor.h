#pragma once
#include <memory>  // future use of smart pointers
#include <string>
#include "query_evaluator/pql.h"

/*
 * Utility class that takes in a PQL query string and returns a Query object
 * for further processing by the Query system
 *
 */

namespace QE {
class QueryPreprocessor {
 private:
  void parseDeclarations(Query*, std::vector<std::string>*);
  void parseSelect(Query*, std::vector<std::string>*);
  void parseSuchThat(Query*, std::vector<std::string>*);

  // Utility methods
  static Declaration* findDeclaration(std::vector<Declaration>* declarations,
                                      std::string synonym_to_match);
  static void argToStmtEntityRef(std::vector<Declaration>* declarations,
                                         std::string arg);

 public:
  // Given a string containing PQL,
  // returns the Query object representing the query's structure
  Query* getQuery(std::string&);
};
}  // namespace QE