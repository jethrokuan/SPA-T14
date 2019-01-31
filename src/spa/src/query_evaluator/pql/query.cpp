#include "query_evaluator/pql/query.h"
using namespace QE;

Query::Query()
    : declarations(nullptr),
      selected_declaration(nullptr),
      such_that(nullptr),
      pattern(nullptr){};

Query::Query(std::vector<QE::Declaration>* decls,
             QE::Declaration* selected_decl)
    : declarations(decls),
      selected_declaration(selected_decl),
      such_that(nullptr),
      pattern(nullptr){};

Query::~Query() {
  delete declarations;
  delete selected_declaration;
  delete such_that;
  delete pattern;
}
