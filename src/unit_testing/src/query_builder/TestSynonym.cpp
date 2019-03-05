#include "catch.hpp"

#include "query_builder/pql/pql.h"

TEST_CASE ("Empty Synonym") {
  auto d = QE::Synonym::construct("");
  REQUIRE(!d);
}

TEST_CASE ("Single letter Synonym") {
  auto d = QE::Synonym::construct("p");
  REQUIRE(d);
}

TEST_CASE ("Single number Synonym") {
  auto d = QE::Synonym::construct("1");
  REQUIRE(!d);
}

TEST_CASE ("Single space Synonym") {
  auto d = QE::Synonym::construct(" ");
  REQUIRE(!d);
}

TEST_CASE ("Single underscore Synonym") {
  auto d = QE::Synonym::construct("_");
  REQUIRE(!d);
}

TEST_CASE ("Multi-letter Synonym") {
  auto d = QE::Synonym::construct("pn");
  REQUIRE(d);
}

TEST_CASE ("Multi-letter with trailing number Synonym") {
  auto d = QE::Synonym::construct("pn1");
  REQUIRE(d);
}

TEST_CASE ("Interleaving letter and number Synonym") {
  auto d = QE::Synonym::construct("pn1as");
  REQUIRE(d);
}

TEST_CASE ("Single special character Synonym") {
  auto d = QE::Synonym::construct("@");
  REQUIRE(!d);
}

TEST_CASE ("Empty Synonym with quotes") {
  auto d = QE::Synonym::construct("");
  REQUIRE(!d);
}

TEST_CASE ("Single letter Synonym with quotes") {
  auto d = QE::Synonym::construct("\"p\"");
  REQUIRE(!d);
}

TEST_CASE ("Single number Synonym with quotes") {
  auto d = QE::Synonym::construct("\"1\"");
  REQUIRE(!d);
}

TEST_CASE ("Single space Synonym with quotes") {
  auto d = QE::Synonym::construct("\" \"");
  REQUIRE(!d);
}

TEST_CASE ("Single underscore Synonym with quotes") {
  auto d = QE::Synonym::construct("\"_\"");
  REQUIRE(!d);
}

TEST_CASE ("Multi-letter Synonym with quotes") {
  auto d = QE::Synonym::construct("\"pn\"");
  REQUIRE(!d);
}

TEST_CASE ("Multi-letter with trailing number Synonym with quotes") {
  auto d = QE::Synonym::construct("\"pn1\"");
  REQUIRE(!d);
}

TEST_CASE ("Interleaving letter and number Synonym with quotes") {
  auto d = QE::Synonym::construct("\"pn1as\"");
  REQUIRE(!d);
}

TEST_CASE ("Single special character Synonym with quotes") {
  auto d = QE::Synonym::construct("\"@\"");
  REQUIRE(!d);
}