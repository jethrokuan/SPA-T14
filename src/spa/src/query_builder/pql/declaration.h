#pragma once
#include <iostream>
#include <regex>
#include <string>

#include "query_builder/pql/design_entity.h"
#include "query_builder/pql/stmtentref.h"
#include "query_builder/pql/synonym.h"

namespace QE {
//! Represents a single declaration in a PQL query (e.g. 'assign a')
class Declaration {
 private:
  // Spec: LETTER (LETTER | DIGIT)*
  const std::regex synonym_regex = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");
  // -- Data --
  const DesignEntity design_entity;
  const Synonym synonymObj;

 public:
  DesignEntity getDesignEntity() { return design_entity; }
  Synonym getSynonym() { return synonymObj; }

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

  static std::optional<Declaration> findDeclarationForSynonym(
      std::vector<Declaration>* decls, Synonym& synonym) {
    auto found_declaration =
        std::find_if(decls->begin(), decls->end(), [&](auto decl) {
          return decl.getSynonym().synonym == synonym.synonym;
        });
    if (found_declaration == decls->end()) {
      return std::nullopt;
    } else {
      return std::make_optional<Declaration>(*found_declaration);
    }
  }
};
}  // namespace QE
