#include "pql.h"
#include <memory>
#include <regex>

using namespace QE;
Query::Query(std::vector<QE::Declaration>* decls, QE::Declaration* selected_decl)
    : declarations(decls)
    , selected_declaration(selected_decl)
    , such_that(nullptr)
    , pattern(nullptr){};

Query::~Query() {
    delete declarations;
    delete selected_declaration;
    delete such_that;
    delete pattern;
}

std::unique_ptr<Query> Query::makeStubQuery() {
    auto decls = new std::vector<QE::Declaration>();
    auto selected_decl = new QE::Declaration(DesignEntity::ASSIGN, "p");
    return std::make_unique<Query>(decls, selected_decl);
}

bool Declaration::isValid() {
    return std::regex_match(synonym, synonym_regex);
}
