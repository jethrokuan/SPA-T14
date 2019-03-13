#include "query_builder/pql/query.h"
using namespace QE;

Query::Query()
    : declarations(new std::vector<Declaration>()),
      result(new Result),
      such_that(new std::vector<SuchThat*>()),
      pattern(new std::vector<Pattern*>()){};

Query::~Query() {
  delete declarations;
  delete result;
  delete such_that;
  delete pattern;
}
