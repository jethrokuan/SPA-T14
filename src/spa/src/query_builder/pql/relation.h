#pragma once
#include <map>
#include <regex>
#include <string>

#include "query_builder/pql/design_entity.h"
#include "query_builder/pql/relation.h"

namespace QE {
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
std::string getStringFromRelation(Relation);
const std::map<Relation, std::string>& getRelationToStringMap();
std::pair<std::vector<DesignEntity>, std::vector<DesignEntity>>
getArgSynonymTypesFromRelation(Relation&);
}  // namespace QE