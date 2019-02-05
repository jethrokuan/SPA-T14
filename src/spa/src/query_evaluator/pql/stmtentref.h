#pragma once
#include <string>
#include <variant>

#include "query_evaluator/pql/quoteident.h"
#include "query_evaluator/pql/relation.h"
#include "query_evaluator/pql/synonym.h"
#include "query_evaluator/pql/underscore.h"

namespace QE {
using StatementNumber = unsigned int;
using StmtRef = std::variant<Synonym, Underscore, StatementNumber>;
using EntRef = std::variant<Synonym, Underscore, QuoteIdent>;
using StmtOrEntRef = std::variant<StmtRef, EntRef>;

// Given a particular Relation type
// Describes the type of statements we're expecting for first and second
// arguments

// Use defines since there are only 2 types
#define REFTYPE_STMTREF_AS_STRING "StmtRef"
#define REFTYPE_ENTREF_AS_STRING "EntRef"

enum class RefType { STMTREF, ENTREF };
std::pair<RefType, RefType> getArgTypesFromRelation(Relation&);

const inline std::string refTypeToString(RefType& refType) {
  return refType == RefType::STMTREF ? REFTYPE_STMTREF_AS_STRING
                                     : REFTYPE_ENTREF_AS_STRING;
}
}  // namespace QE