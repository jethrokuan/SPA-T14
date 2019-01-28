#include "catch.hpp"

#include "query_evaluator/pql.h"

TEST_CASE("Empty Declaration") {
  auto d = QE::Declaration(QE::DesignEntity::ASSIGN, "");
  REQUIRE(!d.isValid());
}

TEST_CASE("Single letter declaration") {
  auto d = QE::Declaration(QE::DesignEntity::ASSIGN, "p");
  REQUIRE(d.isValid());
}

TEST_CASE("Single number declaration") {
  auto d = QE::Declaration(QE::DesignEntity::ASSIGN, "1");
  REQUIRE(!d.isValid());
}

TEST_CASE("Single space declaration") {
  auto d = QE::Declaration(QE::DesignEntity::ASSIGN, " ");
  REQUIRE(!d.isValid());
}

TEST_CASE("Single underscore declaration") {
  auto d = QE::Declaration(QE::DesignEntity::ASSIGN, "_");
  REQUIRE(!d.isValid());
}

TEST_CASE("Multi-letter declaration") {
  auto d = QE::Declaration(QE::DesignEntity::ASSIGN, "pn");
  REQUIRE(d.isValid());
}

TEST_CASE("Multi-letter with trailing number declaration") {
  auto d = QE::Declaration(QE::DesignEntity::ASSIGN, "pn1");
  REQUIRE(d.isValid());
}

TEST_CASE("Interleaving letter and number declaration") {
  auto d = QE::Declaration(QE::DesignEntity::ASSIGN, "pn1as");
  REQUIRE(d.isValid());
}

TEST_CASE("Single special character declaration") {
  auto d = QE::Declaration(QE::DesignEntity::ASSIGN, "@");
  REQUIRE(!d.isValid());
}