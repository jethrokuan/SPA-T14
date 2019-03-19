#include "catch.hpp"

#include <iostream>

#include <vector>

#include "query_executor/constraint_solver/constraint_solver.h"

TEST_CASE ("Test Constraint Solver with no constraints") {
  auto qc = QueryConstraints();
  REQUIRE_THROWS_AS(ConstraintSolver::constrainAndSelect(qc, "a") ==
                        std::vector<std::string>{},
                    std::runtime_error);
}

TEST_CASE ("Test Constraint Solver with one single-var constraint") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints(
      "x", std::unordered_set<std::string>{"1", "2", "3"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
          std::vector<std::string>{"1", "2", "3"});
}

TEST_CASE (
    "Test Constraint Solver with two single-var constraints not affecting each "
    "other") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints(
      "x", std::unordered_set<std::string>{"1", "2", "3"});
  qc.addToSingleVariableConstraints(
      "y", std::unordered_set<std::string>{"1", "2", "3"});
  SECTION ("Select x") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
            std::vector<std::string>{"1", "2", "3"});
  }
  SECTION ("Select y") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "y") ==
            std::vector<std::string>{"1", "2", "3"});
  }
}

TEST_CASE (
    "Test Constraint Solver with two single-var constraints affecting each "
    "other") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints(
      "x", std::unordered_set<std::string>{"1", "2", "3"});
  qc.addToSingleVariableConstraints("x",
                                    std::unordered_set<std::string>{"1", "2"});
  REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
          std::vector<std::string>{"1", "2"});
}

TEST_CASE (
    "Test Constraint Solver with one single-var constraint and one paired "
    "constraint not affecting each other") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints(
      "x", std::unordered_set<std::string>{"1", "2", "3"});
  qc.addToPairedVariableConstraints(
      "y", "z",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "2"}, {"3", "4"}});
  SECTION ("Select x") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
            std::vector<std::string>{"1", "2", "3"});
  }
  SECTION ("Select y") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "y") ==
            std::vector<std::string>{"1", "3"});
  }
  SECTION ("Select z") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "z") ==
            std::vector<std::string>{"2", "4"});
  }
}

TEST_CASE (
    "Test Constraint Solver with one single-var constraint and one paired "
    "constraint affecting each other") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints(
      "x", std::unordered_set<std::string>{"1", "2", "3"});
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "2"}, {"3", "4"}});
  SECTION ("Select x") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
            std::vector<std::string>{"1", "3"});
  }
  SECTION ("Select z") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "z") ==
            std::vector<std::string>{"2", "4"});
  }
}

TEST_CASE (
    "Test Constraint Solver with two paired "
    "constraints not affecting each other") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "2"}, {"3", "4"}});
  qc.addToPairedVariableConstraints(
      "a", "b",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "2"}, {"3", "4"}});
  SECTION ("Select x") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
            std::vector<std::string>{"1", "3"});
  }
  SECTION ("Select z") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "z") ==
            std::vector<std::string>{"2", "4"});
  }
  SECTION ("Select a") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
            std::vector<std::string>{"1", "3"});
  }
  SECTION ("Select b") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "b") ==
            std::vector<std::string>{"2", "4"});
  }
}

TEST_CASE (
    "Test Constraint Solver with two paired "
    "constraints affecting each other") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "2"}, {"2", "4"}});
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "2"}, {"1", "3"}});

  SECTION ("Select x") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
            std::vector<std::string>{"1"});
  }
  SECTION ("Select z") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "z") ==
            std::vector<std::string>{"2"});
  }
}

TEST_CASE (
    "Test Constraint Solver with three paired "
    "constraints affecting each other") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "2"}, {"2", "4"}});
  qc.addToPairedVariableConstraints(
      "x", "z",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "2"}, {"1", "3"}});
  qc.addToPairedVariableConstraints(
      "x", "a",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "2"}, {"2", "4"}});
  SECTION ("Select x") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "x") ==
            std::vector<std::string>{"1"});
  }
  SECTION ("Select z") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "z") ==
            std::vector<std::string>{"2"});
  }
  SECTION ("Select a") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
            std::vector<std::string>{"2"});
  };
}

TEST_CASE (
    "Test Issue #170 - paired constraints affecting each other + a single "
    "constraint") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "a", "v",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"2", "v1"}, {"4", "v1"}, {"5", "v2"}});
  qc.addToPairedVariableConstraints(
      "a", "v",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"2", "v2"}, {"4", "v1"}, {"5", "v2"}});
  qc.addToSingleVariableConstraints(
      "a", std::unordered_set<std::string>{{"2", "4", "5"}});
  SECTION ("Select a") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
            std::vector<std::string>{"4", "5"});
  }
  SECTION ("Select v") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "v") ==
            std::vector<std::string>{"v1", "v2"});
  };
}

TEST_CASE (
    "Test Issue #170+ - paired constraints affecting each other + a single "
    "constraint also affecting the result") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "a", "v",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"2", "v1"}, {"4", "v1"}, {"5", "v2"}});
  qc.addToPairedVariableConstraints(
      "a", "v",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"2", "v2"}, {"4", "v1"}, {"5", "v2"}});
  qc.addToSingleVariableConstraints("a",
                                    std::unordered_set<std::string>{"2", "5"});
  SECTION ("Select a") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
            std::vector<std::string>{"5"});
  }
  SECTION ("Select v") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "v") ==
            std::vector<std::string>{"v2"});
  };
}

