#include "catch.hpp"

#include "query_builder/pql/pql.h"

TEST_CASE ("Follow SuchThat stmtx2 construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::StmtRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::Follows, a1, a2);
  REQUIRE(d);
}

TEST_CASE ("Follow SuchThat stmt entref construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::EntRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::Follows, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("Follow SuchThat entref stmtref construction") {
  QE::StmtRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::Follows, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("Follow SuchThat entref entref construction") {
  QE::EntRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::Follows, a1, a2);
  REQUIRE(!d);
}

///

TEST_CASE ("FollowsT SuchThat stmtx2 construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::StmtRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::FollowsT, a1, a2);
  REQUIRE(d);
}

TEST_CASE ("FollowsT SuchThat stmt entref construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::EntRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::FollowsT, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("FollowsT SuchThat entref stmtref construction") {
  QE::StmtRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::FollowsT, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("FollowsT SuchThat entref entref construction") {
  QE::EntRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::FollowsT, a1, a2);
  REQUIRE(!d);
}

//

TEST_CASE ("Parent SuchThat stmtx2 construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::StmtRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::Parent, a1, a2);
  REQUIRE(d);
}

TEST_CASE ("Parent SuchThat stmt entref construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::EntRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::Parent, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("Parent SuchThat entref stmtref construction") {
  QE::StmtRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::Parent, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("Parent SuchThat entref entref construction") {
  QE::EntRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::Parent, a1, a2);
  REQUIRE(!d);
}

//

TEST_CASE ("ParentT SuchThat stmtx2 construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::StmtRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::ParentT, a1, a2);
  REQUIRE(d);
}

TEST_CASE ("ParentT SuchThat stmt entref construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::EntRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::ParentT, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("ParentT SuchThat entref stmtref construction") {
  QE::StmtRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::ParentT, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("ParentT SuchThat entref entref construction") {
  QE::EntRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::ParentT, a1, a2);
  REQUIRE(!d);
}

//

TEST_CASE ("UsesS SuchThat stmtx2 construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::StmtRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::UsesS, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("UsesS SuchThat stmt entref construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::EntRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::UsesS, a1, a2);
  REQUIRE(d);
}

TEST_CASE ("UsesS SuchThat entref stmtref construction") {
  QE::StmtRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::UsesS, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("UsesS SuchThat entref entref construction") {
  QE::EntRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::UsesS, a1, a2);
  REQUIRE(!d);
}

//

TEST_CASE ("ModifiesS: SuchThat stmtx2 construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::StmtRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::ModifiesS, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("ModifiesS SuchThat stmt entref construction") {
  QE::StmtRef s1 = QE::Underscore();
  QE::EntRef s2 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::ModifiesS, a1, a2);
  REQUIRE(d);
}

TEST_CASE ("ModifiesS SuchThat entref stmtref construction") {
  QE::StmtRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::ModifiesS, a1, a2);
  REQUIRE(!d);
}

TEST_CASE ("ModifiesS SuchThat entref entref construction") {
  QE::EntRef s2 = QE::Underscore();
  QE::EntRef s1 = QE::Underscore();
  QE::StmtOrEntRef a1 = s1;
  QE::StmtOrEntRef a2 = s2;
  auto d = QE::SuchThat::construct(QE::Relation::ModifiesS, a1, a2);
  REQUIRE(!d);
}
