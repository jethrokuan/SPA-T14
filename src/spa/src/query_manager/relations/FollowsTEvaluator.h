#pragma once
#include <cassert>
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/pql/pql.h"
#include "query_manager/query_manager.h"
#include "query_manager/relations/SuchThatEvaluator.h"

using namespace PKB;
using namespace QE;

class FollowsTEvaluator : public SuchThatEvaluator {
 public:
  FollowsTEvaluator(Query* query, PKBManager* pkb)
      : SuchThatEvaluator(query, pkb){};

  // Handle cases with at least one variable selected

  BoolOrStrings handleLeftVarSelectedRightBasic() override {
    return pkb->getBeforeLineS(*arg2AsBasic);
  }
  BoolOrStrings handleRightVarSelectedLeftBasic() override {
    return pkb->getFollowingLineS(*arg1AsBasic);
  }
  BoolOrStrings handleLeftVarSelectedRightUnderscore() override {
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (!pkb->getFollowingLineS(de).empty()) {
        results.push_back(de);
      }
    }
    return results;
  }
  BoolOrStrings handleRightVarSelectedLeftUnderscore() override {
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (!pkb->getBeforeLineS(de).empty()) {
        results.push_back(de);
      }
    }
    return results;
  }
  BoolOrStrings handleLeftVarSelectedRightVarUnselected() override {
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return std::vector<std::string>();
    }
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    auto right_arg_de = Declaration::findDeclarationForSynonym(
                            query->declarations, *arg2AsSynonym)
                            ->getDesignEntity();
    auto all_unselected_designentities =
        QueryManager::getSelect(pkb, right_arg_de);
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      for (auto unselect_de : all_unselected_designentities) {
        if (pkb->isLineFollowLineS(de, unselect_de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
  }
  BoolOrStrings handleRightVarSelectedLeftVarUnselected() override {
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return std::vector<std::string>();
    }
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    auto left_arg_de = Declaration::findDeclarationForSynonym(
                           query->declarations, *arg1AsSynonym)
                           ->getDesignEntity();
    auto all_unselected_designentities =
        QueryManager::getSelect(pkb, left_arg_de);
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      for (auto unselect_de : all_unselected_designentities) {
        if (pkb->isLineFollowLineS(unselect_de, de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
  }

  // Handle cases with no variables selected

  BoolOrStrings handleDoubleUnderscore() override {
    // TODO: NOT YET IMPLEMENTED
    assert(false);
  }
  BoolOrStrings handleBothVarsUnselected() override {
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return false;
    }
    auto left_arg_de = Declaration::findDeclarationForSynonym(
                           query->declarations, *arg1AsSynonym)
                           ->getDesignEntity();
    auto right_arg_de = Declaration::findDeclarationForSynonym(
                            query->declarations, *arg1AsSynonym)
                            ->getDesignEntity();

    auto all_left_designentities = QueryManager::getSelect(pkb, left_arg_de);
    auto all_right_designentities = QueryManager::getSelect(pkb, right_arg_de);
    std::vector<std::string> results;
    for (auto left_de : all_left_designentities) {
      for (auto right_de : all_right_designentities) {
        // Any satisfied relation would mean this clause is true overall
        if (pkb->isLineFollowLineS(left_de, right_de)) {
          return true;
        }
      }
    }
    return false;
  }
  BoolOrStrings handleLeftVarUnselectedRightBasic() override {
    return !(pkb->getBeforeLineS(*arg2AsBasic).empty());
  }
  BoolOrStrings handleRightVarUnselectedLeftBasic() override {
    return !(pkb->getFollowingLineS(*arg1AsBasic).empty());
  }
  BoolOrStrings handleLeftVarUnselectedRightUnderscore() override {
    return !(pkb->getFollowingLineS(*arg1AsBasic).empty());
  }
  BoolOrStrings handleRightVarUnselectedLeftUnderscore() override {
    return !(pkb->getBeforeLineS(*arg2AsBasic).empty());
  }
};