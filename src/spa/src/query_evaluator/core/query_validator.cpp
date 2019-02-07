#include "query_evaluator/core/query_validator.h"
#include "query_evaluator/core/exceptions.h"
#include "query_evaluator/pql/declaration.h"

using namespace QE;

void QueryValidator::validateQuery(const Query& query) {
  validatePatternVariableAsAssign(query);
  validateModifyUsesNoFirstArgUnderscore(query);
}

void QueryValidator::validatePatternVariableAsAssign(const Query& query) {
  if (query.pattern == nullptr) {
    return;
  }

  // Search the available declarations for the pattern synonym
  // The synonym must be an assignment synonym
  auto found_declaration = std::find_if(
      query.declarations->begin(), query.declarations->end(), [&](auto decl) {
        return decl.getDesignEntity() == DesignEntity::ASSIGN &&
               decl.getSynonym().synonym == query.pattern->getSynonym().synonym;
      });

  if (found_declaration == query.declarations->end()) {
    throw PQLValidationException(
        "Semantic Error: cannot match synonym " +
        query.pattern->getSynonym().synonym +
        " to an assignment synonym in list of declarations");
  }
}

void QueryValidator::validateModifyUsesNoFirstArgUnderscore(
    const Query& query) {}
