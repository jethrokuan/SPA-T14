#include "pql.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <optional>

using namespace QE;

Query::Query()
    : declarations(nullptr),
      selected_declaration(nullptr),
      such_that(nullptr),
      pattern(nullptr){};

Query::Query(std::vector<QE::Declaration>* decls,
             QE::Declaration* selected_decl)
    : declarations(decls),
      selected_declaration(selected_decl),
      such_that(nullptr),
      pattern(nullptr){};

Query::~Query() {
  delete declarations;
  delete selected_declaration;
  delete such_that;
  delete pattern;
}

// Definitions for Synonym
const std::regex Synonym::synonym_regex = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");
std::optional<Synonym> Synonym::construct(const char* synonym) {
  std::string s(synonym);
  return Synonym::construct(s);
}
std::optional<Synonym> Synonym::construct(std::string& synonym) {
  if (std::regex_match(synonym, synonym_regex)) {
    return Synonym(synonym);
  } else {
    return std::nullopt;
  }
}

// Definitions for QuoteIdent
const std::regex QuoteIdent::quoteident_regex =
    std::regex("\"[a-zA-Z](\\d|[a-zA-Z])*\"");
std::optional<QuoteIdent> QuoteIdent::construct(const char* quote_ident) {
  std::string qi(quote_ident);
  return QuoteIdent::construct(qi);
}
std::optional<QuoteIdent> QuoteIdent::construct(std::string& quote_ident) {
  if (std::regex_match(quote_ident, quoteident_regex)) {
    return QuoteIdent(quote_ident);
  } else {
    return std::nullopt;
  }
}

// Make sure this relation has the correct argument types
std::optional<SuchThat> SuchThat::construct(Relation rel, StmtOrEntRef& arg1,
                                            StmtOrEntRef& arg2) {
  std::pair<RefType, RefType> argTypes = getArgTypesFromRelation(rel);
  if ((argTypes.first == RefType::STMTREF &&
       std::holds_alternative<EntRef>(arg1)) ||
      (argTypes.first == RefType::ENTREF &&
       std::holds_alternative<StmtRef>(arg1)) ||
      (argTypes.second == RefType::STMTREF &&
       std::holds_alternative<EntRef>(arg2)) ||
      (argTypes.second == RefType::ENTREF &&
       std::holds_alternative<StmtRef>(arg2))) {
    return std::nullopt;
  } else {
    // We have the correct types: construct this relation
    return SuchThat(rel, arg1, arg2);
  }
}

// Make sure this relation has the correct argument types
std::optional<SuchThat*> SuchThat::construct_heap(Relation rel,
                                                  StmtOrEntRef& arg1,
                                                  StmtOrEntRef& arg2) {
  std::pair<RefType, RefType> argTypes = getArgTypesFromRelation(rel);
  if ((argTypes.first == RefType::STMTREF &&
       std::holds_alternative<EntRef>(arg1)) ||
      (argTypes.first == RefType::ENTREF &&
       std::holds_alternative<StmtRef>(arg1)) ||
      (argTypes.second == RefType::STMTREF &&
       std::holds_alternative<EntRef>(arg2)) ||
      (argTypes.second == RefType::ENTREF &&
       std::holds_alternative<StmtRef>(arg2))) {
    return std::nullopt;
  } else {
    // We have the correct types: construct this relation
    return new SuchThat(rel, arg1, arg2);
  }
}

namespace QE {
// Hard to use unordered_map even though faster
// - need to define specialized hash for enum class
std::map<DesignEntity, std::string> designEntityToStringMap({
    {DesignEntity::STMT, "stmt"},
    {DesignEntity::READ, "read"},
    {DesignEntity::PRINT, "print"},
    {DesignEntity::CALL, "call"},
    {DesignEntity::WHILE, "while"},
    {DesignEntity::IF, "if"},
    {DesignEntity::ASSIGN, "assign"},
    {DesignEntity::VARIABLE, "variable"},
    {DesignEntity::CONSTANT, "constant"},
    {DesignEntity::PROCEDURE, "procedure"},
});

std::map<Relation, std::string> relationToStringMap({
    {Relation::ModifiesS, "Modifies"},
    {Relation::UsesS, "Uses"},
    {Relation::Parent, "Parent"},
    {Relation::ParentT, "Parent*"},
    {Relation::Follows, "Follows"},
    {Relation::FollowsT, "Follows*"},
});

std::map<Relation, std::pair<RefType, RefType>> relationToArgTypesMap({
    {Relation::ModifiesS, std::make_pair(RefType::STMTREF, RefType::ENTREF)},
    {Relation::UsesS, std::make_pair(RefType::STMTREF, RefType::ENTREF)},
    {Relation::Parent, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
    {Relation::ParentT, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
    {Relation::Follows, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
    {Relation::FollowsT, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
});

// Generic template for swapping keys and value of a map into a new map
// Should put in generic lib
template <class T1, class T2>
std::map<T2, T1> swapPairs(std::map<T1, T2> m) {
  std::map<T2, T1> m1;
  for (auto&& item : m) {
    m1.emplace(item.second, item.first);
  }
  return m1;
};

auto stringToDesignEntityMap =
    swapPairs<DesignEntity, std::string>(designEntityToStringMap);
auto stringToRelationMap =
    swapPairs<Relation, std::string>(relationToStringMap);

DesignEntity getDesignEntity(std::string& designentity_string) {
  return stringToDesignEntityMap.at(designentity_string);
}
std::string getDesignEntityString(DesignEntity de) {
  return designEntityToStringMap.at(de);
}
Relation getRelation(std::string& relation_string) {
  return stringToRelationMap.at(relation_string);
}
std::string getStringFromRelation(Relation relation) {
  return relationToStringMap.at(relation);
}
const std::map<Relation, std::string>& getRelationToStringMap() {
  return relationToStringMap;
}

std::pair<RefType, RefType> getArgTypesFromRelation(Relation& r) {
  return relationToArgTypesMap.at(r);
}
}  // namespace QE
