#include "catch.hpp"

#include <iostream>
#include <vector>

#include "query_evaluator/core/exceptions.h"
#include "query_evaluator/core/query_validator.h"
#include "query_evaluator/pql/pql.h"

using namespace QE;

TEST_CASE ("Test pattern assignment synonym not-assign Validate") {
  // procedure p; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::PROCEDURE, Synonym::construct("p").value())};
  auto select =
      new Declaration(DesignEntity::PROCEDURE, Synonym::construct("p").value());
  QE::Synonym syn = QE::Synonym::construct("p").value();
  QE::EntRef entRef = QE::EntRef(QE::Underscore());
  QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
  auto pattern = Pattern::construct_heap(syn, entRef, expr).value();
  Query q = Query();
  q.declarations = decl;
  q.selected_declaration = select;
  q.pattern = pattern;

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}

TEST_CASE ("Test pattern assignment synonym assign Validate") {
  // assign p; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("p").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("p").value());
  QE::Synonym syn = QE::Synonym::construct("p").value();
  QE::EntRef entRef = QE::EntRef(QE::Underscore());
  QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
  auto pattern = Pattern::construct_heap(syn, entRef, expr).value();
  Query q = Query();
  q.declarations = decl;
  q.selected_declaration = select;
  q.pattern = pattern;

  QueryValidator qv = QueryValidator();

  REQUIRE_NOTHROW(qv.validateQuery(q));
}

TEST_CASE ("Test no pattern Validate") {
  // assign p; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("p").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("p").value());
  Query q = Query();
  q.declarations = decl;
  q.selected_declaration = select;

  QueryValidator qv = QueryValidator();

  REQUIRE_NOTHROW(qv.validateQuery(q));
}

TEST_CASE ("Test pattern assignment synonym assign no matching decl Validate") {
  // procedure q; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("q").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("p").value());
  QE::Synonym syn = QE::Synonym::construct("p").value();
  QE::EntRef entRef = QE::EntRef(QE::Underscore());
  QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
  auto pattern = Pattern::construct_heap(syn, entRef, expr).value();
  Query q = Query();
  q.declarations = decl;
  q.selected_declaration = select;
  q.pattern = pattern;

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}

TEST_CASE ("Test such that with Modifies firstArg underscore Validate") {
  // procedure q; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("q").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("p").value());
  StmtOrEntRef a1 = QE::StmtRef(QE::Underscore());
  StmtOrEntRef a2 = QE::EntRef(QE::Underscore());
  auto suchthat = SuchThat::construct_heap(Relation::ModifiesS, a1, a2).value();
  Query q = Query();
  q.declarations = decl;
  q.selected_declaration = select;
  q.such_that = suchthat;

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}

TEST_CASE ("Test such that with UsesS firstArg underscore Validate") {
  // procedure q; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("q").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("p").value());
  StmtOrEntRef a1 = QE::StmtRef(QE::Underscore());
  StmtOrEntRef a2 = QE::EntRef(QE::Underscore());
  auto suchthat = SuchThat::construct_heap(Relation::UsesS, a1, a2).value();
  Query q = Query();
  q.declarations = decl;
  q.selected_declaration = select;
  q.such_that = suchthat;

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}

TEST_CASE ("Test such that with Follows firstArg underscore (ok) Validate") {
  // procedure q; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("q").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("p").value());
  StmtOrEntRef a1 = QE::StmtRef(QE::Underscore());
  StmtOrEntRef a2 = QE::StmtRef(QE::Underscore());
  auto suchthat = SuchThat::construct_heap(Relation::Follows, a1, a2).value();
  Query q = Query();
  q.declarations = decl;
  q.selected_declaration = select;
  q.such_that = suchthat;

  QueryValidator qv = QueryValidator();

  REQUIRE_NOTHROW(qv.validateQuery(q));
}

TEST_CASE ("Test such that no suchthat Validate") {
  // procedure q; Select p pattern p (_,_)
  auto decl = new std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, Synonym::construct("q").value())};
  auto select =
      new Declaration(DesignEntity::ASSIGN, Synonym::construct("p").value());
  Query q = Query();
  q.declarations = decl;
  q.selected_declaration = select;

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
  q.selected_declaration = select;
  q.such_that = suchthat;

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
  q.selected_declaration = select;
  q.such_that = suchthat;

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
  q.selected_declaration = select;
  q.such_that = suchthat;

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
  q.selected_declaration = select;
  q.such_that = suchthat;

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
  q.selected_declaration = select;
  q.such_that = suchthat;

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
  q.selected_declaration = select;
  q.pattern = pattern;

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
  q.selected_declaration = select;
  q.pattern = pattern;

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
  q.selected_declaration = select;
  q.pattern = pattern;

  QueryValidator qv = QueryValidator();

  REQUIRE_THROWS_AS(qv.validateQuery(q), PQLValidationException);
}