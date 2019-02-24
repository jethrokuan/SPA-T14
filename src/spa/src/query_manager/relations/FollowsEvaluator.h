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

class FollowsEvaluator : public SuchThatEvaluator {
 public:
  FollowsEvaluator(Query* query, PKBManager* pkb)
      : SuchThatEvaluator(query, pkb){};

  // Handle cases with at least one variable selected

  BoolOrStrings handleLeftVarSelectedRightBasic() override {
    // Follows(s, 3)
    if (auto beforeLine = pkb->getBeforeLine(*arg2AsBasic)) {
      return std::vector<std::string>{*beforeLine};
    } else {
      return std::vector<std::string>();
    }
  }
  BoolOrStrings handleRightVarSelectedLeftBasic() override {
    // Follows(3, s)
    if (auto afterLine = pkb->getFollowingLine(*arg2AsBasic)) {
      return std::vector<std::string>{*afterLine};
    } else {
      return std::vector<std::string>();
    }
  }
  BoolOrStrings handleLeftVarSelectedRightUnderscore() override {
    // Follows(s, _)
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (pkb->getFollowingLine(de)) {
        results.push_back(de);
      }
    }
    return results;
  }
  BoolOrStrings handleRightVarSelectedLeftUnderscore() override {
    // Follows*(_, s)
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (pkb->getBeforeLine(de)) {
        results.push_back(de);
      }
    }
    return results;
  }
  BoolOrStrings handleLeftVarSelectedRightVarUnselected() override {
    // Follows(s, s1)
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
        if (pkb->isLineFollowLine(de, unselect_de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
  }
  BoolOrStrings handleRightVarSelectedLeftVarUnselected() override {
    // Follows(s1, s)
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
        if (pkb->isLineFollowLine(unselect_de, de) &&
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
    // Follows(s1, s2)
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
        if (pkb->isLineFollowLine(left_de, right_de)) {
          return true;
        }
      }
    }
    return false;
  }
  BoolOrStrings handleLeftVarUnselectedRightBasic() override {
    // Follows(s1, 3)
    return pkb->getBeforeLine(*arg2AsBasic).has_value();
  }
  BoolOrStrings handleRightVarUnselectedLeftBasic() override {
    // Follows(3, s1)
    return pkb->getFollowingLine(*arg1AsBasic).has_value();
  }
  BoolOrStrings handleLeftBasicRightUnderscore() override {
    // Follows(3, _)
    return handleRightVarUnselectedLeftBasic();
  }
  BoolOrStrings handleRightBasicLeftUnderscore() override {
    // Follows(_, 3)
    return handleLeftVarUnselectedRightBasic();
  }
  BoolOrStrings handleLeftVarUnselectedRightUnderscore() override {
    // Follows(s1, _) --> is there a statement that is followed by anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return !std::get<std::vector<std::string>>(
                handleLeftVarSelectedRightUnderscore())
                .empty();
  }
  BoolOrStrings handleRightVarUnselectedLeftUnderscore() override {
    // Follows(_, s1) --> is there a statement that follows anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return !std::get<std::vector<std::string>>(
                handleRightVarSelectedLeftUnderscore())
                .empty();
  }
};