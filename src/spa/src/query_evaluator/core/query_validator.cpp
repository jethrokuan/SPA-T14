#include "query_evaluator/core/query_validator.h"
#include <variant>
#include "query_evaluator/core/exceptions.h"
#include "query_evaluator/pql/declaration.h"

using namespace QE;

void QueryValidator::validateQuery(const Query& query) {
  // Do not change the order of these calls - there are dependencies to reduce
  // double-checking of conditions
  validatePatternVariableAsAssign(query);
  validateModifyUsesNoFirstArgUnderscore(query);
  validateSuchThatSynonyms(query);
  validateSynonymTypes(query);
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

void QueryValidator::validateSuchThatSynonyms(const Query& query) {
  if (query.such_that == nullptr) {
    return;
  }
  // Error if first arg is underscore otherwise
  auto such_that_firstarg = query.such_that->getFirstArg();
  auto such_that_secondarg = query.such_that->getSecondArg();

  // Lack of code reuse here to allow for more expressive error messages
  // Can be changed in the future: TODO
  std::visit(
      [&](auto&& arg) {
        if (std::holds_alternative<Synonym>(arg)) {
          auto found_declaration =
              std::find_if(query.declarations->begin(),
                           query.declarations->end(), [&](auto decl) {
                             return decl.getSynonym().synonym ==
                                    std::get<Synonym>(arg).synonym;
                           });
          if (found_declaration == query.declarations->end()) {
            throw PQLValidationException(
                "Cannot find a matching declaration for synonym " +
                std::get<Synonym>(arg).synonym + " in first argument of " +
                getStringFromRelation(query.such_that->getRelation()));
          }
        }
      },
      such_that_firstarg);
  std::visit(
      [&](auto&& arg) {
        if (std::holds_alternative<Synonym>(arg)) {
          auto found_declaration =
              std::find_if(query.declarations->begin(),
                           query.declarations->end(), [&](auto decl) {
                             return decl.getSynonym().synonym ==
                                    std::get<Synonym>(arg).synonym;
                           });
          if (found_declaration == query.declarations->end()) {
            throw PQLValidationException(
                "Cannot find a matching declaration for synonym " +
                std::get<Synonym>(arg).synonym + " in second argument of " +
                getStringFromRelation(query.such_that->getRelation()));
          }
        }
      },
      such_that_secondarg);
}

void QueryValidator::validateSynonymTypes(const Query& query) {
  if (query.such_that == nullptr) {
    return;
  }
  // Idea here is to check the synonym's design entity type
  // against the list of allowed design entity types allowed for
  // each argument of the relation in this query
  auto relation = query.such_that->getRelation();
  auto argSynonymTypes = getArgSynonymTypesFromRelation(relation);

  // Check both arguments - must be correct type
  auto first_arg_iter = std::visit(
      [&](auto&& arg) {
        if (std::holds_alternative<Synonym>(arg)) {
          return std::find_if(query.declarations->begin(),
                              query.declarations->end(), [&](auto decl) {
                                return decl.getSynonym().synonym ==
                                       std::get<Synonym>(arg).synonym;
                              });
        } else {
          return query.declarations->end();
        }
      },
      query.such_that->getFirstArg());
  if (first_arg_iter != query.declarations->end()) {
    auto found_declaration = *first_arg_iter;
    if (std::find(argSynonymTypes.first.begin(), argSynonymTypes.first.end(),
                  found_declaration.getDesignEntity()) ==
        argSynonymTypes.first.end()) {
      throw PQLValidationException(
          "Cannot match given design entity type: " +
          getDesignEntityString(found_declaration.getDesignEntity()) +
          " with allowed list of design entities: " +
          getDesignEntityVectorString(argSynonymTypes.first) +
          " during parse of such_that relation: " +
          getStringFromRelation(relation) + "'s first argument");
    }
  }

  auto second_arg_iter = std::visit(
      [&](auto&& arg) {
        if (std::holds_alternative<Synonym>(arg)) {
          return std::find_if(query.declarations->begin(),
                              query.declarations->end(), [&](auto decl) {
                                return decl.getSynonym().synonym ==
                                       std::get<Synonym>(arg).synonym;
                              });
        } else {
          return query.declarations->end();
        }
      },
      query.such_that->getSecondArg());
  if (second_arg_iter != query.declarations->end()) {
    auto found_declaration = *second_arg_iter;
    if (std::find(argSynonymTypes.second.begin(), argSynonymTypes.second.end(),
                  found_declaration.getDesignEntity()) ==
        argSynonymTypes.second.end()) {
      throw PQLValidationException(
          "Cannot match given design entity type: " +
          getDesignEntityString(found_declaration.getDesignEntity()) +
          " with allowed list of design entities: " +
          getDesignEntityVectorString(argSynonymTypes.second) +
          " during parse of such_that relation: " +
          getStringFromRelation(relation) + "'s second argument");
    }
  }
}
