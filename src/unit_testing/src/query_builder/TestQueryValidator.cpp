#include "catch.hpp"

#include <iostream>
#include <vector>

#include "query_builder/core/exceptions.h"
#include "query_builder/core/query_validator.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"

using namespace QE;
TEST_CASE ("Test pattern assignment synonym not-assign Validate") {}

TEST_CASE ("Test pattern assignment synonym assign no matching decl Validate") {
  std::string query_string = "assign a1; Select a1 pattern a(_, _)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with Modifies firstArg underscore Validate") {
  std::string query_string = "assign a; Select a such that Modifies(_, \"a\")";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with Uses firstArg underscore Validate") {
  std::string query_string = "assign a; Select a such that Uses(_, \"a\")";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with Follows firstArg underscore Validate (ok)") {
  std::string query_string = "assign a; Select a such that Follows(_, 2)";
  auto qe = QueryBuilder();
  REQUIRE_NOTHROW(qe.makePqlQuery(query_string));
}

TEST_CASE ("Test such that with Follows secondArg quoteIdent") {
  std::string query_string = "assign a; Select a such that Follows(_, \"2\")";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with Parent firstArg quoteIdent") {
  std::string query_string = "assign a; Select a such that Parent(\"2\", _)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test pattern with firstArg stmtRef") {
  std::string query_string = "assign a; Select a pattern a(2, \"x\")";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

/*
TEST_CASE ("Test such that with not-found synonym firstarg Validate") {
  std::string query_string = "assign a; Select a such that Follows(_, \"a\")";
  auto qe = QueryBuilder();
  REQUIRE_NOTHROW(qe.makePqlQuery(query_string));
}
*/

/*
  // procedure q; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("q").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("p").value());
  Query q = Query();
  q.declarations = decl;
  q.result->selected_declarations = new std::vector{select};

  QueryValidator qv = QueryValidator();

  REQUIRE_NOTHROW(qv.validateQuery(q));
}

TEST_CASE ("Test such that with not-found synonym firstarg Validate") {
  // procedure q; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("a").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("a").value());
  StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a1").value());
  StmtOrEntRef a2 = QE::EntRef(QE::Underscore());
  auto suchthat = SuchThat::construct_heap(Relation::ModifiesS, a1, a2).value();
  Query q = Query();
  q.declarations = decl;
  q.result->selected_declarations = new std::vector{select};
  q.such_that = new std::vector{suchthat};

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}

TEST_CASE ("Test such that with invalid type firstarg Validate") {
  // Modifies can't take a variable as first argument
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::VARIABLE, Synonym::construct("a1").value())};
  auto select =
      new Declaration(DesignEntity::VARIABLE, Synonym::construct("a1").value());
  StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a1").value());
  StmtOrEntRef a2 = QE::EntRef(QE::Underscore());
  auto suchthat = SuchThat::construct_heap(Relation::ModifiesS, a1, a2).value();
  Query q = Query();
  q.declarations = decl;
  q.result->selected_declarations = new std::vector{select};
  q.such_that = new std::vector{suchthat};

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}

TEST_CASE ("Test such that with invalid type secondarg Validate") {
  // Modifies can't take a variable as first argument
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::STMT, Synonym::construct("a1").value()),
      Declaration(DesignEntity::STMT, Synonym::construct("a2").value())};
  auto select =
      new Declaration(DesignEntity::STMT, Synonym::construct("a1").value());
  StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a1").value());
  StmtOrEntRef a2 = QE::EntRef(QE::Synonym::construct("a2").value());
  auto suchthat = SuchThat::construct_heap(Relation::ModifiesS, a1, a2).value();
  Query q = Query();
  q.declarations = decl;
  q.result->selected_declarations = new std::vector{select};
  q.such_that = new std::vector{suchthat};

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}

TEST_CASE ("Test multiple declarations with the same name") {
  // Modifies can't take a variable as first argument
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::STMT, Synonym::construct("a1").value()),
      Declaration(DesignEntity::STMT, Synonym::construct("a1").value())};
  auto select =
      new Declaration(DesignEntity::STMT, Synonym::construct("a1").value());
  StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a1").value());
  StmtOrEntRef a2 = QE::EntRef(QE::Underscore());
  auto suchthat = SuchThat::construct_heap(Relation::ModifiesS, a1, a2).value();
  Query q = Query();
  q.declarations = decl;
  q.result->selected_declarations = new std::vector{select};
  q.such_that = new std::vector{suchthat};

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}

TEST_CASE (
    "Test multiple declarations with the same name with different types") {
  // Modifies can't take a variable as first argument
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::STMT, Synonym::construct("a1").value()),
      Declaration(DesignEntity::ASSIGN, Synonym::construct("a1").value())};
  auto select =
      new Declaration(DesignEntity::STMT, Synonym::construct("a1").value());
  StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a1").value());
  StmtOrEntRef a2 = QE::EntRef(QE::Underscore());
  auto suchthat = SuchThat::construct_heap(Relation::ModifiesS, a1, a2).value();
  Query q = Query();
  q.declarations = decl;
  q.result->selected_declarations = new std::vector{select};
  q.such_that = new std::vector{suchthat};

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}

TEST_CASE ("Test pattern assignment first argument variable Validate") {
  // assign p; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("a").value()),
      Declaration(DesignEntity::VARIABLE, Synonym::construct("v").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("a").value());
  QE::Synonym syn = QE::Synonym::construct("a").value();
  QE::EntRef entRef = QE::EntRef(QE::Synonym::construct("v").value());
  QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
  auto pattern = Pattern::construct_heap(syn, entRef, expr).value();
  Query q = Query();
  q.declarations = decl;
  q.result->selected_declarations = new std::vector{select};
  q.pattern = new std::vector{pattern};

  QueryValidator qv = QueryValidator();

  REQUIRE_NOTHROW(qv.validateQuery(q));
}

TEST_CASE ("Test pattern assignment first argument not-var Validate") {
  // assign p; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("a").value()),
      Declaration(DesignEntity::STMT, Synonym::construct("v").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("a").value());
  QE::Synonym syn = QE::Synonym::construct("a").value();
  QE::EntRef entRef = QE::EntRef(QE::Synonym::construct("v").value());
  QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
  auto pattern = Pattern::construct_heap(syn, entRef, expr).value();
  Query q = Query();
  q.declarations = decl;
  q.result->selected_declarations = new std::vector{select};
  q.pattern = new std::vector{pattern};

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}

TEST_CASE ("Test pattern assignment first argument not-present Validate") {
  // assign p; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("a").value()),
      Declaration(DesignEntity::STMT, Synonym::construct("v").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("a").value());
  QE::Synonym syn = QE::Synonym::construct("a").value();
  QE::EntRef entRef = QE::EntRef(QE::Synonym::construct("v1").value());
  QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
  auto pattern = Pattern::construct_heap(syn, entRef, expr).value();
  Query q = Query();
  q.declarations = decl;
  q.result->selected_declarations = new std::vector{select};
  q.pattern = new std::vector{pattern};

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}
*/