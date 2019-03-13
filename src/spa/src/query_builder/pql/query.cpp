#include "query_builder/pql/query.h"
using namespace QE;

Query::Query()
    : declarations(new std::vector<Declaration>()),
      selected_declarations(new std::vector<Declaration*>()),
      such_that(new std::vector<SuchThat*>()),
      pattern(new std::vector<Pattern*>()){};

Query::~Query() {
  delete declarations;
  delete selected_declarations;
  delete such_that;
  delete pattern;
}
