#include "catch.hpp"

#include <iostream>

#include <vector>

#include "query_executor/constraint_solver/constraint_database.h"

TEST_CASE ("Test Constraint Database single variable add functionality") {
  auto db = ConstraintDatabase();
  db.addToSingleVariableConstraints("x", {"1", "2", "3"});
  std::cout << db;
}

TEST_CASE ("Test Constraint Database paired variable add functionality") {
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  std::cout << db;
}

TEST_CASE ("Test Constraint Database same variable constraint x 2") {
  auto db = ConstraintDatabase();
  db.addToSingleVariableConstraints("x", {"1", "2", "3"});
  db.addToSingleVariableConstraints("x", {"1", "2"});
  std::cout << db;
}
