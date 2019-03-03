#include "catch.hpp"

#include <iostream>
#include <vector>

#include "query_builder/core/exceptions.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"

using namespace QE;

TEST_CASE ("Test Query Builder stub functionality") {
  auto qe = QueryBuilder();
  std::string query_string =
      "procedure p; assign a; Select p such that Uses(p, \"c\") pattern a "
      "(_,_)";
  auto query_result = qe.makePqlQuery(query_string);
  REQUIRE(query_result != nullptr);
}