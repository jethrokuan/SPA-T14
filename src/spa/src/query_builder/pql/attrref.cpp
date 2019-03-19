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

AttrName QE::attrNameFromString(std::string s) {
  AttrName name;
  if (s.compare("procName") == 0) {
    name = AttrName::PROC_NAME;
  } else if (s.compare("varName") == 0) {
    name = AttrName::VAR_NAME;
  } else if (s.compare("value") == 0) {
    name = AttrName::VALUE;
  } else if (s.compare("stmt#") == 0) {
    name = AttrName::STMT_NO;
  } else {
    throw PQLParseException("Expected a valid attrName, got " + s);
  }
  return name;
}

AttrRef::AttrRef(std::variant<QuoteIdent, unsigned int, Synonym, SynAttr> attr_,
                 std::vector<Declaration>* decls)
    : attr(attr_), attrType(AttrType::NAME) {
  AttrType attr_type_ = AttrType::NAME;
  std::visit(
      Utils::overload{
          [&attr_type_](QuoteIdent) { attr_type_ = AttrType::NAME; },
          [&attr_type_](unsigned int) { attr_type_ = AttrType::INTEGER; },
          [&attr_type_, decls](Synonym syn) {  // Has to be of prog_line type
            auto decl = Declaration::findDeclarationForSynonym(decls, syn);

            if (!decl) {
              throw PQLParseException("Undeclared synonym " + syn.synonym);
            }

            if (decl->getDesignEntity() != DesignEntity::PROG_LINE) {
              throw PQLParseException(
                  "Synonym in attr ref needs to be of type prog_line.");
            }

            attr_type_ = AttrType::INTEGER;
          },
          [&attr_type_](SynAttr synAttr) {
            attr_type_ = getAttrType(synAttr.attrName);
          }},
      attr);
  attrType = attr_type_;
};

bool AttrRef::operator==(const AttrRef& other) const {
  return attr == other.attr && attrType == other.attrType;
}

SynAttr::SynAttr(Synonym synonym, AttrName attrName,
                 std::vector<Declaration>* decls)
    : synonym(synonym), attrName(attrName) {
  auto decl = Declaration::findDeclarationForSynonym(decls, synonym);
  if (!decl) {
    throw PQLParseException("No such synonym declared.");
  }

  AttrSet valid_attr_names = getAttrSet(decl->getDesignEntity());

  if (valid_attr_names.find(attrName) == valid_attr_names.end()) {
    throw PQLParseException("Invalid attribute name.");
  }
}

bool SynAttr::operator==(const SynAttr& other) const {
  return synonym == other.synonym && attrName == other.attrName;
}
