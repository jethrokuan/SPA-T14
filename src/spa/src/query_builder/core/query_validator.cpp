#include "query_builder/core/query_validator.h"
#include <unordered_set>
#include <variant>
#include "query_builder/core/exceptions.h"
#include "query_builder/pql/declaration.h"

using namespace QE;

void QueryValidator::validateQuery(Query& query) {
  // Do not change the order of these calls - there are dependencies to reduce
  // double-checking of conditions
  validatePatternVariableAsAssign(query);
  validateModifyUsesNoFirstArgUnderscore(query);
  validateSuchThatSynonymsAreDeclared(query);
  validateSpecializeModifiesUses(query);
  validateSynonymTypes(query);
  validateNoIdenticalSynonyms(query);
  validatePatternFirstArgSynonymIsVariable(query);
}
void QueryValidator::validatePatternVariableAsAssign(const Query& query) {
  for (auto pattern : *(query.patternb)) {
    // Search the available declarations for the pattern synonym
    // The synonym must be an assignment synonym
    auto found_declaration = std::find_if(
        query.declarations->begin(), query.declarations->end(), [&](auto decl) {
          return decl.getDesignEntity() == DesignEntity::ASSIGN &&
                 decl.getSynonym().synonym == pattern->getSynonym().synonym;
        });

    if (found_declaration == query.declarations->end()) {
      throw PQLValidationException(
          "Semantic Error: cannot match synonym " +
          pattern->getSynonym().synonym +
          " to an assignment synonym in list of declarations");
    }
  }
}

void QueryValidator::validateModifyUsesNoFirstArgUnderscore(
    const Query& query) {
  for (auto such_that : *(query.rel_cond)) {
    // Early return if not modifies or uses
    if (such_that->relation != Relation::Uses &&
        such_that->relation != Relation::Modifies) {
      return;
    }

    // Error if first arg is underscore otherwise
    auto such_that_firstarg = such_that->arg1;
    // Check one level deeper in first arg to check for underscore
    if (std::holds_alternative<Underscore>(such_that_firstarg)) {
      throw PQLValidationException(
          "First argument of Modifies/Uses cannot be an underscore - "
          "ambiguous");
    }
  }
}

void QueryValidator::validateSuchThatSynonymsAreDeclared(const Query& query) {
  for (auto such_that : *(query.rel_cond)) {
    // Error if first arg is underscore otherwise
    auto such_that_firstarg = such_that->arg1;
    auto such_that_secondarg = such_that->arg2;

    // Lack of code reuse here to allow for more expressive error messages
    // Can be changed in the future: TODO
    if (auto arg = std::get_if<Synonym>(&such_that_firstarg)) {
      if (!Declaration::findDeclarationForSynonym(query.declarations, *arg)) {
        throw PQLValidationException(
            "Cannot find a matching declaration for synonym " + arg->synonym +
            " in first argument of " +
            getStringFromRelation(such_that->relation));
      }
    }
    if (auto arg = std::get_if<Synonym>(&such_that_secondarg)) {
      if (!Declaration::findDeclarationForSynonym(query.declarations, *arg)) {
        throw PQLValidationException(
            "Cannot find a matching declaration for synonym " + arg->synonym +
            " in second argument of " +
            getStringFromRelation(such_that->relation));
      }
    }
  }
}

void QueryValidator::validateSpecializeModifiesUses(Query& query) {
  for (auto such_that : *(query.rel_cond)) {
    // Early return if not modifies or uses
    if (such_that->relation != Relation::Uses &&
        such_that->relation != Relation::Modifies) {
      return;
    }

    auto such_that_firstarg = such_that->arg1;
    // QuoteIdent ==> first argument entref ==> must be a procedure version
    if (std::holds_alternative<QuoteIdent>(such_that_firstarg)) {
      such_that->relation = such_that->relation == Relation::Uses
                                ? Relation::UsesP
                                : Relation::ModifiesP;
    } else if (std::holds_alternative<StatementNumber>(such_that_firstarg)) {
      // A statementnumber ==> this is a statement relation (*S)
      such_that->relation = such_that->relation == Relation::Uses
                                ? Relation::UsesS
                                : Relation::ModifiesS;
    } else if (auto syn = std::get_if<Synonym>(&such_that_firstarg)) {
      // If it's a synonym - need to check if stmt synonym or proc
      // We can check for value() since previous validator has checked existence
      auto decl =
          Declaration::findDeclarationForSynonym(query.declarations, *syn)
              .value();
      if (decl.getDesignEntity() == DesignEntity::PROCEDURE) {
        such_that->relation = such_that->relation == Relation::Uses
                                  ? Relation::UsesP
                                  : Relation::ModifiesP;
      } else {
        such_that->relation = such_that->relation == Relation::Uses
                                  ? Relation::UsesS
                                  : Relation::ModifiesS;
      }
    }
    // Underscore is handled separately
  }
}

void QueryValidator::validateSynonymTypes(const Query& query) {
  for (auto such_that : *(query.rel_cond)) {
    // Idea here is to check the synonym's design entity type
    // against the list of allowed design entity types allowed for
    // each argument of the relation in this query
    auto relation = such_that->relation;
    auto argSynonymTypes = getArgSynonymTypesFromRelation(relation);

    // Check both arguments - must be correct type
    if (auto arg = std::get_if<Synonym>(&such_that->arg1)) {
      auto found_declaration =
          Declaration::findDeclarationForSynonym(query.declarations, *arg);
      if (std::find(argSynonymTypes.first.begin(), argSynonymTypes.first.end(),
                    found_declaration->getDesignEntity()) ==
          argSynonymTypes.first.end()) {
        throw PQLValidationException(
            "Cannot match given design entity type: " +
            getDesignEntityString(found_declaration->getDesignEntity()) +
            " with allowed list of design entities: " +
            getDesignEntityVectorString(argSynonymTypes.first) +
            " during parse of such_that relation: " +
            getStringFromRelation(relation) + "'s first argument");
      }
    }

    if (auto arg = std::get_if<Synonym>(&such_that->arg2)) {
      auto found_declaration =
          Declaration::findDeclarationForSynonym(query.declarations, *arg);
      if (std::find(argSynonymTypes.second.begin(),
                    argSynonymTypes.second.end(),
                    found_declaration->getDesignEntity()) ==
          argSynonymTypes.second.end()) {
        throw PQLValidationException(
            "Cannot match given design entity type: " +
            getDesignEntityString(found_declaration->getDesignEntity()) +
            " with allowed list of design entities: " +
            getDesignEntityVectorString(argSynonymTypes.second) +
            " during parse of such_that relation: " +
            getStringFromRelation(relation) + "'s first argument");
      }
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
  for (auto pattern : *(query.patternb)) {
    auto first_arg = pattern->getFirstArg();
    if (auto first_arg_syn = std::get_if<Synonym>(&first_arg)) {
      // Search the available declarations for the pattern synonym
      // The synonym must be an assignment synonym
      auto found_declaration = std::find_if(
          query.declarations->begin(), query.declarations->end(),
          [&](auto decl) {
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
}
