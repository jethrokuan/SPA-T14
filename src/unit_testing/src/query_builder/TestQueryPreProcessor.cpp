#include "catch.hpp"

#include <iostream>
#include <vector>
#include "query_builder/core/exceptions.h"
#include "query_builder/core/query_preprocessor.h"
#include "query_builder/query_builder.h"

using namespace QE;
TEST_CASE ("Test one entity one select query Preprocess") {
  SECTION ("Test one assign one select query") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign a;Select a";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("a").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()));
    REQUIRE(query->such_that == nullptr);
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Test one variable one select query") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "variable v;Select v";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::VARIABLE, QE::Synonym::construct("v").value())});
    REQUIRE(*(query->selected_declaration) ==
            Declaration(DesignEntity::VARIABLE,
                        QE::Synonym::construct("v").value()));
    REQUIRE(query->such_that == nullptr);
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Test one stmt one select query") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "stmt p;Select p";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::STMT, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::STMT, QE::Synonym::construct("p").value()));
    REQUIRE(query->such_that == nullptr);
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Test one procedure one select query") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "procedure p;Select p";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::PROCEDURE, QE::Synonym::construct("p").value())});
    REQUIRE(*(query->selected_declaration) ==
            Declaration(DesignEntity::PROCEDURE,
                        QE::Synonym::construct("p").value()));
    REQUIRE(query->such_that == nullptr);
    REQUIRE(query->pattern == nullptr);
  }
}

TEST_CASE ("Test multi-declaration one select query Preprocess") {
  SECTION ("Test two assign one select query") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign a, a1;Select a";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a").value()),
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a1").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()));
    REQUIRE(query->such_that == nullptr);
    REQUIRE(query->pattern == nullptr);
  }
  SECTION ("Test two assign compressed whitespace one select query") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign a,a1;Select a";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a").value()),
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a1").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()));
    REQUIRE(query->such_that == nullptr);
    REQUIRE(query->pattern == nullptr);
  }
  SECTION ("Test three assign one select query") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign a, a1, a2;Select a";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a").value()),
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a1").value()),
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("a2").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("a").value()));
    REQUIRE(query->such_that == nullptr);
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Test three stmt one select query") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "stmt a, a1, a2;Select a";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::STMT,
                            QE::Synonym::construct("a").value()),
                Declaration(DesignEntity::STMT,
                            QE::Synonym::construct("a1").value()),
                Declaration(DesignEntity::STMT,
                            QE::Synonym::construct("a2").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::STMT, QE::Synonym::construct("a").value()));
    REQUIRE(query->such_that == nullptr);
    REQUIRE(query->pattern == nullptr);
  }
}

