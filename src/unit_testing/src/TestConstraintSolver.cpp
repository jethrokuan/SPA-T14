#include "catch.hpp"

#include <iostream>
#include <set>
#include <vector>

#include "query_executor/constraint_solver/constraint_solver.h"

TEST_CASE ("Test Constraint Solver with no constraints") {
  auto qc = QueryConstraints();
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
          std::vector<std::string>{});
}

TEST_CASE ("Test Constraint Solver with one single-var constraint") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints("x", std::set<std::string>{"1", "2", "3"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
          std::vector<std::string>{"1", "2", "3"});
}

TEST_CASE (
    "Test Constraint Solver with two single-var constraints not affecting each "
    "other") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints("x", std::set<std::string>{"1", "2", "3"});
  qc.addToSingleVariableConstraints("y", std::set<std::string>{"1", "2", "3"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
          std::vector<std::string>{"1", "2", "3"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "y") ==
          std::vector<std::string>{"1", "2", "3"});
}

TEST_CASE (
    "Test Constraint Solver with two single-var constraints affecting each "
    "other") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints("x", std::set<std::string>{"1", "2", "3"});
  qc.addToSingleVariableConstraints("x", std::set<std::string>{"1", "2"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
          std::vector<std::string>{"1", "2"});
}

TEST_CASE (
    "Test Constraint Solver with one single-var constraint and one paired "
    "constraint not affecting each other") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints("x", std::set<std::string>{"1", "2", "3"});
  qc.addToPairedVariableConstraints(
      "y", "z",
      std::set<std::pair<std::string, std::string>>{{"1", "2"}, {"3", "4"}});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
          std::vector<std::string>{"1", "2", "3"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "y") ==
          std::vector<std::string>{"1", "3"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "z") ==
          std::vector<std::string>{"2", "4"});
}

TEST_CASE (
    "Test Constraint Solver with one single-var constraint and one paired "
    "constraint affecting each other") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints("x", std::set<std::string>{"1", "2", "3"});
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::set<std::pair<std::string, std::string>>{{"1", "2"}, {"3", "4"}});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
          std::vector<std::string>{"1", "3"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "z") ==
          std::vector<std::string>{"2", "4"});
}
