#include "query_builder/pql/query.h"
using namespace QE;

Query::Query()
    : declarations(new std::vector<Declaration>()),
      result(new Result),
      rel_conds(new std::vector<RelCond*>()),
      pattern_conds(new std::vector<PatternCond*>()),
      with_conds(new std::vector<WithCond*>()){};

Query::~Query() {
  delete declarations;
  delete result;
}

namespace QE {
void printClause(Clause clause) {
  std::visit(overload{[&](auto r) { std::cout << *r; }}, clause);
}
}  // namespace QE
