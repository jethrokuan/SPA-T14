#pragma once
#include <iostream>
#include <regex>
#include <string>

#include "query_evaluator/pql/design_entity.h"
#include "query_evaluator/pql/stmtentref.h"
#include "query_evaluator/pql/synonym.h"

namespace QE {
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
}  // namespace QE
