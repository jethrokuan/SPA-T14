#pragma once
#include <memory>
#include <regex>
#include <vector>

/*
 * Defines the main constructs for the Query Language PQL
 */

// Grammar for the select clause
// select-cl : declaration* ‘Select’ synonym [ suchthat-cl ] [ pattern-cl ]
// (zero or more declarations), followed by the word Select
namespace QE {

// WARNING: WHEN UPDATING THIS CLASS --> THE wordMap in the CPP FILE MUST BE
// UPDATED
enum class DesignEntity {
  STMT = 1,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE = 10
};
// wordMap is declared in the .cpp file
// Basic accessor for the DesignEntity word map and a reverse mapper
// Note: can throw exception..really wish i had std::optional
DesignEntity getDesignEntity(std::string&);
std::string getDesignEntityString(DesignEntity);

// WARNING: WHEN UPDATING THIS CLASS --> Update cpp file
enum class Relation {
  ModifiesS = 1,
  UsesS,
  Parent,
  ParentT,
  Follows,
  FollowsT = 6
};
Relation getRelation(std::string&);
std::string getRelationFromString(Relation);
const std::map<Relation, std::string>& getRelationToStringMap();

class Declaration {
 private:
  // Spec: LETTER (LETTER | DIGIT)*
  const std::regex synonym_regex = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");

 public:
  // -- Data --

  const DesignEntity design_entity;
  const std::string synonym;

  // -- Constructors --

  Declaration(DesignEntity de, std::string syn)
      : design_entity(de), synonym(syn){};

  // -- Methods --

  // Checks if synonym is valid (and therefore that Declaration is valid)
  bool isValid();

  // -- Operators --

  bool operator==(const Declaration& a2) const {
    return design_entity == a2.design_entity && synonym == a2.synonym;
  }

  friend std::ostream& operator<<(std::ostream& os, Declaration const& decl) {
    os << getDesignEntityString(decl.design_entity) << std::string(" ")
       << decl.synonym;
    return os;
  }
};
/*
IDENT: LETTER (LETTER | DIGIT)*
INTEGER: DIGIT+

synonym: IDENT
stmtRef: synonym | ‘_’ | INTEGER

entRef: synonym | ‘_’ | ‘"’ IDENT ‘"’
suchthat-cl: ‘such that’ relRef
relRef: ModifiesS | UsesS | Parent | ParentT | Follows | FollowsT

ModifiesS: ‘Modifies’ ‘(’ stmtRef ‘,’ entRef ‘)’
UsesS: ‘Uses’ ‘(’ stmtRef ‘,’ entRef ‘)’
Parent: ‘Parent’ ‘(’ stmtRef ‘,’ stmtRef ‘)’
ParentT: ‘Parent*’ ‘(’ stmtRef ‘,’ stmtRef ‘)’
Follows: ‘Follows’ ‘(’ stmtRef ‘,’ stmtRef ‘)’
FollowsT: ‘Follows*’ ‘(’ stmtRef ‘,’ stmtRef ‘)’
*/

// using stmtRef = std::variant<Synonym, Underscore, int

class SuchThat {
 public:
  Relation relation;
};

class Pattern {};

class Query {
 public:
  // -- Data --

  std::vector<Declaration>* declarations;
  // Selected declaration refers to the synonym that is after the 'Select'
  Declaration* selected_declaration;
  // No std::optional (is in C++17 - have to use nullable types)
  SuchThat* such_that;
  Pattern* pattern;

  // -- Accessors and constructors --

  Query(std::vector<Declaration>*, Declaration*);
  Query();
  ~Query();

  // -- Methods --

  // Make a basic (invalid) query with empty definition and empty select
  static std::unique_ptr<Query> makeStubQuery();
};
}  // namespace QE
