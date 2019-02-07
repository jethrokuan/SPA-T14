#include "catch.hpp"

#include "query_evaluator/pql/pql.h"

TEST_CASE ("Empty QuoteIndent without Quotes") {
  auto d = QE::QuoteIdent::construct("");
  REQUIRE(!d);
}

TEST_CASE ("Single letter QuoteIndent without Quotes") {
  auto d = QE::QuoteIdent::construct("p");
  REQUIRE(!d);
}

TEST_CASE ("Single number QuoteIndent without Quotes") {
  auto d = QE::QuoteIdent::construct("1");
  REQUIRE(!d);
}

TEST_CASE ("Single space QuoteIndent without Quotes") {
  auto d = QE::QuoteIdent::construct(" ");
  REQUIRE(!d);
}

TEST_CASE ("Single underscore QuoteIndent without Quotes") {
  auto d = QE::QuoteIdent::construct("_");
  REQUIRE(!d);
}

TEST_CASE ("Multi-letter QuoteIndent without Quotes") {
  auto d = QE::QuoteIdent::construct("pn");
  REQUIRE(!d);
}

TEST_CASE ("Multi-letter with trailing number QuoteIndent without Quotes") {
  auto d = QE::QuoteIdent::construct("pn1");
  REQUIRE(!d);
}

TEST_CASE ("Interleaving letter and number QuoteIndent without Quotes") {
  auto d = QE::QuoteIdent::construct("pn1as");
  REQUIRE(!d);
}

TEST_CASE ("Single special character QuoteIndent without Quotes") {
  auto d = QE::QuoteIdent::construct("@");
  REQUIRE(!d);
}

// SECTION: with quotes (TODO: use catch2 section)

TEST_CASE ("Empty QuoteIdent") {
  auto d = QE::QuoteIdent::construct("");
  REQUIRE(!d);
}

TEST_CASE ("Single letter QuoteIdent") {
  auto d = QE::QuoteIdent::construct("\"p\"");
  REQUIRE(d);
}

TEST_CASE ("Single number QuoteIdent") {
  auto d = QE::QuoteIdent::construct("\"1\"");
  REQUIRE(!d);
}

TEST_CASE ("Single space QuoteIdent") {
  auto d = QE::QuoteIdent::construct("\" \"");
  REQUIRE(!d);
}

TEST_CASE ("Single underscore QuoteIdent") {
  auto d = QE::QuoteIdent::construct("\"_\"");
  REQUIRE(!d);
}

TEST_CASE ("Multi-letter QuoteIdent") {
  auto d = QE::QuoteIdent::construct("\"pn\"");
  REQUIRE(d);
}

TEST_CASE ("Multi-letter with trailing number QuoteIdent") {
  auto d = QE::QuoteIdent::construct("\"pn1\"");
  REQUIRE(d);
}

TEST_CASE ("Interleaving letter and number QuoteIdent") {
  auto d = QE::QuoteIdent::construct("\"pn1as\"");
  REQUIRE(d);
}

TEST_CASE ("Single special character QuoteIdent") {
  auto d = QE::QuoteIdent::construct("\"@\"");
  REQUIRE(!d);
}