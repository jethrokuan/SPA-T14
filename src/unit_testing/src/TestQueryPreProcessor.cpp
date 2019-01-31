#include "catch.hpp"

#include <iostream>
#include <vector>
#include "query_evaluator/query_evaluator.h"
#include "query_evaluator/query_preprocessor.h"

using namespace QE;
TEST_CASE("Test one assign one select query Preprocess") {
  auto qp = QE::QueryPreprocessor();
  std::string input = "assign p;Select p";
  auto query = qp.getQuery(input);
  REQUIRE(*(query->declarations) ==
          std::vector<Declaration>{Declaration(
              DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
  REQUIRE(
      *(query->selected_declaration) ==
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));
  REQUIRE(query->such_that == nullptr);
}

TEST_CASE("Test two assign one select query Preprocess") {
  auto qp = QE::QueryPreprocessor();
  std::string input = "assign p;stmt s;Select p";
  auto query = qp.getQuery(input);
  REQUIRE(*(query->declarations) ==
          std::vector<Declaration>{
              Declaration(DesignEntity::ASSIGN,
                          QE::Synonym::construct("p").value()),
              Declaration(DesignEntity::STMT,
                          QE::Synonym::construct("s").value())});
  REQUIRE(
      *(query->selected_declaration) ==
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));
  REQUIRE(query->such_that == nullptr);
}

TEST_CASE("Test three assign one select query Preprocess") {
  auto qp = QE::QueryPreprocessor();
  std::string input = "assign p;stmt s;assign q;Select p";
  auto query = qp.getQuery(input);
  REQUIRE(
      *(query->declarations) ==
      std::vector<Declaration>{
          Declaration(DesignEntity::ASSIGN,
                      QE::Synonym::construct("p").value()),
          Declaration(DesignEntity::STMT, QE::Synonym::construct("s").value()),
          Declaration(DesignEntity::ASSIGN,
                      QE::Synonym::construct("q").value())});
  REQUIRE(
      *(query->selected_declaration) ==
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));
  REQUIRE(query->such_that == nullptr);
}

TEST_CASE(
    "Test one assign one select one malformed such that query Preprocess") {
  auto qp = QE::QueryPreprocessor();
  std::string input = "assign p;Select p such that Follows(6++=sss| , s23123|)";
  auto query = qp.getQuery(input);
  REQUIRE(*(query->declarations) ==
          std::vector<Declaration>{Declaration(
              DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
  REQUIRE(
      *(query->selected_declaration) ==
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));
  REQUIRE(query->such_that == nullptr);
}

TEST_CASE(
    "Test one assign one select one syntactically correct such that query "
    "Preprocess") {
  auto qp = QE::QueryPreprocessor();
  std::string input = "assign p;Select p such that Follows(_,_)";
  auto query = qp.getQuery(input);
  REQUIRE(*(query->declarations) ==
          std::vector<Declaration>{Declaration(
              DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
  REQUIRE(
      *(query->selected_declaration) ==
      Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

  QE::StmtOrEntRef a1 = QE::StmtRef(QE::Underscore());
  QE::StmtOrEntRef a2 = QE::StmtRef(QE::Underscore());
  REQUIRE(*(query->such_that) ==
          SuchThat::construct(Relation::Follows, a1, a2).value());
}
