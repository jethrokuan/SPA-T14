#include "catch.hpp"

#include <iostream>
#include <vector>

#include "query_builder/core/exceptions.h"
#include "query_builder/core/query_validator.h"
#include "query_builder/pql/pql.h"
#include "query_builder/query_builder.h"

using namespace QE;
TEST_CASE ("Test pattern assignment synonym not-assign Validate") {}

TEST_CASE ("Test pattern assignment synonym assign no matching decl Validate") {
  std::string query_string = "assign a1; Select a1 pattern a(_, _)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with Modifies firstArg underscore Validate") {
  std::string query_string = "assign a; Select a such that Modifies(_, \"a\")";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with Uses firstArg underscore Validate") {
  std::string query_string = "assign a; Select a such that Uses(_, \"a\")";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with Follows firstArg underscore Validate (ok)") {
  std::string query_string = "assign a; Select a such that Follows(_, 2)";
  auto qe = QueryBuilder();
  REQUIRE_NOTHROW(qe.makePqlQuery(query_string));
}

TEST_CASE ("Test such that with Follows secondArg quoteIdent") {
  std::string query_string = "assign a; Select a such that Follows(_, \"a2\")";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with Parent firstArg quoteIdent") {
  std::string query_string = "assign a; Select a such that Parent(\"a2\", _)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test pattern with firstArg stmtRef") {
  std::string query_string = "assign a; Select a pattern a(2, \"x\")";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with not-found synonym firstarg Validate") {
  std::string query_string = "assign a; Select a such that Follows(a1, a)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with invalid type firstarg Validate") {
  std::string query_string =
      "variable v, v1; Select v such that Modifies(v, v1)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test such that with invalid type secondarg Validate") {
  std::string query_string = "assign a, a1; Select a such that Modifies(a, a1)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test multiple declarations with the same name") {
  std::string query_string =
      "assign a, a; variable v; Select a such that Modifies(a, v)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE (
    "Test multiple declarations with the same name with different types") {
  std::string query_string =
      "assign a; stmt a; variable v; Select a such that Modifies(a, v)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test pattern assignment first argument variable Validate") {
  std::string query_string = "assign a; variable v; Select a pattern a(v, _)";
  auto qe = QueryBuilder();
  REQUIRE_NOTHROW(qe.makePqlQuery(query_string));
}

TEST_CASE ("Test pattern assignment first argument not-var Validate") {
  std::string query_string =
      "assign a, a1; variable v; Select a pattern a(a1, _)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test pattern assignment first argument not-present Validate") {
  std::string query_string =
      "assign a, a1; variable v; Select a pattern a(a2, _)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test selected synonym declared Validate") {
  std::string query_string =
      "assign a, a1; variable v; Select v1 pattern a(v, _)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}

TEST_CASE ("Test selected synonym (tupled) declared Validate") {
  std::string query_string =
      "assign a, a1; variable v; Select <v, v2> pattern a(v, _)";
  auto qe = QueryBuilder();
  REQUIRE_THROWS_AS(qe.makePqlQuery(query_string), PQLValidationException);
}
