#include "catch.hpp"

#include "query_evaluator/query_evaluator.h"
#include "query_evaluator/query_preprocessor.h"
#include "query_evaluator/pql.h"
#include <vector>
#include <iostream>

TEST_CASE("Test Empty Query Preprocess") {
    auto qp = QE::QueryPreprocessor();
    std::string input = ""; 
    auto query = qp.getQuery(input);
    //REQUIRE(query->getDeclarations().empty() == true);
    REQUIRE(query->getDeclarations()->size() == 0);
    // REQUIRE(query->getSelectedDeclarations() == QE::Declaration());
    REQUIRE(query->getSuchThat() == nullptr);
    REQUIRE(query->getPattern() == nullptr);
}