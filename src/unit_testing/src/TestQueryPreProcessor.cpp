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
          std::vector<Declaration>{Declaration(DesignEntity::ASSIGN, "p")});
  REQUIRE(*(query->selected_declaration) ==
          Declaration(DesignEntity::ASSIGN, "p"));
}

TEST_CASE("Test two assign one select query Preprocess") {
  auto qp = QE::QueryPreprocessor();
  std::string input = "assign p;stmt s;Select p";
  auto query = qp.getQuery(input);
  REQUIRE(*(query->declarations) ==
          std::vector<Declaration>{Declaration(DesignEntity::ASSIGN, "p"),
                                   Declaration(DesignEntity::STMT, "s")});
  REQUIRE(*(query->selected_declaration) ==
          Declaration(DesignEntity::ASSIGN, "p"));
}

TEST_CASE("Test three assign one select query Preprocess") {
  auto qp = QE::QueryPreprocessor();
  std::string input = "assign p;stmt s;assign q;Select p";
  auto query = qp.getQuery(input);
  REQUIRE(*(query->declarations) ==
          std::vector<Declaration>{Declaration(DesignEntity::ASSIGN, "p"),
                                   Declaration(DesignEntity::STMT, "s"),
                                   Declaration(DesignEntity::ASSIGN, "q")});
  REQUIRE(*(query->selected_declaration) ==
          Declaration(DesignEntity::ASSIGN, "p"));
}