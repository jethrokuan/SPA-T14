#include "query_builder/pql/query.h"
using namespace QE;

Query::Query()
    : declarations(new std::vector<Declaration>()),
      result(new Result),
      such_that(new std::vector<SuchThat*>()),
      rel_cond(new std::vector<RelCond*>()),
      pattern(new std::vector<Pattern*>()),
      patternb(new std::vector<PatternB*>()){};

Query::~Query() {
  delete declarations;
  delete result;
  delete such_that;
  delete pattern;
}
