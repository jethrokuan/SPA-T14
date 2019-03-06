#include "catch.hpp"

#include <iostream>
#include <set>
#include <vector>

#include "query_executor/constraint_solver/query_constraints.h"

TEST_CASE ("Test Query Constraints swapping functionality") {
  auto qc = QueryConstraints();
  qc.addToSingleVariableConstraints("x", std::set<std::string>{"1", "2", "3"});
  qc.addToSingleVariableConstraints("y", std::set<std::string>{"4", "5", "6"});
  qc.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"3", "4"}});
  qc.addToPairedVariableConstraints("y", "x", {{"3", "1"}, {"4", "3"}});

  SingleVariableConstraints s0 = {"x", std::set<std::string>{"1", "2", "3"}};
  SingleVariableConstraints s1 = {"y", std::set<std::string>{"4", "5", "6"}};
  PairedVariableConstraints s2 = {{"x", "y"}, {{"1", "2"}, {"3", "4"}}};
  PairedVariableConstraints s3 = {{"x", "y"}, {{"1", "3"}, {"3", "4"}}};
  REQUIRE(qc.getSingleVariableConstraintListRef()[0] == s0);
  REQUIRE(qc.getSingleVariableConstraintListRef()[1] == s1);
  REQUIRE(qc.getPairedVariableConstraintListRef()[0] == s2);
  REQUIRE(qc.getPairedVariableConstraintListRef()[1] == s3);
}

TEST_CASE ("Test Query Constraints isVarInallPossibleValues functionality") {
  auto qc = QueryConstraints();
  REQUIRE(qc.isVarInallPossibleValues("x") == false);
  REQUIRE(qc.isVarInallPossibleValues("y") == false);
  qc.addToAllPossibleValues("x", std::set<std::string>{"1", "2", "3"});
  REQUIRE(qc.isVarInallPossibleValues("x") == true);
  REQUIRE(qc.isVarInallPossibleValues("y") == false);

  qc.addToAllPossibleValues("y", std::set<std::string>{"4", "5", "6"});
  REQUIRE(qc.isVarInallPossibleValues("y") == true);
}

TEST_CASE (
    "Test Query Constraints isVarInallPossibleValues functionality for "
    "paired constraints - no change expected") {
  auto qc = QueryConstraints();
  REQUIRE(qc.isVarInallPossibleValues("x") == false);
  REQUIRE(qc.isVarInallPossibleValues("y") == false);
  qc.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"3", "4"}});
  REQUIRE(qc.isVarInallPossibleValues("x") == false);
  REQUIRE(qc.isVarInallPossibleValues("y") == false);
}
