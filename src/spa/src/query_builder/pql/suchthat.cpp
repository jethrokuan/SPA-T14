#include "query_builder/pql/suchthat.h"
using namespace QE;

// Make sure this relation has the correct argument types
std::optional<SuchThat> SuchThat::construct(Relation rel,
                                            StmtOrEntRef& arg_left,
                                            StmtOrEntRef& arg_right) {
  std::pair<RefType, RefType> argTypes = getArgTypesFromRelation(rel);
  if ((argTypes.first == RefType::STMTREF &&
       std::holds_alternative<EntRef>(arg_left)) ||
      (argTypes.first == RefType::ENTREF &&
       std::holds_alternative<StmtRef>(arg_left)) ||
      (argTypes.second == RefType::STMTREF &&
       std::holds_alternative<EntRef>(arg_right)) ||
      (argTypes.second == RefType::ENTREF &&
       std::holds_alternative<StmtRef>(arg_right))) {
    return std::nullopt;
  } else {
    // We have the correct types: construct this relation
    return SuchThat(rel, arg_left, arg_right);
  }
}

// Make sure this relation has the correct argument types
std::optional<SuchThat*> SuchThat::construct_heap(Relation rel,
                                                  StmtOrEntRef& arg_left,
                                                  StmtOrEntRef& arg_right) {
  std::pair<RefType, RefType> argTypes = getArgTypesFromRelation(rel);
  if ((argTypes.first == RefType::STMTREF &&
       std::holds_alternative<EntRef>(arg_left)) ||
      (argTypes.first == RefType::ENTREF &&
       std::holds_alternative<StmtRef>(arg_left)) ||
      (argTypes.second == RefType::STMTREF &&
       std::holds_alternative<EntRef>(arg_right)) ||
      (argTypes.second == RefType::ENTREF &&
       std::holds_alternative<StmtRef>(arg_right))) {
    return std::nullopt;
  } else {
    // We have the correct types: construct this relation
    return new SuchThat(rel, arg_left, arg_right);
  }
}
