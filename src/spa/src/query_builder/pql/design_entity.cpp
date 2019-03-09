#include "query_builder/pql/design_entity.h"
#include "query_builder/core/exceptions.h"
#include "utils/utils.h"

#include <stdexcept>

#include <unordered_map>

using namespace Utils;

namespace QE {
// Hard to use unordered_map even though faster
// - need to define specialized hash for enum class
std::unordered_map<DesignEntity, std::string> designEntityToStringMap({
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

auto stringToDesignEntityMap =
    swapPairs<DesignEntity, std::string>(designEntityToStringMap);

DesignEntity getDesignEntity(std::string& designentity_string) {
  try {
    return stringToDesignEntityMap.at(designentity_string);
  } catch (const std::out_of_range& oor) {
    throw PQLParseException("Cannot find a Design Entity called " +
                            designentity_string);
  }
}

// An exception here would be a programmer error - no need to handle differently
std::string getDesignEntityString(DesignEntity de) {
  return designEntityToStringMap.at(de);
}

std::string getDesignEntityVectorString(std::vector<DesignEntity> des) {
  std::string res = "|";
  for (auto de : des) {
    res += getDesignEntityString(de);
    res += "|";
  }
  return res;
}

}  // namespace QE
