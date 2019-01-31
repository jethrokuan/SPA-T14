#pragma once
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
enum class RefType { STMTREF, ENTREF };
std::pair<RefType, RefType> getArgTypesFromRelation(Relation&);
}  // namespace QE