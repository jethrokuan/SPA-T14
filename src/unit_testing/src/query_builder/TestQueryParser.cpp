#include "catch.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "query_builder/core/exceptions.h"
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
      REQUIRE_THROWS_WITH(qe.makePqlQuery(input), "Expected ',', got 's1'.");
    }
  }
}

TEST_CASE ("Test QueryParser Result Clause") {
  SECTION ("SUCCESS: Select BOOLEAN") {
    std::string input = "Select BOOLEAN";
    auto query = qe.makePqlQuery(input);
    REQUIRE(query.result->selected_declarations->empty());
    REQUIRE(query.result->T == ResultType::BOOLEAN);
  }

  SECTION ("FAILURE: Select boolean") {
    std::string input = "Select boolean";
    REQUIRE_THROWS_WITH(
        qe.makePqlQuery(input),
        "Cannot find a matching declaration for synonym boolean");
  }

  SECTION ("SUCCESS: Single items") {
    SECTION ("synonym") {
      std::string input = "stmt p; Select p";
      auto query = qe.makePqlQuery(input);
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
              Synonym("p"));
      REQUIRE(query.result->T == ResultType::TUPLE);
    }

    SECTION ("attrref") {
      std::string input = "stmt p; Select p.stmt#";
      auto query = qe.makePqlQuery(input);
      REQUIRE(query.result->selected_declarations->size() == 1);
      REQUIRE(std::get<SynAttr>(query.result->selected_declarations->at(0)) ==
              SynAttr(Synonym("p"), AttrName::STMT_NO, query.declarations));
      REQUIRE(query.result->T == ResultType::TUPLE);
    }
  }

  SECTION ("FAILURE: Single Items") {
    SECTION ("Invalid attr name on synonym") {
      std::string input = "stmt p; Select p.procName";
      REQUIRE_THROWS_AS(qe.makePqlQuery(input), PQLParseException);
    }
  }

  SECTION ("SUCCESS: Tuple") {
    SECTION ("one synonym") {
      std::string input = "stmt p, p1; Select <p>";
      auto query = qe.makePqlQuery(input);
      REQUIRE(query.result->selected_declarations->size() == 1);
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
              Synonym("p"));
      REQUIRE(query.result->T == ResultType::TUPLE);
    }

    SECTION ("one attrref") {
      std::string input = "stmt p, p1; Select <p.stmt#>";
      auto query = qe.makePqlQuery(input);
      REQUIRE(query.result->selected_declarations->size() == 1);
      REQUIRE(std::get<SynAttr>(query.result->selected_declarations->at(0)) ==
              SynAttr(Synonym("p"), AttrName::STMT_NO, query.declarations));
      REQUIRE(query.result->T == ResultType::TUPLE);
    }

    SECTION ("multiple synonyms") {
      std::string input = "stmt p, p1; Select <p, p1>";
      auto query = qe.makePqlQuery(input);
      REQUIRE(query.result->selected_declarations->size() == 2);
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(0)) ==
              Synonym("p"));
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(1)) ==
              Synonym("p1"));
      REQUIRE(query.result->T == ResultType::TUPLE);
    }

    SECTION ("multiple synattr") {
      std::string input = "read r; Select <r.stmt#, r.varName>";
      auto query = qe.makePqlQuery(input);
      REQUIRE(query.result->selected_declarations->size() == 2);
      REQUIRE(std::get<SynAttr>(query.result->selected_declarations->at(0)) ==
              SynAttr(Synonym("r"), AttrName::STMT_NO, query.declarations));
      REQUIRE(std::get<SynAttr>(query.result->selected_declarations->at(1)) ==
              SynAttr(Synonym("r"), AttrName::VAR_NAME, query.declarations));
      REQUIRE(query.result->T == ResultType::TUPLE);
    }

    SECTION ("mixed synonyms and synattr") {
      std::string input = "read r; Select <r.stmt#, r>";
      auto query = qe.makePqlQuery(input);
      REQUIRE(query.result->selected_declarations->size() == 2);
      REQUIRE(std::get<SynAttr>(query.result->selected_declarations->at(0)) ==
              SynAttr(Synonym("r"), AttrName::STMT_NO, query.declarations));
      REQUIRE(std::get<Synonym>(query.result->selected_declarations->at(1)) ==
              Synonym("r"));
      REQUIRE(query.result->T == ResultType::TUPLE);
    }
  }

  SECTION ("FAILURE: syntax errors") {
    SECTION ("Tuple missing brackets") {
      std::string input = "read r; Select r.stmt#, r";
      REQUIRE_THROWS_WITH(
          qe.makePqlQuery(input),
          "Expecting a such-that, pattern or with clause., got ,");
    }

    SECTION ("Tuple missing closing bracket") {
      std::string input = "read r; Select <r.stmt#, r";
      REQUIRE_THROWS_WITH(qe.makePqlQuery(input), "Expected ',', got ''.");
    }

    SECTION ("Tuple missing comma") {
      std::string input = "read r; Select <r.stmt# r>";
      REQUIRE_THROWS_WITH(qe.makePqlQuery(input), "Expected ',', got 'r'.");
    }
  }
}
