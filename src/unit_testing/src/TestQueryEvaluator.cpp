#include "catch.hpp"

#include "query_evaluator/query_evaluator.h"

TEST_CASE("Test Empty Query") {
    QE::QueryEvaluator qeval = QE::QueryEvaluator();
    std::string input = ""; 
    qeval.makePqlQuery(input);
    REQUIRE(1 == 2);
}