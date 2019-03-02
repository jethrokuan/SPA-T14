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

TEST_CASE (
    "Test Constraint Solver with two paired "
    "constraints not affecting each other") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::set<std::pair<std::string, std::string>>{{"1", "2"}, {"3", "4"}});
  qc.addToPairedVariableConstraints(
      "a", "b",
      std::set<std::pair<std::string, std::string>>{{"1", "2"}, {"3", "4"}});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
          std::vector<std::string>{"1", "3"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "z") ==
          std::vector<std::string>{"2", "4"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
          std::vector<std::string>{"1", "3"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "b") ==
          std::vector<std::string>{"2", "4"});
}

TEST_CASE (
    "Test Constraint Solver with two paired "
    "constraints affecting each other") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::set<std::pair<std::string, std::string>>{{"1", "2"}, {"2", "4"}});
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::set<std::pair<std::string, std::string>>{{"1", "2"}, {"1", "3"}});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
          std::vector<std::string>{"1"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "z") ==
          std::vector<std::string>{"2"});
}

TEST_CASE (
    "Test Constraint Solver with three paired "
    "constraints affecting each other") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::set<std::pair<std::string, std::string>>{{"1", "2"}, {"2", "4"}});
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::set<std::pair<std::string, std::string>>{{"1", "2"}, {"1", "3"}});
  qc.addToPairedVariableConstraints(
      "x", "a",
      std::set<std::pair<std::string, std::string>>{{"1", "2"}, {"2", "4"}});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
          std::vector<std::string>{"1"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "z") ==
          std::vector<std::string>{"2"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
          std::vector<std::string>{"2"});
}

TEST_CASE (
    "Test Issue #170 - paired constraints affecting each other + a single "
    "constraint") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "a", "v",
      std::set<std::pair<std::string, std::string>>{
          {"2", "v1"}, {"4", "v1"}, {"5", "v2"}});
  qc.addToPairedVariableConstraints(
      "a", "v",
      std::set<std::pair<std::string, std::string>>{
          {"2", "v2"}, {"4", "v1"}, {"5", "v2"}});
  qc.addToSingleVariableConstraints("a",
                                    std::set<std::string>{{"2", "4", "5"}});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
          std::vector<std::string>{"4", "5"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "v") ==
          std::vector<std::string>{"v1", "v2"});
}

TEST_CASE (
    "Test Issue #170+ - paired constraints affecting each other + a single "
    "constraint also affecting the result") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "a", "v",
      std::set<std::pair<std::string, std::string>>{
          {"2", "v1"}, {"4", "v1"}, {"5", "v2"}});
  qc.addToPairedVariableConstraints(
      "a", "v",
      std::set<std::pair<std::string, std::string>>{
          {"2", "v2"}, {"4", "v1"}, {"5", "v2"}});
  qc.addToSingleVariableConstraints("a", std::set<std::string>{"2", "5"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
          std::vector<std::string>{"5"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "v") ==
          std::vector<std::string>{"v2"});
}

TEST_CASE (
    "Test Issue #170 - paired constraints affecting each other + a single "
    "constraint + order sensitivity") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "a", "v",
      std::set<std::pair<std::string, std::string>>{
          {"2", "v1"}, {"4", "v1"}, {"5", "v2"}});
  qc.addToPairedVariableConstraints(
      "v", "a",
      std::set<std::pair<std::string, std::string>>{
          {"v2", "a"}, {"v1", "4"}, {"v2", "5"}});
  qc.addToSingleVariableConstraints("a",
                                    std::set<std::string>{{"2", "4", "5"}});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
          std::vector<std::string>{"4", "5"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "v") ==
          std::vector<std::string>{"v1", "v2"});
}

TEST_CASE ("Test Issue #174 - can we handle iterative constraints?") {
  // Why does this pass? We need a more convoluted case.
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "s1", "s2",
      std::set<std::pair<std::string, std::string>>{
          {"100", "200"}, {"300", "201"}, {"100", "203"}, {"301", "204"}});
  qc.addToPairedVariableConstraints(
      "s2", "s3",
      std::set<std::pair<std::string, std::string>>{
          {"200", "1"}, {"201", "2"}, {"202", "7"}});
  qc.addToPairedVariableConstraints(
      "s3", "s1",
      std::set<std::pair<std::string, std::string>>{
          {"1", "100"}, {"2", "101"}, {"3", "102"}});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "s1") ==
          std::vector<std::string>{"100"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "s2") ==
          std::vector<std::string>{"200"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "s3") ==
          std::vector<std::string>{"1"});
}
