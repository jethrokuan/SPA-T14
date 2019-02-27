#pragma once
#include <cassert>
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/pql/pql.h"
#include "query_manager/query_manager.h"
#include "query_manager/suchthat/SuchThatEvaluator.h"

using namespace PKB;
using namespace QE;

class FollowsEvaluator : public SuchThatEvaluator {
 public:
  FollowsEvaluator(Query* query, PKBManager* pkb)
      : SuchThatEvaluator(query, pkb){};

  // Handle cases with at least one variable selected

  AllowedValuesPairOrBool handleLeftVarSelectedRightBasic() override {
    // Follows(s, 3)
    if (auto beforeLine = pkb->getBeforeLine(*arg2AsBasic)) {
      auto results = std::vector<std::string>{*beforeLine};
      return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, results);
    } else {
      auto results = std::vector<std::string>{};
      return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, results);
    }
  }
  AllowedValuesPairOrBool handleRightVarSelectedLeftBasic() override {
    // Follows(3, s)
    if (auto afterLine = pkb->getFollowingLine(*arg1AsBasic)) {
      auto results = std::vector<std::string>{*afterLine};
      return ConstraintSolver::makeAllowedValues(*arg2AsSynonym, results);
    } else {
      auto results = std::vector<std::string>{};
      return ConstraintSolver::makeAllowedValues(*arg2AsSynonym, results);
    }
  }
  AllowedValuesPairOrBool handleLeftVarSelectedRightUnderscore() override {
    // Follows(s, _)
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (pkb->getFollowingLine(de)) {
        results.push_back(de);
      }
    }
    return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, results);
  }
  AllowedValuesPairOrBool handleRightVarSelectedLeftUnderscore() override {
    // Follows*(_, s)
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (pkb->getBeforeLine(de)) {
        results.push_back(de);
      }
    }
    return ConstraintSolver::makeAllowedValues(*arg2AsSynonym, results);
  }
  AllowedValuesPairOrBool handleLeftVarSelectedRightVarUnselected() override {
    // Follows(s, s1)
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return ConstraintSolver::makeEmptyAllowedValuesPairForSynonyms(
          *arg1AsSynonym, *arg2AsSynonym);
    }
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    auto right_arg_de = Declaration::findDeclarationForSynonym(
                            query->declarations, *arg2AsSynonym)
                            ->getDesignEntity();
    auto all_unselected_designentities =
        QueryManager::getSelect(pkb, right_arg_de);
    AllowedValueSet results;
    for (auto de : all_selected_designentities) {
      for (auto unselect_de : all_unselected_designentities) {
        if (pkb->isLineFollowLine(de, unselect_de)) {
          results.insert({de, unselect_de});
        }
      }
    }
    return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, *arg2AsSynonym,
                                               results);
  }
  AllowedValuesPairOrBool handleRightVarSelectedLeftVarUnselected() override {
    // Follows(s1, s)
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return ConstraintSolver::makeEmptyAllowedValuesPairForSynonyms(
          *arg1AsSynonym, *arg2AsSynonym);
    }
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    auto left_arg_de = Declaration::findDeclarationForSynonym(
                           query->declarations, *arg1AsSynonym)
                           ->getDesignEntity();
    auto all_unselected_designentities =
        QueryManager::getSelect(pkb, left_arg_de);
    AllowedValueSet results;
    for (auto de : all_selected_designentities) {
      for (auto unselect_de : all_unselected_designentities) {
        if (pkb->isLineFollowLine(unselect_de, de)) {
          results.insert({unselect_de, de});
        }
      }
    }
    return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, *arg2AsSynonym,
                                               results);
  }

  // Handle cases with no variables selected

  AllowedValuesPairOrBool handleDoubleUnderscore() override {
    return !pkb->isLineFollowLineSetEmpty();
  }
  AllowedValuesPairOrBool handleBothVarsUnselected() override {
    // Follows(s1, s2)
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return ConstraintSolver::makeEmptyAllowedValuesPairForSynonyms(
          *arg1AsSynonym, *arg2AsSynonym);
    }
    auto left_arg_de = Declaration::findDeclarationForSynonym(
                           query->declarations, *arg1AsSynonym)
                           ->getDesignEntity();
    auto right_arg_de = Declaration::findDeclarationForSynonym(
                            query->declarations, *arg1AsSynonym)
                            ->getDesignEntity();

    auto all_left_designentities = QueryManager::getSelect(pkb, left_arg_de);
    auto all_right_designentities = QueryManager::getSelect(pkb, right_arg_de);
    AllowedValueSet results;
    for (auto left_de : all_left_designentities) {
      for (auto right_de : all_right_designentities) {
        // Any satisfied relation would mean this clause is true overall
        if (pkb->isLineFollowLine(left_de, right_de)) {
          results.insert({left_de, right_de});
        }
      }
    }
    return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, *arg2AsSynonym,
                                               results);
  }
  AllowedValuesPairOrBool handleLeftVarUnselectedRightBasic() override {
    // Follows(s1, 3)
    return handleLeftVarSelectedRightBasic();
  }
  AllowedValuesPairOrBool handleRightVarUnselectedLeftBasic() override {
    // Follows(3, s1)
    return handleRightVarSelectedLeftBasic();
  }
  AllowedValuesPairOrBool handleLeftBasicRightUnderscore() override {
    // Follows(3, _)
    return pkb->getFollowingLine(*arg1AsBasic).has_value();
  }
  AllowedValuesPairOrBool handleRightBasicLeftUnderscore() override {
    // Follows(_, 3)
    return pkb->getBeforeLine(*arg2AsBasic).has_value();
  }
  AllowedValuesPairOrBool handleLeftVarUnselectedRightUnderscore() override {
    // Follows(s1, _) --> is there a statement that is followed by anything?
    // Reuse the left-var selected results until an optimized PKB query can
    // help
    return handleLeftVarSelectedRightUnderscore();
  }
  AllowedValuesPairOrBool handleRightVarUnselectedLeftUnderscore() override {
    // Follows(_, s1) --> is there a statement that follows anything?
    // Reuse the left-var selected results until an optimized PKB query can
    // help
    return handleRightVarSelectedLeftUnderscore();
  }
};