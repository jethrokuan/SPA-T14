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
  db.addToPairedVariableConstraints("x", "y",
                                    {{"1", "2"}, {"4", "5"}, {"777", "777"}});
  std::cout << db;
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "first var for both)") {
  std::cout << "======> JOINED 1-1: \n";
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  db.addToPairedVariableConstraints(
      "x", "z", {{"1", "100"}, {"2", "200"}, {"777", "777"}});
  std::cout << db;
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "second var for existing, first var for incoming)") {
  std::cout << "======> JOINED 2-1: \n";
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("y", "x", {{"2", "1"}, {"3", "2"}});
  db.addToPairedVariableConstraints(
      "x", "z", {{"1", "100"}, {"2", "200"}, {"777", "777"}});
  std::cout << db;
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "second var for both)") {
  std::cout << "======> JOINED 2-2: \n";
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("y", "x", {{"2", "1"}, {"3", "2"}});
  db.addToPairedVariableConstraints(
      "z", "x", {{"100", "1"}, {"200", "2"}, {"777", "777"}});
  std::cout << db;
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "first var for existing, second var for incoming)") {
  std::cout << "======> JOINED 1-2: \n";
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  db.addToPairedVariableConstraints(
      "z", "x", {{"100", "1"}, {"200", "2"}, {"777", "777"}});
  std::cout << db;
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "first var for both) - duplicate values on second paired set") {
  std::cout << "======> JOINED 1-1: \n";
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  db.addToPairedVariableConstraints(
      "x", "z", {{"1", "100"}, {"2", "700"}, {"2", "500"}, {"777", "777"}});
  std::cout << db;
}