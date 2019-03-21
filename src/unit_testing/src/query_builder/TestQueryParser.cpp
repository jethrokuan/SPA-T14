#include "catch.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "query_builder/core/query_lexer.h"
#include "query_builder/core/query_parser.h"
#include "query_builder/pql/query.h"
#include "query_builder/query_builder.h"

using namespace QE;

auto qe = QueryBuilder();

TEST_CASE ("Test QueryParser Declarations") {
  SECTION ("SUCCESS: Single Declarations") {
    SECTION ("SUCCESS: assign a;Select a") {
      std::string input = "assign a;Select a";
      auto query = qe.makePqlQuery(input);
      REQUIRE(*(query.declarations) ==
              std::vector<Declaration>{
                  Declaration(DesignEntity::ASSIGN, Synonym("a"))});
      REQUIRE(query.result->selected_declarations->size() == 1);
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
              Synonym("a"));
      REQUIRE(query.rel_cond->empty());
      REQUIRE(query.patternb->empty());
      REQUIRE(query.with_cond->empty());
    }

    SECTION ("SUCCESS: variable v;Select v") {
      std::string input = "variable v; Select v";
      auto query = qe.makePqlQuery(input);
      REQUIRE(*(query.declarations) ==
              std::vector<Declaration>{
                  Declaration(DesignEntity::VARIABLE, Synonym("v"))});
      REQUIRE(query.result->selected_declarations->size() == 1);
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
              Synonym("v"));
      REQUIRE(query.rel_cond->empty());
      REQUIRE(query.patternb->empty());
      REQUIRE(query.with_cond->empty());
    }

    SECTION ("SUCCESS: stmt p;Select p") {
      std::string input = "stmt p; Select p";
      auto query = qe.makePqlQuery(input);
      REQUIRE(*(query.declarations) == std::vector<Declaration>{Declaration(
                                           DesignEntity::STMT, Synonym("p"))});
      REQUIRE(query.result->selected_declarations->size() == 1);
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
              Synonym("p"));
      REQUIRE(query.rel_cond->empty());
      REQUIRE(query.patternb->empty());
      REQUIRE(query.with_cond->empty());
    }

    SECTION ("SUCCESS: read r;Select r") {
      std::string input = "read r; Select r";
      auto query = qe.makePqlQuery(input);
      REQUIRE(*(query.declarations) == std::vector<Declaration>{Declaration(
                                           DesignEntity::READ, Synonym("r"))});
      REQUIRE(query.result->selected_declarations->size() == 1);
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
              Synonym("r"));
      REQUIRE(query.rel_cond->empty());
      REQUIRE(query.patternb->empty());
      REQUIRE(query.with_cond->empty());
    }

    SECTION ("SUCCESS: prog_line p;Select p") {
      std::string input = "prog_line p; Select p";
      auto query = qe.makePqlQuery(input);
      REQUIRE(*(query.declarations) ==
              std::vector<Declaration>{
                  Declaration(DesignEntity::PROG_LINE, Synonym("p"))});
      REQUIRE(query.result->selected_declarations->size() == 1);
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
              Synonym("p"));
      REQUIRE(query.rel_cond->empty());
      REQUIRE(query.patternb->empty());
      REQUIRE(query.with_cond->empty());
    }
  }

  SECTION ("FAILURE: invalid design entities") {
    SECTION ("FAILURE: Stmt p;Select p") {
      std::string input = "Stmt p; Select p";
      REQUIRE_THROWS_WITH(qe.makePqlQuery(input),
                          "Expected 'Select', got 'Stmt'.");
    }

    SECTION ("FAILURE: progline p;Select p") {
      std::string input = "progline p; Select p";
      REQUIRE_THROWS_WITH(qe.makePqlQuery(input),
                          "Expected 'Select', got 'progline'.");
    }
  }

  SECTION ("SUCCESS: multiple declarations") {
    SECTION ("SUCCESS: assign a1, a2; Select a2") {
      std::string input = "assign a1, a2; Select a2";
      auto query = qe.makePqlQuery(input);
      REQUIRE(*(query.declarations) ==
              std::vector<Declaration>{
                  Declaration(DesignEntity::ASSIGN, Synonym("a1")),
                  Declaration(DesignEntity::ASSIGN, Synonym("a2"))});
      REQUIRE(query.result->selected_declarations->size() == 1);
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
              Synonym("a2"));
      REQUIRE(query.rel_cond->empty());
      REQUIRE(query.patternb->empty());
      REQUIRE(query.with_cond->empty());
    }

    SECTION ("SUCCESS: assign a1; stmt s1; Select a1") {
      std::string input = "assign a1; stmt s1; Select a1";
      auto query = qe.makePqlQuery(input);
      REQUIRE(*(query.declarations) ==
              std::vector<Declaration>{
                  Declaration(DesignEntity::ASSIGN, Synonym("a1")),
                  Declaration(DesignEntity::STMT, Synonym("s1"))});
      REQUIRE(query.result->selected_declarations->size() == 1);
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
              Synonym("a1"));
      REQUIRE(query.rel_cond->empty());
      REQUIRE(query.patternb->empty());
      REQUIRE(query.with_cond->empty());
    }
  }

  SECTION ("FAILURE: Syntax errors") {
    SECTION ("SUCCESS: assign a1, stmt s1; Select a1") {
      std::string input = "assign a1, stmt s1; Select a1";
      REQUIRE_THROWS_WITH(qe.makePqlQuery(input), "Expected, ',', got 's1'.");
    }
  }

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
                Declaration(DesignEntity::ASSIGN, QE::Synonym("a")),
                Declaration(DesignEntity::ASSIGN, QE::Synonym("a1")),
                Declaration(DesignEntity::PROCEDURE, QE::Synonym("p"))});
    REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
            (QE::Synonym("a")));
    REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(1)) ==
            (QE::Synonym("p")));
    REQUIRE(*(query.rel_cond->at(0)) ==
            RelCond(Relation::Modifies, QE::Synonym("a"), QE::QuoteIdent("b"),
                    query.declarations));
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
                Declaration(DesignEntity::ASSIGN, QE::Synonym("a")),
                Declaration(DesignEntity::ASSIGN, QE::Synonym("a1")),
                Declaration(DesignEntity::PROCEDURE, QE::Synonym("p"))});
    REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
            (QE::Synonym("a")));
    REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(1)) ==
            (QE::Synonym("p")));
    REQUIRE(*(query.rel_cond->at(0)) ==
            RelCond(Relation::Uses, QE::Synonym("a"), QE::QuoteIdent("b"),
                    query.declarations));
    REQUIRE(*(query.rel_cond->at(1)) ==
            RelCond(Relation::Modifies, QE::Synonym("a"), QE::QuoteIdent("b"),
                    query.declarations));
    REQUIRE(*(query.patternb->at(0)) == PatternB(QE::Synonym("a"),
                                                 QE::QuoteIdent("x"),
                                                 Matcher(true, "1")));
    REQUIRE(*(query.with_cond->at(0)) ==
            QE::WithCond(
                QE::AttrRef(QE::SynAttr(QE::Synonym("a"), QE::AttrName::STMT_NO,
                                        query.declarations),
                            query.declarations),
                QE::AttrRef(1, query.declarations)));
  }
}
