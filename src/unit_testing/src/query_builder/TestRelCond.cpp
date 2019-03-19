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
      Declaration(DesignEntity::ASSIGN, QE::Synonym("a")),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym("p"))};
  RelCond r = RelCond(Relation::Modifies, QE::Synonym("a"),
                      QE::Synonym("p"), &decls);
  REQUIRE(r.relation == Relation::ModifiesS);
}

TEST_CASE (
    "Test Relcond Specialization of Modifies -> ModifiesP for procedure arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym("a")),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym("p"))};
  RelCond r = RelCond(Relation::Modifies, QE::Synonym("p"),
                      QE::Synonym("a"), &decls);
  REQUIRE(r.relation == Relation::ModifiesP);
}

TEST_CASE (
    "Test Relcond Specialization of Modifies -> ModifiesP for quoteident arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym("a")),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym("p"))};
  RelCond r = RelCond(Relation::Modifies, QE::QuoteIdent("x"),
                      QE::Synonym("a"), &decls);
  REQUIRE(r.relation == Relation::ModifiesP);
}

TEST_CASE (
    "Test Relcond Specialization of Modifies -> ModifiesS for stmt arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym("a")),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym("p"))};
  RelCond r = RelCond(Relation::Modifies, 123,
                      QE::Synonym("a"), &decls);
  REQUIRE(r.relation == Relation::ModifiesS);
}

TEST_CASE (
    "Test Relcond Specialization of Uses -> UsesS for assign arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym("a")),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym("p"))};
  RelCond r = RelCond(Relation::Uses, QE::Synonym("a"),
                      QE::Synonym("p"), &decls);
  REQUIRE(r.relation == Relation::UsesS);
}

TEST_CASE (
    "Test Relcond Specialization of Uses -> UsesP for procedure arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym("a")),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym("p"))};
  RelCond r = RelCond(Relation::Uses, QE::Synonym("p"),
                      QE::Synonym("a"), &decls);
  REQUIRE(r.relation == Relation::UsesP);
}

TEST_CASE (
    "Test Relcond Specialization of Uses -> UsesP for quoteident arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym("a")),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym("p"))};
  RelCond r = RelCond(Relation::Uses, QE::QuoteIdent("x"),
                      QE::Synonym("a"), &decls);
  REQUIRE(r.relation == Relation::UsesP);
}

TEST_CASE (
    "Test Relcond Specialization of Uses -> UsesS for stmt arg"
    "synonym") {
  auto decls = std::vector<Declaration>{
      Declaration(DesignEntity::ASSIGN, QE::Synonym("a")),
      Declaration(DesignEntity::PROCEDURE,
                  QE::Synonym("p"))};
  RelCond r =
      RelCond(Relation::Uses, 123, QE::Synonym("a"), &decls);
  REQUIRE(r.relation == Relation::UsesS);
}
