#include "query_evaluator/core/query_validator.h"
#include <variant>
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
    const Query& query) {
  if (query.such_that == nullptr) {
    return;
  }

  // Early return if not modifies or uses
  if (query.such_that->getRelation() != Relation::UsesS &&
      query.such_that->getRelation() != Relation::ModifiesS) {
    return;
  }

  // Error if first arg is underscore otherwise
  auto such_that_firstarg = query.such_that->getFirstArg();
  // Check one level deeper in first arg to check for underscore
  std::visit(
      [](auto&& arg) {
        if (std::holds_alternative<Underscore>(arg)) {
          throw PQLValidationException(
              "First argument of Modifies/Uses cannot be an underscore - "
              "ambiguous");
        }
      },
      such_that_firstarg);
}
