#pragma once
#include <memory>
#include <optional>
#include <regex>
#include <variant>
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

class Synonym {
 private:
  // Spec: LETTER (LETTER | DIGIT)*
  static const std::regex synonym_regex;

  // Make constructors private
  Synonym() {}
  Synonym(std::string& synonym) : synonym(synonym) {}

 public:
  std::string synonym;
  // Constructs a guaranteed valid Synonym
  static std::optional<Synonym> construct(std::string&);
  static std::optional<Synonym> construct(const char*);

  bool operator==(const Synonym& a2) const { return synonym == a2.synonym; }
  friend std::ostream& operator<<(std::ostream& os, Synonym const& syn) {
    os << syn.synonym;
    return os;
  }
};

class QuoteIdent {
 private:
  // Spec: LETTER (LETTER | DIGIT)*
  static const std::regex quoteident_regex;

  // Make constructors private
  QuoteIdent() {}
  QuoteIdent(std::string& quote_ident) : quote_ident(quote_ident) {}

 public:
  std::string quote_ident;
  // Constructs a guaranteed valid Synonym
  static std::optional<QuoteIdent> construct(std::string&);
  static std::optional<QuoteIdent> construct(const char*);

  bool operator==(const QuoteIdent& a2) const {
    return quote_ident == a2.quote_ident;
  }
  friend std::ostream& operator<<(std::ostream& os, QuoteIdent const& qi) {
    os << qi.quote_ident;
    return os;
  }
};

class Declaration {
 private:
  // Spec: LETTER (LETTER | DIGIT)*
  const std::regex synonym_regex = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");
  // -- Data --
  const DesignEntity design_entity;
  const Synonym synonymObj;

 public:
  DesignEntity getDesignEntity() { return design_entity; }
  std::string getSynonym() { return synonymObj.synonym; }

  // -- Constructors --

  Declaration(DesignEntity de, Synonym syn)
      : design_entity(de), synonymObj(syn){};

  // -- Operators --

  bool operator==(const Declaration& a2) const {
    return design_entity == a2.design_entity && synonymObj == a2.synonymObj;
  }

  friend std::ostream& operator<<(std::ostream& os, Declaration const& decl) {
    os << getDesignEntityString(decl.design_entity) << std::string(" ")
       << decl.synonymObj;
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

class Underscore {};

using StmtRef = std::variant<Synonym, Underscore, unsigned int>;
using EntRef = std::variant<Synonym, Underscore, QuoteIdent>;

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
};
}  // namespace QE
