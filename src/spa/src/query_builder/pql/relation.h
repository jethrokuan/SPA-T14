#pragma once

#include <regex>
#include <string>
#include <unordered_map>

#include "query_builder/pql/design_entity.h"
#include "query_builder/pql/ref.h"
#include "query_builder/pql/relation.h"

namespace QE {
// WARNING: WHEN UPDATING THIS CLASS --> Update cpp file
enum class Relation {
  Modifies,
  ModifiesS,
  ModifiesP,
  Uses,
  UsesP,
  UsesS,
  Calls,
  CallsT,
  Parent,
  ParentT,
  Follows,
  FollowsT,
  Next,
  NextT,
  Affects,
  AffectsT,
};
Relation getRelation(std::string&);
std::string getStringFromRelation(Relation);
const std::unordered_map<Relation, std::string>& getRelationToStringMap();
std::pair<RefTypeIndexSet, RefTypeIndexSet> getArgRefTypesFromRelation(
    const Relation&);
std::pair<std::vector<DesignEntity>, std::vector<DesignEntity>>
getArgSynonymTypesFromRelation(const Relation&);
}  // namespace QE
