#include "pql.h"
#include <memory>

using namespace QE;
Query::Query(std::vector<QE::Declaration>* decls, QE::Declaration* selected_decl)
    : declarations(decls)
    , selected_declaration(selected_decl)
    , such_that(nullptr)
    , pattern(nullptr){};

std::unique_ptr<Query> Query::makeStubQuery() {
    auto decls = new std::vector<QE::Declaration>();
    auto selected_decl = new QE::Declaration();

    return std::make_unique<Query>(decls, selected_decl);
}
