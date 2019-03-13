#include <algorithm>
#include <iostream>

#include <vector>

#include "query_builder/pql/relation.h"
#include "query_builder/pql/stmtentref.h"
#include "utils/utils.h"

using namespace Utils;

namespace QE {

std::unordered_map<Relation, std::string> relationToStringMap({
    {Relation::ModifiesS, "Modifies"},
    {Relation::UsesS, "Uses"},
    {Relation::Parent, "Parent"},
    {Relation::ParentT, "Parent*"},
    {Relation::Follows, "Follows"},
    {Relation::FollowsT, "Follows*"},
});

std::unordered_map<Relation, std::pair<RefType, RefType>> relationToArgTypesMap(
    {
        {Relation::ModifiesS,
         std::make_pair(RefType::STMTREF, RefType::ENTREF)},
        {Relation::UsesS, std::make_pair(RefType::STMTREF, RefType::ENTREF)},
        {Relation::Parent, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
        {Relation::ParentT, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
        {Relation::Follows, std::make_pair(RefType::STMTREF, RefType::STMTREF)},
        {Relation::FollowsT,
         std::make_pair(RefType::STMTREF, RefType::STMTREF)},
    });

//! Maps each relation to design entities for each of its allowed arguments if
//! they are synonyms
std::unordered_map<
    Relation, std::pair<std::vector<DesignEntity>, std::vector<DesignEntity>>>
    relationToArgSynonymTypesMap({
        {Relation::ModifiesS,
         // No PRINT design entity for Uses: print uses a variable, not
         // modifies
         // TODO: Add CALL in later iterations
         std::make_pair(ALL_STMT_DESIGN_ENTITIES_NO_PRINT,
                        std::vector<DesignEntity>{DesignEntity::VARIABLE})},
        // No READ design entity for Uses: read modifies a variable - not uses
        {Relation::UsesS,
         std::make_pair(ALL_STMT_DESIGN_ENTITIES_NO_READ,
                        std::vector<DesignEntity>{DesignEntity::VARIABLE})},
        {Relation::Parent,
         std::make_pair(ALL_STMT_DESIGN_ENTITIES, ALL_STMT_DESIGN_ENTITIES)},
        {Relation::ParentT,
         std::make_pair(ALL_STMT_DESIGN_ENTITIES, ALL_STMT_DESIGN_ENTITIES)},
        {Relation::Follows,
         std::make_pair(ALL_STMT_DESIGN_ENTITIES, ALL_STMT_DESIGN_ENTITIES)},
        {Relation::FollowsT,
         std::make_pair(ALL_STMT_DESIGN_ENTITIES, ALL_STMT_DESIGN_ENTITIES)},
    });

auto stringToRelationMap =
    swapPairs<Relation, std::string>(relationToStringMap);

Relation getRelation(std::string& relation_string) {
  return stringToRelationMap.at(relation_string);
}
std::string getStringFromRelation(Relation relation) {
  return relationToStringMap.at(relation);
}
const std::unordered_map<Relation, std::string>& getRelationToStringMap() {
  return relationToStringMap;
}

std::pair<RefType, RefType> getArgTypesFromRelation(Relation& r) {
  return relationToArgTypesMap.at(r);
}

std::pair<std::vector<DesignEntity>, std::vector<DesignEntity>>
getArgSynonymTypesFromRelation(Relation& r) {
  return relationToArgSynonymTypesMap.at(r);
}

}  // namespace QE
