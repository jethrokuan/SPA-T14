#include <algorithm>
#include <iostream>

#include <vector>

#include "query_builder/pql/relation.h"
#include "query_builder/pql/stmtentref.h"
#include "utils/utils.h"

using namespace Utils;

namespace QE {

std::unordered_map<Relation, std::string> relationToStringMap(
    {{Relation::Affects, "Affects"},
     {Relation::AffectsT, "Affects*"},
     {Relation::ModifiesS, "Modifies(S)"},
     {Relation::UsesS, "Uses(S)"},
     {Relation::ModifiesP, "Modifies(P)"},
     {Relation::UsesP, "Uses(P)"},
     {Relation::Modifies, "Modifies"},
     {Relation::Uses, "Uses"},
     {Relation::Parent, "Parent"},
     {Relation::ParentT, "Parent*"},
     {Relation::Follows, "Follows"},
     {Relation::FollowsT, "Follows*"},
     {Relation::Calls, "Calls"},
     {Relation::CallsT, "Calls*"},
     {Relation::Next, "Next"},
     {Relation::NextT, "Next*"}});

//! Maps each relation to design entities for each of its allowed arguments if
//! they are synonyms
std::unordered_map<
    Relation, std::pair<std::vector<DesignEntity>, std::vector<DesignEntity>>>
    relationToArgSynonymTypesMap(
        {{Relation::ModifiesS,
          // No PRINT design entity for Modifies: print does not modify
          std::make_pair(ALL_STMT_DESIGN_ENTITIES_NO_PRINT,
                         VARIABLE_DESIGN_ENTITY)},
         // No READ design entity for Uses: read modifies a variable - not uses
         {Relation::UsesS, std::make_pair(ALL_STMT_DESIGN_ENTITIES_NO_READ,
                                          VARIABLE_DESIGN_ENTITY)},
         {Relation::ModifiesP,
          std::make_pair(PROCEDURE_DESIGN_ENTITY, VARIABLE_DESIGN_ENTITY)},
         {Relation::UsesP,
          std::make_pair(PROCEDURE_DESIGN_ENTITY, VARIABLE_DESIGN_ENTITY)},
         {Relation::Parent,
          std::make_pair(ALL_STMT_DESIGN_ENTITIES, ALL_STMT_DESIGN_ENTITIES)},
         {Relation::ParentT,
          std::make_pair(ALL_STMT_DESIGN_ENTITIES, ALL_STMT_DESIGN_ENTITIES)},
         {Relation::Follows,
          std::make_pair(ALL_STMT_DESIGN_ENTITIES, ALL_STMT_DESIGN_ENTITIES)},
         {Relation::FollowsT,
          std::make_pair(ALL_STMT_DESIGN_ENTITIES, ALL_STMT_DESIGN_ENTITIES)},
         {Relation::Calls,
          std::make_pair(PROCEDURE_DESIGN_ENTITY, PROCEDURE_DESIGN_ENTITY)},
         {Relation::CallsT,
          std::make_pair(PROCEDURE_DESIGN_ENTITY, PROCEDURE_DESIGN_ENTITY)},
         {Relation::Next,
          std::make_pair(PROG_LINE_DESIGN_ENTITY, PROG_LINE_DESIGN_ENTITY)},
         {Relation::NextT,
          std::make_pair(PROG_LINE_DESIGN_ENTITY, PROG_LINE_DESIGN_ENTITY)},
         {Relation::Affects,
          std::make_pair(ASSIGN_DESIGN_ENTITY, ASSIGN_DESIGN_ENTITY)},
         {Relation::AffectsT,
          std::make_pair(ASSIGN_DESIGN_ENTITY, ASSIGN_DESIGN_ENTITY)}});

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

std::pair<std::vector<DesignEntity>, std::vector<DesignEntity>>
getArgSynonymTypesFromRelation(Relation& r) {
  return relationToArgSynonymTypesMap.at(r);
}

}  // namespace QE
