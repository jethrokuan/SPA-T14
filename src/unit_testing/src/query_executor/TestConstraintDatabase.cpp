#include "catch.hpp"

#include <iostream>

#include <vector>

#include "query_executor/constraint_solver/constraint_database.h"

TEST_CASE ("Test Constraint Database single variable add1 functionality") {
  auto db = ConstraintDatabase();
  db.addToSingleVariableConstraints("x", {"1", "2", "3"});
}
