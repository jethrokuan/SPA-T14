#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "query_builder/pql/declaration.h"
#include "query_builder/pql/design_entity.h"
#include "query_builder/pql/quoteident.h"
#include "query_builder/pql/synonym.h"

using QE::Declaration;
using QE::DesignEntity;
using QE::Synonym;

namespace QE {

enum class AttrType {
  NAME,
  INTEGER,
};

enum class AttrName {
  PROC_NAME,
  VAR_NAME,
  VALUE,
  STMT_NO,
};

using AttrSet = std::unordered_set<AttrName>;

AttrName attrNameFromString(std::string s);
AttrSet getAttrSet(DesignEntity de);
AttrType getAttrType(AttrName attrName);

class SynAttr {
 public:
  Synonym synonym;
  AttrName attrName;

  SynAttr(Synonym synonym_, AttrName attrName_)
      : synonym(synonym_), attrName(attrName_){};

  static std::optional<SynAttr> construct(Synonym synonym, AttrName name,
                                          std::vector<Declaration>* decls);

  bool operator==(const SynAttr& other) const;
};

class AttrRef {
 public:
  AttrRef(std::variant<QuoteIdent, unsigned int, Synonym, SynAttr> attr_,
          AttrType attrType_)
      : attr(attr_), attrType(attrType_){};
  std::variant<QuoteIdent, unsigned int, Synonym, SynAttr> attr;
  AttrType attrType;

  static std::optional<AttrRef> construct(
      std::variant<QuoteIdent, unsigned int, Synonym, SynAttr> attr,
      std::vector<Declaration>* decls);

  bool operator==(const AttrRef& other) const;
};
}  // namespace QE
