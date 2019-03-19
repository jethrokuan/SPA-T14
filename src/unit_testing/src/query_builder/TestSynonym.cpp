#include "catch.hpp"

#include "query_builder/core/exceptions.h"
#include "query_builder/pql/pql.h"

using namespace QE;

TEST_CASE ("Empty Synonym") {
  REQUIRE_THROWS_AS(Synonym(""), PQLParseException);
}

TEST_CASE ("Single letter Synonym") { REQUIRE_NOTHROW(Synonym("p")); }

TEST_CASE ("Single number Synonym") {
  REQUIRE_THROWS_AS(Synonym("1"), PQLParseException);
}

TEST_CASE ("Single space Synonym") {
  REQUIRE_THROWS_AS(Synonym(" "), PQLParseException);
}

TEST_CASE ("Single underscore Synonym") {
  REQUIRE_THROWS_AS(Synonym("_"), PQLParseException);
}

TEST_CASE ("Multi-letter Synonym") { REQUIRE_NOTHROW(Synonym("pn")); }

TEST_CASE ("Multi-letter with trailing number Synonym") {
  REQUIRE_NOTHROW(Synonym("pn1"));
}

TEST_CASE ("Interleaving letter and number Synonym") {
  REQUIRE_NOTHROW(Synonym("pn1as"));
}

TEST_CASE ("Single special character Synonym") {
  REQUIRE_THROWS_AS(Synonym("@"), PQLParseException);
}

TEST_CASE ("Single letter Synonym with quotes") {
  REQUIRE_THROWS_AS(Synonym("\"p\""), PQLParseException);
}

TEST_CASE ("Single number Synonym with quotes") {
  REQUIRE_THROWS_AS(Synonym("\"1\""), PQLParseException);
}

TEST_CASE ("Single space Synonym with quotes") {
  REQUIRE_THROWS_AS(Synonym("\" \""), PQLParseException);
}

TEST_CASE ("Single underscore Synonym with quotes") {
  REQUIRE_THROWS_AS(Synonym("\"_\""), PQLParseException);
}

TEST_CASE ("Multi-letter Synonym with quotes") {
  REQUIRE_THROWS_AS(Synonym("\"pn\""), PQLParseException);
}

TEST_CASE ("Multi-letter with trailing number Synonym with quotes") {
  REQUIRE_THROWS_AS(Synonym("\"pn1\""), PQLParseException);
}

TEST_CASE ("Interleaving letter and number Synonym with quotes") {
  REQUIRE_THROWS_AS(Synonym("\"p1nas\""), PQLParseException);
}

TEST_CASE ("Single special character Synonym with quotes") {
  REQUIRE_THROWS_AS(Synonym("\"@\""), PQLParseException);
}
