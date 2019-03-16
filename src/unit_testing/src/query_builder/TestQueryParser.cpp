#include "catch.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "query_builder/core/query_lexer.h"
#include "query_builder/core/query_parser.h"
#include "query_builder/pql/query.h"

using namespace QE;

TEST_CASE ("Test Query Parser") {
  SECTION ("Test one assign one select query") {
    std::string input =
        "assign a, a1; procedure p; Select <a, p> such that Modifies (a, "
        "\"b\")";
    QueryLexer lexer = QueryLexer(input);
    lexer.lex();
    QueryParser parser = QueryParser(lexer.Tokens);
    Query query = parser.parse();
    REQUIRE(*query.declarations ==
            std::vector<Declaration>{
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a").value()),
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a1").value()),
                Declaration(DesignEntity::PROCEDURE,
                            QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query.result->selected_declarations->at(0)) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()));
    REQUIRE(*(query.result->selected_declarations->at(1)) ==
            Declaration(DesignEntity::PROCEDURE,
                        QE::Synonym::construct("p").value()));
    REQUIRE(*(query.rel_cond->at(0)) ==
            RelCond(Relation::Modifies, QE::Synonym::construct("a").value(),
                    QE::QuoteIdent("b"), query.declarations));
  }

  SECTION ("Test one assign one select query") {
    std::string input =
        "assign a, a1; procedure p; Select <a, p> such that Uses (a, "
        "\"b\") and Modifies(a, \"b\") pattern a (\"x\", _\"1\"_) with a.stmt# "
        "= 1";
    QueryLexer lexer = QueryLexer(input);
    lexer.lex();
    QueryParser parser = QueryParser(lexer.Tokens);
    Query query = parser.parse();
    REQUIRE(*query.declarations ==
            std::vector<Declaration>{
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a").value()),
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a1").value()),
                Declaration(DesignEntity::PROCEDURE,
                            QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query.result->selected_declarations->at(0)) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()));
    REQUIRE(*(query.result->selected_declarations->at(1)) ==
            Declaration(DesignEntity::PROCEDURE,
                        QE::Synonym::construct("p").value()));
    REQUIRE(*(query.rel_cond->at(0)) ==
            RelCond(Relation::Uses, QE::Synonym::construct("a").value(),
                    QE::QuoteIdent("b"), query.declarations));
    REQUIRE(*(query.rel_cond->at(1)) ==
            RelCond(Relation::Modifies, QE::Synonym::construct("a").value(),
                    QE::QuoteIdent("b"), query.declarations));
    REQUIRE(*(query.patternb->at(0)) ==
            PatternB(QE::Synonym::construct("a").value(), QE::QuoteIdent("x"),
                     Matcher(true, "1")));
    REQUIRE(*(query.with_cond->at(0)) ==
            QE::WithCond(
                QE::AttrRef(QE::SynAttr(QE::Synonym::construct("a").value(),
                                        QE::AttrName::STMT_NO),
                            QE::AttrType::INTEGER),
                QE::AttrRef(1, QE::AttrType::INTEGER)));
  }
}
