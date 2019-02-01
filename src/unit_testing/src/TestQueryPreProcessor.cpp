#include "catch.hpp"

#include <iostream>
#include <vector>
#include "query_evaluator/query_evaluator.h"
#include "query_evaluator/core/query_preprocessor.h"

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
  REQUIRE(query->pattern == nullptr);
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
  REQUIRE(query->pattern == nullptr);
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
  REQUIRE(query->pattern == nullptr);
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
  REQUIRE(query->pattern == nullptr);
}

TEST_CASE(
    "Test one assigned one select syntactically correct FOLLOWS such that "
    "Preprocess") {
  SECTION("Follows(stmtref(_),stmtref(_))") {
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
    REQUIRE(query->pattern == nullptr);
  }

  SECTION("Follows(stmtref(int), stmtref(int))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Follows(65,78)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(65);
    QE::StmtOrEntRef a2 = QE::StmtRef(78);
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::Follows, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION("Follows(stmtref(synonym), stmtref(synonym))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Follows(a, b)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::StmtRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::Follows, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION("Follows(stmtref(synonym), entref(\"asd\"))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Follows(a, \"asd\")";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::StmtRef(QE::Synonym::construct("b").value());
    // No such_that - invalid parse
    REQUIRE(query->such_that == nullptr);
    REQUIRE(query->pattern == nullptr);
  }
}

TEST_CASE(
    "Test one assigned one select syntactically correct FOLLOWS such that one "
    "pattern") {
  SECTION("Pattern test QuoteIdent DoubleUnderscoreFactor") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Follows(a, b) pattern p (\"x\", "
        "_\"y\"_)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::StmtRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::Follows, a1, a2).value());

    QE::Synonym syn = QE::Synonym::construct("p").value();
    QE::EntRef entRef = QE::EntRef(QE::QuoteIdent::construct("\"x\"").value());
    QE::ExpressionSpec expr = QE::ExpressionSpec(
        QE::DoubleUnderscoreFactor::construct("_\"y\"_").value());
    auto pat = Pattern::construct(syn, entRef, expr);
    REQUIRE(*(query->pattern) == pat);
  }
  SECTION("Pattern test QuoteIdent UnderScore") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Follows(a, b) pattern p (\"x\", _)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::StmtRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::Follows, a1, a2).value());

    QE::Synonym syn = QE::Synonym::construct("p").value();
    QE::EntRef entRef = QE::EntRef(QE::QuoteIdent::construct("\"x\"").value());
    QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
    auto pat = Pattern::construct(syn, entRef, expr);
    REQUIRE(*(query->pattern) == pat);
  }
  SECTION("Pattern test UnderScore UnderScore") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Follows(a, b) pattern p (_, _)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::StmtRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::Follows, a1, a2).value());

    QE::Synonym syn = QE::Synonym::construct("p").value();
    QE::EntRef entRef = QE::EntRef(QE::Underscore());
    QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
    auto pat = Pattern::construct(syn, entRef, expr);
    REQUIRE(*(query->pattern) == pat);
  }
  SECTION("Pattern test Synonym UnderScore") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Follows(a, b) pattern p (x, _)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::StmtRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::Follows, a1, a2).value());

    QE::Synonym syn = QE::Synonym::construct("p").value();
    QE::EntRef entRef = QE::EntRef(QE::Synonym::construct("x").value());
    QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
    auto pat = Pattern::construct(syn, entRef, expr);
    REQUIRE(*(query->pattern) == pat);
  }
}