TEST_CASE ("Test two assign one select query Preprocess") {
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

TEST_CASE ("Test three assign one select query Preprocess") {
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

TEST_CASE (
    "Test one assigned one select syntactically correct FOLLOWS such that "
    "Preprocess") {
  SECTION ("Follows(stmtref(_),stmtref(_))") {
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

  SECTION ("Follows(stmtref(int), stmtref(int))") {
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

  SECTION ("Follows(stmtref(synonym), stmtref(synonym))") {
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

  SECTION ("Follows(stmtref(synonym), entref(\"asd\"))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Follows(a, \"asd\")";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }
}

TEST_CASE (
    "Test one assigned one select syntactically correct PARENT such that "
    "Preprocess") {
  SECTION ("Parent(stmtref(_),stmtref(_))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Parent(_,_)";
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
            SuchThat::construct(Relation::Parent, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Parent(stmtref(int), stmtref(int))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Parent(65,78)";
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
            SuchThat::construct(Relation::Parent, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Parent(stmtref(synonym), stmtref(synonym))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Parent(a, b)";
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
            SuchThat::construct(Relation::Parent, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Parent(stmtref(synonym), entref(\"asd\"))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Parent(a, \"asd\")";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }
}

TEST_CASE (
    "Test one assigned one select syntactically correct MODIFIES such that "
    "Preprocess") {
  SECTION ("Modifies(stmtref(int), entref(\"asd\"))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Modifies(65, \"asd\")";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(65);
    QE::StmtOrEntRef a2 =
        QE::EntRef(QE::QuoteIdent::construct("\"asd\"").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::ModifiesS, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Modifies(stmtref(synonym), entref(\"asd\"))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Modifies(a, \"asd\")";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 =
        QE::EntRef(QE::QuoteIdent::construct("\"asd\"").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::ModifiesS, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Modifies(stmtref(synonym), entref(synonym))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Modifies(a, b)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::EntRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::ModifiesS, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Modifies(stmtref(synonym), stmtref(int))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Modifies(a, 65)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }
}

TEST_CASE (
    "Test one assigned one select syntactically correct USES such that "
    "Preprocess") {
  SECTION ("Uses(stmtref(int), entref(\"asd\"))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Uses(65, \"asd\")";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(65);
    QE::StmtOrEntRef a2 =
        QE::EntRef(QE::QuoteIdent::construct("\"asd\"").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::UsesS, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Uses(stmtref(synonym), entref(\"asd\"))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Uses(a, \"asd\")";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 =
        QE::EntRef(QE::QuoteIdent::construct("\"asd\"").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::UsesS, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Uses(stmtref(synonym), entref(synonym))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Uses(a, b)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::EntRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::UsesS, a1, a2).value());
    REQUIRE(query->pattern == nullptr);
  }

  SECTION ("Uses(stmtref(synonym), stmtref(int))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Uses(a, 65)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }
}

TEST_CASE (
    "Test one assigned one select syntactically correct FOLLOWS such that one "
    "pattern") {
  SECTION ("Pattern test QuoteIdent DoubleUnderscoreFactor") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p; stmt a, b;Select p such that Follows(a, b) pattern p "
        "(\"x\", "
        "_\"y\"_)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("p").value()),
                Declaration(DesignEntity::STMT,
                            QE::Synonym::construct("a").value()),
                Declaration(DesignEntity::STMT,
                            QE::Synonym::construct("b").value()),
            });
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
  SECTION ("Pattern test UnderScore QuoteIdent") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Follows(a, b) pattern p (_, _\"y\"_)";
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
    QE::ExpressionSpec expr = QE::ExpressionSpec(
        QE::DoubleUnderscoreFactor::construct("_\"y\"_").value());
    auto pat = Pattern::construct(syn, entRef, expr);
    REQUIRE(*(query->pattern) == pat);
  }
  SECTION ("Pattern test QuoteIdent UnderScore") {
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
  SECTION ("Pattern test UnderScore UnderScore") {
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
  SECTION ("Pattern test Synonym UnderScore") {
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

  SECTION ("Pattern test DoubleUnderscoreFactor UnderScore") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Follows(a, b) pattern p (_\"y\"_, _)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }
}

TEST_CASE (
    "Test one assigned one select syntactically correct USES such that one "
    "pattern") {
  SECTION ("Pattern test QuoteIdent DoubleUnderscoreFactor") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p; stmt a, b; Select p such that Uses(a, b) pattern p (\"x\", "
        "_\"y\"_)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::ASSIGN,
                            QE::Synonym::construct("p").value()),
                Declaration(DesignEntity::STMT,
                            QE::Synonym::construct("a").value()),
                Declaration(DesignEntity::STMT,
                            QE::Synonym::construct("b").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::EntRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::UsesS, a1, a2).value());

    QE::Synonym syn = QE::Synonym::construct("p").value();
    QE::EntRef entRef = QE::EntRef(QE::QuoteIdent::construct("\"x\"").value());
    QE::ExpressionSpec expr = QE::ExpressionSpec(
        QE::DoubleUnderscoreFactor::construct("_\"y\"_").value());
    auto pat = Pattern::construct(syn, entRef, expr);
    REQUIRE(*(query->pattern) == pat);
  }
  SECTION ("Pattern test UnderScore QuoteIdent") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Uses(a, b) pattern p (_, _\"y\"_)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::EntRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::UsesS, a1, a2).value());

    QE::Synonym syn = QE::Synonym::construct("p").value();
    QE::EntRef entRef = QE::EntRef(QE::Underscore());
    QE::ExpressionSpec expr = QE::ExpressionSpec(
        QE::DoubleUnderscoreFactor::construct("_\"y\"_").value());
    auto pat = Pattern::construct(syn, entRef, expr);
    REQUIRE(*(query->pattern) == pat);
  }
  SECTION ("Pattern test QuoteIdent UnderScore") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Uses(a, b) pattern p (\"x\", _)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::EntRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::UsesS, a1, a2).value());

    QE::Synonym syn = QE::Synonym::construct("p").value();
    QE::EntRef entRef = QE::EntRef(QE::QuoteIdent::construct("\"x\"").value());
    QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
    auto pat = Pattern::construct(syn, entRef, expr);
    REQUIRE(*(query->pattern) == pat);
  }
  SECTION ("Pattern test UnderScore UnderScore") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Uses(a, b) pattern p (_, _)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::EntRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::UsesS, a1, a2).value());

    QE::Synonym syn = QE::Synonym::construct("p").value();
    QE::EntRef entRef = QE::EntRef(QE::Underscore());
    QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
    auto pat = Pattern::construct(syn, entRef, expr);
    REQUIRE(*(query->pattern) == pat);
  }
  SECTION ("Pattern test Synonym UnderScore") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Uses(a, b) pattern p (x, _)";
    auto query = qp.getQuery(input);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{Declaration(
                DesignEntity::ASSIGN, QE::Synonym::construct("p").value())});
    REQUIRE(
        *(query->selected_declaration) ==
        Declaration(DesignEntity::ASSIGN, QE::Synonym::construct("p").value()));

    QE::StmtOrEntRef a1 = QE::StmtRef(QE::Synonym::construct("a").value());
    QE::StmtOrEntRef a2 = QE::EntRef(QE::Synonym::construct("b").value());
    REQUIRE(*(query->such_that) ==
            SuchThat::construct(Relation::UsesS, a1, a2).value());

    QE::Synonym syn = QE::Synonym::construct("p").value();
    QE::EntRef entRef = QE::EntRef(QE::Synonym::construct("x").value());
    QE::ExpressionSpec expr = QE::ExpressionSpec(QE::Underscore());
    auto pat = Pattern::construct(syn, entRef, expr);
    REQUIRE(*(query->pattern) == pat);
  }

  SECTION ("Pattern test DoubleUnderscoreFactor UnderScore") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Follows(a, b) pattern p (_\"y\"_, _)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Pattern test Pattern a(_, \"+a\")") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Follows(a, b) pattern a(_, \"+a\")";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }
}

// This should use REQUIRE_THROWS_MATCHES, but I cannot get the matcher to work
// It cannot seem to convert my exception class to a string.
// I've tried both to_string and operator<< implementations, no luck
TEST_CASE ("Test Preprocess Exceptions") {
  SECTION ("Test Declaration exception for invalid Design Entity") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "invalid_design_entity p;Select p";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Test Declaration exception for invalid synonym") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign @;Select p";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Test Declaration exception for != 2 select tokens") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLTokenizeException);
  }

  SECTION ("Test Declaration exception for no select tokens") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLTokenizeException);
  }

  SECTION (
      "Test Semantic exception for no matching declaration for Select "
      "synonym") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select q";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Test Semantic exception for no matching relation for such_that") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Followz(p, q)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Test malformed such that query Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Follows(6++=sss| , s23123|)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Test malformed such that first arg query Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Follows(6++=sss|,_)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Test malformed such that second arg query Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Follows(_,6++=sss|)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Test malformed pattern - no other arguments") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p pattern";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLTokenizeException);
  }

  SECTION ("Test malformed pattern - not synonym") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p pattern @ (x, _)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Test malformed pattern - first argument") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p pattern a (@, _)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Test malformed pattern - second argument") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p pattern a (_, @)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLParseException);
  }

  SECTION ("Test multiple such that query Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input =
        "assign p;Select p such that Follows(p , q) such that Modifies(p , q)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLTokenizeException);
  }

  SECTION ("Test multiple pattern query Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p pattern a (x, _) pattern b (y, _)";
    REQUIRE_THROWS_AS(qp.getQuery(input), QE::PQLTokenizeException);
  }
}

TEST_CASE ("Test Preprocess Whitespace Sensitivity") {
  SECTION (
      "Test one assign one select query whitespace before select Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p; Select p";
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

  SECTION (
      "Test one assign one select query whitespace after select Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p ";
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

  SECTION (
      "Test one assign one select query whitespace before+after select "
      "Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p; Select p ";
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

  SECTION (
      "Test one assign one select query whitespace before assign "
      "Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input = " assign p;Select p";
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
  SECTION (
      "Test one assign one select query whitespace after assign "
      "Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p ;Select p";
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

  SECTION (
      "Test one assign one select query whitespace before+after assign "
      "Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input = " assign p ;Select p";
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

  SECTION (
      "Test one assign one select one such that with whitespace before"
      "Follows(stmtref(_),stmtref(_))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p   such that Follows(_,_)";
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

  SECTION (
      "Test one assign one select one such that with whitespace after "
      "Follows(stmtref(_),stmtref(_))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p such that Follows(_,_)   ";
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

  SECTION (
      "Test one assign one select one such that with whitespace before+after "
      "Follows(stmtref(_),stmtref(_))") {
    auto qp = QE::QueryPreprocessor();
    std::string input = "assign p;Select p    such that Follows(_,_)   ";
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
}