#include "catch.hpp"

#include <iostream>
#include <vector>

#include "query_evaluator/core/exceptions.h"
#include "query_evaluator/pql/pql.h"
#include "query_evaluator/query_evaluator.h"

using namespace QE;

TEST_CASE ("Test Query Evaluator stub functionality") {
  auto qe = QueryEvaluator();
  std::string query_string =
      "procedure p; assign a; Select p such that Uses(p, \"c\") pattern a "
      "(_,_)";
  auto query_result = qe.makePqlQuery(query_string);
  REQUIRE(query_result != nullptr);
}