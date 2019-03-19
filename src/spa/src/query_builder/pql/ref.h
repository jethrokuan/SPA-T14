#pragma once
#include <unordered_set>
#include <variant>
#include "query_builder/pql/quoteident.h"
#include "query_builder/pql/synonym.h"
#include "query_builder/pql/underscore.h"

namespace QE {
using StatementNumber = unsigned int;
using Ref = std::variant<Synonym, Underscore, StatementNumber, QuoteIdent>;
// If Ref is changed  (even order!) - both of these sets may need to be changed
// StmtRef: {Synonym, Underscore, StatementNumber}
using RefTypeIndexSet = std::unordered_set<unsigned int>;
// 3 sets of this type are defined in ref.cpp
static RefTypeIndexSet stmtRefIndices = {0, 1, 2};
// EntRef: {Synonym, Underscore, QuoteIdent}
static RefTypeIndexSet entRefIndices = {0, 1, 3};
// LineRef: {Synonym, Underscore, StatementNumber} - honestly same as StmtRef
static RefTypeIndexSet lineRefIndices = {0, 1, 2};
}  // namespace QE
