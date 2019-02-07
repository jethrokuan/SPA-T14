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

