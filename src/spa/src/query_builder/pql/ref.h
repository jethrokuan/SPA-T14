#include "query_builder/pql/quoteident.h"
#include "query_builder/pql/relation.h"
#include "query_builder/pql/synonym.h"
#include "query_builder/pql/underscore.h"

namespace QE {
using StatementNumber = unsigned int;
using Ref = std::variant<Synonym, Underscore, StatementNumber, QuoteIdent>;
}  // namespace QE
