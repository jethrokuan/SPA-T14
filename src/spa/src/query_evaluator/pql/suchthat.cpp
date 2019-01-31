#include "query_evaluator/pql/suchthat.h"
using namespace QE;

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

