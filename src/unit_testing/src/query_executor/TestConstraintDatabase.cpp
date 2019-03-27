#include "catch.hpp"

#include <iostream>

#include <vector>

#include "query_executor/constraint_solver/constraint_database.h"

TEST_CASE ("Test Constraint Database single variable add functionality") {
  auto db = ConstraintDatabase();
  db.addToSingleVariableConstraints("x", {"1", "2", "3"});
  REQUIRE(db.selectOne("x") == std::vector<std::string>{"1", "2", "3"});
  REQUIRE(db.selectMultiple({"x"}) == std::vector<std::string>{"1", "2", "3"});
  REQUIRE(db.selectBoolean() == true);
}

TEST_CASE ("Test Constraint Database paired variable add functionality") {
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  REQUIRE(db.selectBoolean() == true);
  REQUIRE(db.selectOne("x") == std::vector<std::string>{"1", "2"});
  REQUIRE(db.selectOne("y") == std::vector<std::string>{"2", "3"});
  REQUIRE(db.selectMultiple({"x", "y"}) ==
          std::vector<std::string>{"1 2", "2 3"});
  REQUIRE(db.selectMultiple({"y", "x"}) ==
          std::vector<std::string>{"2 1", "3 2"});
}

TEST_CASE (
    "Test Constraint Database intersect single variable constraint x 2") {
  auto db = ConstraintDatabase();
  db.addToSingleVariableConstraints("x", {"1", "2", "3"});
  db.addToSingleVariableConstraints("x", {"1", "2", "4"});
  REQUIRE(db.selectOne("x") == std::vector<std::string>{"1", "2"});
  REQUIRE(db.selectMultiple({"x"}) == std::vector<std::string>{"1", "2"});
  REQUIRE(db.selectBoolean() == true);
}

TEST_CASE (
    "Test Constraint Database intersect disjoint single variable constraint x "
    "2") {
  auto db = ConstraintDatabase();
  db.addToSingleVariableConstraints("x", {"1", "2", "3"});
  db.addToSingleVariableConstraints("x", {"7", "8", "9"});
  REQUIRE(db.selectOne("x") == std::vector<std::string>{});
  REQUIRE(db.selectMultiple({"x"}) == std::vector<std::string>{});
  REQUIRE(db.selectBoolean() == false);
}

TEST_CASE (
    "Test Constraint Database intersect paired variable constraint x 2") {
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  db.addToPairedVariableConstraints("x", "y",
                                    {{"1", "2"}, {"4", "5"}, {"777", "777"}});
  REQUIRE(db.selectBoolean() == true);
  REQUIRE(db.selectOne("x") == std::vector<std::string>{"1"});
  REQUIRE(db.selectOne("y") == std::vector<std::string>{"2"});
  REQUIRE(db.selectMultiple({"x", "y"}) == std::vector<std::string>{"1 2"});
}

