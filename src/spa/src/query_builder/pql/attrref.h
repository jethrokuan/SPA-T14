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

AttrSet getAttrSet(DesignEntity de);
AttrType getAttrType(AttrName attrName);

class SynAttr {
 private:
  SynAttr(Synonym synonym_, AttrName attrName_)
      : synonym(synonym_), attrName(attrName_){};

 public:
  Synonym synonym;
  AttrName attrName;
  static std::optional<SynAttr> construct(Synonym synonym, AttrName name,
                                          std::vector<Declaration>* decls);
};

class AttrRef {
 private:
  AttrRef(std::variant<QuoteIdent, unsigned int, Synonym, SynAttr> attr_,
          AttrType attrType_)
      : attr(attr_), attrType(attrType_){};

 public:
  std::variant<QuoteIdent, unsigned int, Synonym, SynAttr> attr;
  AttrType attrType;

  static std::optional<AttrRef> construct(
      std::variant<QuoteIdent, unsigned int, Synonym, SynAttr> attr,
      std::vector<Declaration>* decls);
};
}  // namespace QE
