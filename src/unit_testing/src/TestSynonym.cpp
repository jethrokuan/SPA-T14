#include "catch.hpp"

#include "query_evaluator/pql.h"

TEST_CASE("Empty Declaration") {
  auto d = QE::Synonym::construct("");
  REQUIRE(!d);
}

TEST_CASE("Single letter declaration") {
  auto d = QE::Synonym::construct("p");
  REQUIRE(d);
}

TEST_CASE("Single number declaration") {
  auto d = QE::Synonym::construct("1");
  REQUIRE(!d);
}

TEST_CASE("Single space declaration") {
  auto d = QE::Synonym::construct(" ");
  REQUIRE(!d);
}

TEST_CASE("Single underscore declaration") {
  auto d = QE::Synonym::construct("_");
  REQUIRE(!d);
}

TEST_CASE("Multi-letter declaration") {
  auto d = QE::Synonym::construct("pn");
  REQUIRE(d);
}

TEST_CASE("Multi-letter with trailing number declaration") {
  auto d = QE::Synonym::construct("pn1");
  REQUIRE(d);
}

TEST_CASE("Interleaving letter and number declaration") {
  auto d = QE::Synonym::construct("pn1as");
  REQUIRE(d);
}

TEST_CASE("Single special character declaration") {
  auto d = QE::Synonym::construct("@");
  REQUIRE(!d);
}