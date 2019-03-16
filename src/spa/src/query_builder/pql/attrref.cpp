#include "query_builder/core/exceptions.h"

#include "query_builder/pql/attrref.h"
#include "utils/utils.h"

#include <optional>
#include <vector>

using namespace QE;

std::unordered_map<DesignEntity, AttrSet> validAttrMap({
    {DesignEntity::PROCEDURE, AttrSet{AttrName::PROC_NAME}},
    {DesignEntity::CALL, AttrSet{AttrName::PROC_NAME, AttrName::STMT_NO}},
    {DesignEntity::VARIABLE, AttrSet{AttrName::VAR_NAME}},
    {DesignEntity::IF, AttrSet{AttrName::STMT_NO}},
    {DesignEntity::WHILE, AttrSet{AttrName::STMT_NO}},
    {DesignEntity::READ, AttrSet{AttrName::VAR_NAME, AttrName::STMT_NO}},
    {DesignEntity::PRINT, AttrSet{AttrName::VAR_NAME, AttrName::STMT_NO}},
    {DesignEntity::CONSTANT, AttrSet{AttrName::VALUE}},
    {DesignEntity::ASSIGN, AttrSet{AttrName::STMT_NO}},
    {DesignEntity::STMT, AttrSet{AttrName::STMT_NO}},
    {DesignEntity::PROG_LINE, AttrSet{}},
});

std::unordered_map<AttrName, AttrType> attrNameToTypeMap(
    {{AttrName::PROC_NAME, AttrType::NAME},
     {AttrName::VAR_NAME, AttrType::NAME},
     {AttrName::VALUE, AttrType::INTEGER},
     {AttrName::STMT_NO, AttrType::INTEGER}});

AttrSet QE::getAttrSet(DesignEntity de) { return validAttrMap[de]; }
AttrType QE::getAttrType(AttrName attrName) {
  return attrNameToTypeMap[attrName];
};

std::optional<AttrRef> AttrRef::construct(
    std::variant<QuoteIdent, unsigned int, Synonym, SynAttr> attr,
    std::vector<Declaration>* decls) {
  return std::visit(
      Utils::overload{
          [](QuoteIdent qi) { return AttrRef(qi, AttrType::NAME); },
          [](unsigned int i) { return AttrRef(i, AttrType::INTEGER); },
          [decls](Synonym syn) {  // Has to be of prog_line type
            auto decl = Declaration::findDeclarationForSynonym(decls, syn);

            if (decl->getDesignEntity() != DesignEntity::PROG_LINE) {
              throw PQLParseException(
                  "Synonym in attr ref needs to be of type prog_line.");
            }
            return AttrRef(syn, AttrType::INTEGER);
          },
          [](SynAttr synAttr) {
            AttrType attrType_ = getAttrType(synAttr.attrName);
            return AttrRef(synAttr, attrType_);
          }},
      attr);
};

std::optional<SynAttr> SynAttr::construct(Synonym synonym, AttrName attrName,
                                          std::vector<Declaration>* decls) {
  auto decl = Declaration::findDeclarationForSynonym(decls, synonym);
  if (!decl) {
    throw PQLParseException("No such synonym declared.");
  }
  AttrSet valid_attr_names = getAttrSet(decl->getDesignEntity());
  if (valid_attr_names.find(attrName) == valid_attr_names.end()) {
    return std::nullopt;
  }

  return SynAttr(synonym, attrName);
}
