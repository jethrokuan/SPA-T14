#include "catch.hpp"

#include <iostream>
#include <set>
#include <vector>

#include "query_executor/constraint_solver/query_constraints.h"

TEST_CASE ("Test Query Constraints stub functionality") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints("x", std::set<std::string>{"1", "2", "3"});
  qc.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"3", "4"}});
  qc.addToPairedVariableConstraints("y", "x", {{"3", "1"}, {"4", "3"}});
  std::cout << qc;
  REQUIRE(1 == 1);
}
