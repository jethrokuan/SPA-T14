#pragma once
#include <memory>  // future use of smart pointers
#include <string>
#include <vector>

#include "query_builder/pql/query.h"

namespace QE {

//! Utility class for validating the semantics of Query objects after parsing
class QueryValidator {
 private:
  //! Checks that no two synonyms have the same name
  void validateNoIdenticalSynonyms(const Query&);
  //! Checks neither Modify nor Uses has _ as the first argument (ambiguous)
  void validateModifyUsesNoFirstArgUnderscore(const Query&);
  //! Checks that all the synonyms in the such_that clause are declared
  void validateSuchThatSynonymsAreDeclared(const Query&);
  //! Specializes Modifies/Uses to (Modifies)|Uses(P|S)
  void validateSpecializeModifiesUses(Query&);
  //! Check that all such-that clauses have correct Ref types
  void validateSuchThatRefTypes(const Query&);
  //! Check that all pattern clauses have correct Ref types
  void validatePatternRefTypes(const Query&);
  //! Checks that all synonym arguments have the correct relation types for st
  void validateSuchThatSynonymTypes(const Query&);
  //! Checks that the synonym in a pattern clause is declarated as assign
  void validatePatternVariableAsAssign(const Query&);
  //! Check that if the first argument to pattern is a synonym, must be a var
  void validatePatternFirstArgSynonymIsVariable(const Query&);

 public:
  //! \brief Checks the semantics in the Query, throws
  //! PQLValidationException if an error is detected
  void validateQuery(Query&);
};

}  // namespace QE