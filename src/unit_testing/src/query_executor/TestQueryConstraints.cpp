#include "catch.hpp"

#include <iostream>

#include <vector>

#include "query_executor/constraint_solver/query_constraints.h"

TEST_CASE ("Test Query Constraints swapping functionality") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints(
      "x", std::unordered_set<std::string>{"1", "2", "3"});
  qc.addToSingleVariableConstraints(
      "y", std::unordered_set<std::string>{"4", "5", "6"});
  qc.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"3", "4"}});
  qc.addToPairedVariableConstraints("y", "x", {{"3", "1"}, {"4", "3"}});

  SingleVariableConstraints s0 = {
      "x", std::unordered_set<std::string>{"1", "2", "3"}};
  SingleVariableConstraints s1 = {
      "y", std::unordered_set<std::string>{"4", "5", "6"}};
  PairedVariableConstraints s4 = {{"x", "y"}, {{"3", "4"}}};
  REQUIRE(qc.getSingleVariableConstraintMapRef()[s0.first] == s0.second);
  REQUIRE(qc.getSingleVariableConstraintMapRef()[s1.first] == s1.second);
  REQUIRE(qc.getPairedVariableConstraintMapRef()[s4.first] == s4.second);
}

TEST_CASE ("Test Query Constraints isVarInAllPossibleValues functionality") {
  auto qc = QueryConstraints();
  REQUIRE(qc.isVarInAllPossibleValues("x") == false);
  REQUIRE(qc.isVarInAllPossibleValues("y") == false);
  qc.addToAllPossibleValues("x",
                            std::unordered_set<std::string>{"1", "2", "3"});
  REQUIRE(qc.isVarInAllPossibleValues("x") == true);
  REQUIRE(qc.isVarInAllPossibleValues("y") == false);

  qc.addToAllPossibleValues("y",
                            std::unordered_set<std::string>{"4", "5", "6"});
  REQUIRE(qc.isVarInAllPossibleValues("y") == true);
}

TEST_CASE (
    "Test Query Constraints isVarInAllPossibleValues functionality for "
    "paired constraints - no change expected") {
  auto qc = QueryConstraints();
  REQUIRE(qc.isVarInAllPossibleValues("x") == false);
  REQUIRE(qc.isVarInAllPossibleValues("y") == false);
  qc.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"3", "4"}});
  REQUIRE(qc.isVarInAllPossibleValues("x") == false);
  REQUIRE(qc.isVarInAllPossibleValues("y") == false);
}
