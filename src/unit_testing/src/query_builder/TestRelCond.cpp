#include "catch.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "query_builder/core/query_lexer.h"
#include "query_builder/core/query_parser.h"
#include "query_builder/pql/pql.h"

using namespace QE;

TEST_CASE (
    "Test Relcond Specialization of Modifies -> ModifiesS for assign arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym::construct("p").value())};
  RelCond r = RelCond(Relation::Modifies, QE::Synonym::construct("a").value(),
                      QE::Synonym::construct("p").value(), &decls);
  REQUIRE(r.relation == Relation::ModifiesS);
}

TEST_CASE (
    "Test Relcond Specialization of Modifies -> ModifiesP for procedure arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym::construct("p").value())};
  RelCond r = RelCond(Relation::Modifies, QE::Synonym::construct("p").value(),
                      QE::Synonym::construct("a").value(), &decls);
  REQUIRE(r.relation == Relation::ModifiesP);
}

TEST_CASE (
    "Test Relcond Specialization of Modifies -> ModifiesP for quoteident arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym::construct("p").value())};
  RelCond r = RelCond(Relation::Modifies, QE::QuoteIdent("x"),
                      QE::Synonym::construct("a").value(), &decls);
  REQUIRE(r.relation == Relation::ModifiesP);
}

TEST_CASE (
    "Test Relcond Specialization of Modifies -> ModifiesS for stmt arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym::construct("p").value())};
  RelCond r = RelCond(Relation::Modifies, 123,
                      QE::Synonym::construct("a").value(), &decls);
  REQUIRE(r.relation == Relation::ModifiesS);
}

TEST_CASE (
    "Test Relcond Specialization of Uses -> UsesS for assign arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym::construct("p").value())};
  RelCond r = RelCond(Relation::Uses, QE::Synonym::construct("a").value(),
                      QE::Synonym::construct("p").value(), &decls);
  REQUIRE(r.relation == Relation::UsesS);
}

TEST_CASE (
    "Test Relcond Specialization of Uses -> UsesP for procedure arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym::construct("p").value())};
  RelCond r = RelCond(Relation::Uses, QE::Synonym::construct("p").value(),
                      QE::Synonym::construct("a").value(), &decls);
  REQUIRE(r.relation == Relation::UsesP);
}

TEST_CASE (
    "Test Relcond Specialization of Uses -> UsesP for quoteident arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym::construct("p").value())};
  RelCond r = RelCond(Relation::Uses, QE::QuoteIdent("x"),
                      QE::Synonym::construct("a").value(), &decls);
  REQUIRE(r.relation == Relation::UsesP);
}

TEST_CASE (
    "Test Relcond Specialization of Uses -> UsesS for stmt arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym::construct("p").value())};
  RelCond r =
      RelCond(Relation::Uses, 123, QE::Synonym::construct("a").value(), &decls);
  REQUIRE(r.relation == Relation::UsesS);
}