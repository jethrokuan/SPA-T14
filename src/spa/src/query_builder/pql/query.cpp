#include "query_builder/pql/query.h"
using namespace QE;

Query::Query()
    : declarations(new std::vector<Declaration>()),
      result(new Result),
      rel_cond(new std::vector<RelCond*>()),
      patternb(new std::vector<PatternB*>()),
      with_cond(new std::vector<WithCond*>()){};

Query::~Query() {
  delete declarations;
  delete result;
}
