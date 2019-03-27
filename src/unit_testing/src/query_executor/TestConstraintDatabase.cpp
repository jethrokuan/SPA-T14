#include "catch.hpp"

#include <iostream>

#include <vector>

#include "query_executor/constraint_solver/constraint_database.h"

TEST_CASE ("Test Constraint Database single variable add functionality") {
  std::cout << "======> SINGLE VAR ADD: \n";
  auto db = ConstraintDatabase();
  db.addToSingleVariableConstraints("x", {"1", "2", "3"});
  std::cout << db;
}

TEST_CASE ("Test Constraint Database paired variable add functionality") {
  std::cout << "======> PAIRED VAR ADD: \n";
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  std::cout << db;
}

TEST_CASE (
    "Test Constraint Database intersect single variable constraint x 2") {
  std::cout << "======> SINGLE x 2: \n";
  auto db = ConstraintDatabase();
  db.addToSingleVariableConstraints("x", {"1", "2", "3"});
  db.addToSingleVariableConstraints("x", {"1", "2", "4"});
  std::cout << db;
}

TEST_CASE (
    "Test Constraint Database intersect paired variable constraint x 2") {
  std::cout << "======> PAIRED x 2: \n";
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"4", "5"}});
  std::cout << db;
}
