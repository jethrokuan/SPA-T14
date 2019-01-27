#include "pql.h"

#include <algorithm>
#include <iostream>
#include <map>

using namespace QE;

Query::Query()
    : declarations(nullptr),
      selected_declaration(nullptr),
      such_that(nullptr),
      pattern(nullptr){};

Query::Query(std::vector<QE::Declaration> *decls,
             QE::Declaration *selected_decl)
    : declarations(decls),
      selected_declaration(selected_decl),
      such_that(nullptr),
      pattern(nullptr){};

Query::~Query() {
  delete declarations;
  delete selected_declaration;
  delete such_that;
  delete pattern;
}

std::unique_ptr<Query> Query::makeStubQuery() {
  auto decls = new std::vector<QE::Declaration>();
  auto selected_decl = new QE::Declaration(DesignEntity::ASSIGN, "p");
  return std::make_unique<Query>(decls, selected_decl);
}

bool Declaration::isValid() { return std::regex_match(synonym, synonym_regex); }

namespace QE {
// Hard to use unordered_map even though faster
// - need to define specialized hash for enum class
std::map<DesignEntity, std::string> designEntityToStringMap({
    {DesignEntity::STMT, "stmt"},
    {DesignEntity::READ, "read"},
    {DesignEntity::PRINT, "print"},
    {DesignEntity::CALL, "call"},
    {DesignEntity::WHILE, "while"},
    {DesignEntity::IF, "if"},
    {DesignEntity::ASSIGN, "assign"},
    {DesignEntity::VARIABLE, "variable"},
    {DesignEntity::CONSTANT, "constant"},
    {DesignEntity::PROCEDURE, "procedure"},
});

// Generic template for swapping keys and value of a map into a new map
// Should put in generic lib
template <class T1, class T2>
std::map<T2, T1> swapPairs(std::map<T1, T2> m) {
  std::map<T2, T1> m1;
  for (auto &&item : m) {
    m1.emplace(item.second, item.first);
  }
  return m1;
};

auto stringToDesignEntityMap =
    swapPairs<DesignEntity, std::string>(designEntityToStringMap);

DesignEntity getDesignEntity(std::string &designentity_string) {
  return stringToDesignEntityMap.at(designentity_string);
}
std::string getDesignEntityString(DesignEntity de) {
  return designEntityToStringMap.at(de);
}
}
