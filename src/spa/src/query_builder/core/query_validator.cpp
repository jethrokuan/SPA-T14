#include "query_builder/core/query_validator.h"
#include <unordered_set>
#include <variant>
#include "query_builder/core/exceptions.h"
#include "query_builder/pql/declaration.h"

using namespace QE;

void QueryValidator::validateQuery(const Query& query) {
  // Do not change the order of these calls - there are dependencies to reduce
  // double-checking of conditions
  validatePatternVariableAsAssign(query);
  validateModifyUsesNoFirstArgUnderscore(query);
  validateSuchThatSynonyms(query);
  validateSynonymTypes(query);
  validateNoIdenticalSynonyms(query);
  validatePatternFirstArgSynonymIsVariable(query);
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
          if (!Declaration::findDeclarationForSynonym(query.declarations,
                                                      std::get<Synonym>(arg))) {
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
          if (!Declaration::findDeclarationForSynonym(query.declarations,
                                                      std::get<Synonym>(arg))) {
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
  auto first_arg_opt = std::visit(
      [&](auto&& arg) -> std::optional<Declaration> {
        if (std::holds_alternative<Synonym>(arg)) {
          return Declaration::findDeclarationForSynonym(query.declarations,
                                                        std::get<Synonym>(arg));
        } else {
          return std::nullopt;
        }
      },
      query.such_that->getFirstArg());
  if (first_arg_opt) {
    auto found_declaration = *first_arg_opt;
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

  auto second_arg_opt = std::visit(
      [&](auto&& arg) -> std::optional<Declaration> {
        if (std::holds_alternative<Synonym>(arg)) {
          return Declaration::findDeclarationForSynonym(query.declarations,
                                                        std::get<Synonym>(arg));
        } else {
          return std::nullopt;
        }
      },
      query.such_that->getSecondArg());
  if (second_arg_opt) {
    auto found_declaration = *second_arg_opt;
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

void QueryValidator::validateNoIdenticalSynonyms(const Query& query) {
  // Look through all declarations for identical synonyms
  std::unordered_set<std::string> unique_synonyms;

  for (auto decl : *(query.declarations)) {
    auto synonym = decl.getSynonym().synonym;
    const bool is_in = unique_synonyms.find(synonym) != unique_synonyms.end();
    if (is_in) {
      throw PQLValidationException(
          "Found at least 2 synonyms with the same name: " + synonym);
    } else {
      unique_synonyms.insert(synonym);
    }
  }
}

void QueryValidator::validatePatternFirstArgSynonymIsVariable(
    const Query& query) {
  if (query.pattern == nullptr) {
    return;
  }
  auto first_arg = query.pattern->getFirstArg();
  if (auto first_arg_syn = std::get_if<Synonym>(&first_arg)) {
    // Search the available declarations for the pattern synonym
    // The synonym must be an assignment synonym
    auto found_declaration = std::find_if(
        query.declarations->begin(), query.declarations->end(), [&](auto decl) {
          return decl.getDesignEntity() == DesignEntity::VARIABLE &&
                 decl.getSynonym().synonym == first_arg_syn->synonym;
        });

    if (found_declaration == query.declarations->end()) {
      throw PQLValidationException(
          "Semantic Error: cannot match synonym " + first_arg_syn->synonym +
          " to an variable synonym in list of declarations");
    }
  }
}