TEST_CASE (
    "Test Issue #170 - paired constraints affecting each other + a single "
    "constraint + order sensitivity") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "a", "v",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"2", "v1"}, {"4", "v1"}, {"5", "v2"}});
  qc.addToPairedVariableConstraints(
      "v", "a",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"v2", "a"}, {"v1", "4"}, {"v2", "5"}});
  qc.addToSingleVariableConstraints(
      "a", std::unordered_set<std::string>{{"2", "4", "5"}});
  SECTION ("Select a") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "a") ==
            std::vector<std::string>{"4", "5"});
  }
  SECTION ("Select v") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "v") ==
            std::vector<std::string>{"v1", "v2"});
  };
}

TEST_CASE ("Test Issue #174 - can we handle iterative constraints?") {
  // Why does this pass? We need a more convoluted case.
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "s1", "s2",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"100", "200"}, {"300", "201"}, {"100", "203"}, {"301", "204"}});
  qc.addToPairedVariableConstraints(
      "s2", "s3",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"200", "1"}, {"201", "2"}, {"202", "7"}});
  qc.addToPairedVariableConstraints(
      "s3", "s1",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "100"}, {"2", "101"}, {"3", "102"}});
  SECTION ("Select s1") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "s1") ==
            std::vector<std::string>{"100"});
  }
  SECTION ("Select s2") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "s2") ==
            std::vector<std::string>{"200"});
  };
  SECTION ("Select s3") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "s3") ==
            std::vector<std::string>{"1"});
  };
}

TEST_CASE ("Test Issue #174 - can we handle complex iterative constraints?") {
  // Why does this pass? We need a more convoluted case.
  auto qc = QueryConstraints();
  // (101, 201) (102, 201) depends on 201 existing
  // (201, 2) and (201, 3) depend on 3 existing
  // (2, 101) and (3, 103) depend on 101 and 103 existing. 103 does not

  // So (3, 103) cannot exist --> 3 does not exist
  // 3 does not exist --> (201, 3) does not exist
  //
  qc.addToPairedVariableConstraints(
      "s1", "s2",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"100", "200"}, {"101", "201"}, {"102", "201"}});
  qc.addToPairedVariableConstraints(
      "s2", "s3",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"200", "1"}, {"201", "3"}});
  qc.addToPairedVariableConstraints(
      "s3", "s1",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "100"}, {"2", "101"}, {"3", "103"}});

  SECTION ("Select s1") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "s1") ==
            std::vector<std::string>{"100"});
  }
  SECTION ("Select s2") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "s2") ==
            std::vector<std::string>{"200"});
  };
  SECTION ("Select s3") {
    REQUIRE(ConstraintSolver::constrainAndSelect(qc, "s3") ==
            std::vector<std::string>{"1"});
  };
}

TEST_CASE ("Test multi-variable select from ConstraintSolver") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints(
      "x", std::unordered_set<std::string>{"1", "2", "3"});
  qc.addToSingleVariableConstraints(
      "y", std::unordered_set<std::string>{"4", "5", "6"});
  SECTION ("Select <x, y>") {
    REQUIRE(ConstraintSolver::constrainAndSelect(
                qc, std::vector<std::string>{"x", "y"}) ==
            std::vector<std::vector<std::string>>{{"1", "2", "3"},
                                                  {"4", "5", "6"}});
  }
}

TEST_CASE (
    "Test multi-variable select from ConstraintSolver with one var being "
    "constrained") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints(
      "x", std::unordered_set<std::string>{"1", "2", "3"});
  qc.addToSingleVariableConstraints("x",
                                    std::unordered_set<std::string>{"1", "2"});
  qc.addToSingleVariableConstraints(
      "y", std::unordered_set<std::string>{"4", "5", "6"});
  SECTION ("Select <x, y>") {
    REQUIRE(ConstraintSolver::constrainAndSelect(
                qc, std::vector<std::string>{"x", "y"}) ==
            std::vector<std::vector<std::string>>{{"1", "2"}, {"4", "5", "6"}});
  }
}

TEST_CASE (
    "Test multi-variable select from ConstraintSolver with one paired "
    "constraint") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "s3", "s1",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "100"}, {"2", "101"}, {"3", "103"}});
  SECTION ("Select <x, y>") {
    REQUIRE(ConstraintSolver::constrainAndSelect(
                qc, std::vector<std::string>{"s1", "s3"}) ==
            std::vector<std::vector<std::string>>{{"100", "101", "103"},
                                                  {"1", "2", "3"}});
  }
}

TEST_CASE (
    "Test multi-variable select from ConstraintSolver with two paired "
    "constraints affecting each other") {
  auto qc = QueryConstraints();
  qc.addToPairedVariableConstraints(
      "s3", "s1",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "100"}, {"2", "101"}, {"3", "103"}});
  qc.addToPairedVariableConstraints(
      "s3", "s1",
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>{
          {"1", "100"}, {"2", "101"}});
  SECTION ("Select <x, y>") {
    REQUIRE(ConstraintSolver::constrainAndSelect(
                qc, std::vector<std::string>{"s1", "s3"}) ==
            std::vector<std::vector<std::string>>{{"100", "101"}, {"1", "2"}});
  }
}