TEST_CASE (
    "Test Constraint Database intersect disjoint paired variable constraint x "
    "2") {
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"7", "0"}, {"88", "111"}});
  db.addToPairedVariableConstraints("x", "y",
                                    {{"1", "2"}, {"4", "5"}, {"777", "777"}});
  REQUIRE(db.selectBoolean() == false);
  REQUIRE(db.selectOne("x") == std::vector<std::string>{});
  REQUIRE(db.selectOne("y") == std::vector<std::string>{});
  REQUIRE(db.selectMultiple({"x", "y"}) == std::vector<std::string>{});
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "first var for both)") {
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  db.addToPairedVariableConstraints(
      "x", "z", {{"1", "100"}, {"2", "200"}, {"777", "777"}});
  REQUIRE(db.selectOne("x") == std::vector<std::string>{"1", "2"});
  REQUIRE(db.selectOne("y") == std::vector<std::string>{"2", "3"});
  REQUIRE(db.selectOne("z") == std::vector<std::string>{"100", "200"});
  REQUIRE(db.selectMultiple({"x", "y"}) ==
          std::vector<std::string>{"1 2", "2 3"});
  REQUIRE(db.selectMultiple({"x", "z"}) ==
          std::vector<std::string>{"1 100", "2 200"});
  REQUIRE(db.selectMultiple({"y", "z"}) ==
          std::vector<std::string>{"2 100", "3 200"});
  REQUIRE(db.selectMultiple({"x", "y", "z"}) ==
          std::vector<std::string>{"1 2 100", "2 3 200"});
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "second var for existing, first var for incoming)") {
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("y", "x", {{"2", "1"}, {"3", "2"}});
  db.addToPairedVariableConstraints(
      "x", "z", {{"1", "100"}, {"2", "200"}, {"777", "777"}});
  REQUIRE(db.selectOne("x") == std::vector<std::string>{"1", "2"});
  REQUIRE(db.selectOne("y") == std::vector<std::string>{"2", "3"});
  REQUIRE(db.selectOne("z") == std::vector<std::string>{"100", "200"});
  REQUIRE(db.selectMultiple({"x", "y"}) ==
          std::vector<std::string>{"1 2", "2 3"});
  REQUIRE(db.selectMultiple({"x", "z"}) ==
          std::vector<std::string>{"1 100", "2 200"});
  REQUIRE(db.selectMultiple({"y", "z"}) ==
          std::vector<std::string>{"2 100", "3 200"});
  REQUIRE(db.selectMultiple({"x", "y", "z"}) ==
          std::vector<std::string>{"1 2 100", "2 3 200"});
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "second var for both)") {
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("y", "x", {{"2", "1"}, {"3", "2"}});
  db.addToPairedVariableConstraints(
      "z", "x", {{"100", "1"}, {"200", "2"}, {"777", "777"}});
  REQUIRE(db.selectOne("x") == std::vector<std::string>{"1", "2"});
  REQUIRE(db.selectOne("y") == std::vector<std::string>{"2", "3"});
  REQUIRE(db.selectOne("z") == std::vector<std::string>{"100", "200"});
  REQUIRE(db.selectMultiple({"x", "y"}) ==
          std::vector<std::string>{"1 2", "2 3"});
  REQUIRE(db.selectMultiple({"x", "z"}) ==
          std::vector<std::string>{"1 100", "2 200"});
  REQUIRE(db.selectMultiple({"y", "z"}) ==
          std::vector<std::string>{"2 100", "3 200"});
  REQUIRE(db.selectMultiple({"x", "y", "z"}) ==
          std::vector<std::string>{"1 2 100", "2 3 200"});
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "first var for existing, second var for incoming)") {
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  db.addToPairedVariableConstraints(
      "z", "x", {{"100", "1"}, {"200", "2"}, {"777", "777"}});
  REQUIRE(db.selectOne("x") == std::vector<std::string>{"1", "2"});
  REQUIRE(db.selectOne("y") == std::vector<std::string>{"2", "3"});
  REQUIRE(db.selectOne("z") == std::vector<std::string>{"100", "200"});
  REQUIRE(db.selectMultiple({"x", "y"}) ==
          std::vector<std::string>{"1 2", "2 3"});
  REQUIRE(db.selectMultiple({"x", "z"}) ==
          std::vector<std::string>{"1 100", "2 200"});
  REQUIRE(db.selectMultiple({"y", "z"}) ==
          std::vector<std::string>{"2 100", "3 200"});
  REQUIRE(db.selectMultiple({"x", "y", "z"}) ==
          std::vector<std::string>{"1 2 100", "2 3 200"});
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "first var for both) - duplicate values on second paired set") {
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y", {{"1", "2"}, {"2", "3"}});
  db.addToPairedVariableConstraints(
      "x", "z", {{"1", "100"}, {"2", "700"}, {"2", "500"}, {"777", "777"}});
  // TODO: Add a multi-select here
  REQUIRE(db.selectOne("x") == std::vector<std::string>{"1", "2"});
  REQUIRE(db.selectOne("y") == std::vector<std::string>{"2", "3"});
  REQUIRE(db.selectOne("z") == std::vector<std::string>{"100", "700", "500"});
  REQUIRE(db.selectMultiple({"x", "y"}) ==
          std::vector<std::string>{"1 2", "2 3"});
  REQUIRE(db.selectMultiple({"x", "z"}) ==
          std::vector<std::string>{"1 100", "2 700", "2 500"});
  REQUIRE(db.selectMultiple({"y", "z"}) ==
          std::vector<std::string>{"2 100", "3 700", "3 500"});
  REQUIRE(db.selectMultiple({"x", "y", "z"}) ==
          std::vector<std::string>{"1 2 100", "2 3 700", "2 3 500"});
}

TEST_CASE (
    "Test Constraint Database join paired variable constraint x 2 (join on "
    "first var for both) - duplicate values on both paired sets") {
  auto db = ConstraintDatabase();
  db.addToPairedVariableConstraints("x", "y",
                                    {{"1", "2"}, {"1", "500"}, {"2", "3"}});
  db.addToPairedVariableConstraints(
      "x", "z", {{"1", "100"}, {"2", "700"}, {"2", "500"}, {"777", "777"}});
  REQUIRE(db.selectOne("x") == std::vector<std::string>{"1", "2"});
  REQUIRE(db.selectOne("y") == std::vector<std::string>{"2", "500", "3"});
  REQUIRE(db.selectOne("z") == std::vector<std::string>{"100", "700", "500"});
  REQUIRE(db.selectMultiple({"x", "y"}) ==
          std::vector<std::string>{"1 2", "1 500", "2 3"});
  REQUIRE(db.selectMultiple({"x", "z"}) ==
          std::vector<std::string>{"1 100", "2 700", "2 500"});
  REQUIRE(db.selectMultiple({"y", "z"}) ==
          std::vector<std::string>{"500 100", "2 100", "3 700", "3 500"});
  REQUIRE(
      db.selectMultiple({"x", "y", "z"}) ==
      std::vector<std::string>{"1 2 100", "2 3 700", "1 500 100", "2 3 500"});
}