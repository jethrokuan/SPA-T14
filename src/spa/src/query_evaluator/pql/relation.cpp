#include <algorithm>
#include <iostream>
#include <map>

#include "query_evaluator/pql/relation.h"
#include "query_evaluator/pql/stmtentref.h"
#include "utils/utils.h"

using namespace Utils;

namespace QE {

std::map<Relation, std::string> relationToStringMap({
    {Relation::ModifiesS, "Modifies"},
    {Relation::UsesS, "Uses"},
    {Relation::Parent, "Parent"},
    {Relation::ParentT, "Parent*"},
    {Relation::Follows, "Follows"},
    {Relation::FollowsT, "Follows*"},
});

std::map<Relation, std::pair<RefType, RefType>> relationToArgTypesMap({
    {Relation::ModifiesS, std::make_pair(RefType::STMTREF, RefType::ENTREF)},
    {Relation::UsesS, std::make_pair(RefType::STMTREF, RefType::ENTREF)},
    {Relation::Parent, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
    {Relation::ParentT, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
    {Relation::Follows, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
    {Relation::FollowsT, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
});

auto stringToRelationMap =
    swapPairs<Relation, std::string>(relationToStringMap);

Relation getRelation(std::string& relation_string) {
  return stringToRelationMap.at(relation_string);
}
std::string getStringFromRelation(Relation relation) {
  return relationToStringMap.at(relation);
}
const std::map<Relation, std::string>& getRelationToStringMap() {
  return relationToStringMap;
}

std::pair<RefType, RefType> getArgTypesFromRelation(Relation& r) {
  return relationToArgTypesMap.at(r);
}

}  // namespace QE