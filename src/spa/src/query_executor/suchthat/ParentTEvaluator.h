#pragma once
#include <cassert>
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/query_executor.h"
#include "query_executor/suchthat/SuchThatEvaluator.h"

using namespace PKB;
using namespace QE;

class ParentTEvaluator : public SuchThatEvaluator {
 public:
  ParentTEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  // Handle cases with at least one variable selected

  AllowedValuesPairOrBool handleLeftVarSelectedRightBasic() override {
    // Parent*(s, 3)
    auto results =
        pkb->getParentLineS(*arg2AsBasic).value_or(std::vector<std::string>());
    return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, results);
  }
  AllowedValuesPairOrBool handleRightVarSelectedLeftBasic() override {
    // Parent*(3, s)
    auto results =
        pkb->getChildLineS(*arg1AsBasic).value_or(std::vector<std::string>());
    return ConstraintSolver::makeAllowedValues(*arg2AsSynonym, results);
  }
  AllowedValuesPairOrBool handleLeftVarSelectedRightUnderscore() override {
    // Parent*(s, _)
    // Note that this should select all whiles and ifs
    auto all_selected_designentities = QueryExecutor::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (pkb->getChildLineS(de)) {
        results.push_back(de);
      }
    }
    return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, results);
    ;
  }
  AllowedValuesPairOrBool handleRightVarSelectedLeftUnderscore() override {
    // Parent*(_, s)
    auto all_selected_designentities = QueryExecutor::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (pkb->getParentLineS(de)) {
        results.push_back(de);
      }
    }
    return ConstraintSolver::makeAllowedValues(*arg2AsSynonym, results);
  }
  AllowedValuesPairOrBool handleLeftVarSelectedRightVarUnselected() override {
    // Parent*(s, s1)
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return ConstraintSolver::makeEmptyAllowedValuesPairForSynonyms(
          *arg1AsSynonym, *arg2AsSynonym);
    }
    auto all_selected_designentities = QueryExecutor::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    auto right_arg_de = Declaration::findDeclarationForSynonym(
                            query->declarations, *arg2AsSynonym)
                            ->getDesignEntity();
    auto all_unselected_designentities =
        QueryExecutor::getSelect(pkb, right_arg_de);
    AllowedValuePairSet results;
    for (auto de : all_selected_designentities) {
      for (auto unselect_de : all_unselected_designentities) {
        if (pkb->isLineParentLineS(de, unselect_de)) {
          results.insert({de, unselect_de});
        }
      }
    }
    return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, *arg2AsSynonym,
                                               results);
  }
  AllowedValuesPairOrBool handleRightVarSelectedLeftVarUnselected() override {
    // Parent*(s1, s)
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return ConstraintSolver::makeEmptyAllowedValuesPairForSynonyms(
          *arg1AsSynonym, *arg2AsSynonym);
    }
    auto all_selected_designentities = QueryExecutor::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    auto left_arg_de = Declaration::findDeclarationForSynonym(
                           query->declarations, *arg1AsSynonym)
                           ->getDesignEntity();
    auto all_unselected_designentities =
        QueryExecutor::getSelect(pkb, left_arg_de);
    AllowedValuePairSet results;
    for (auto de : all_selected_designentities) {
      for (auto unselect_de : all_unselected_designentities) {
        if (pkb->isLineParentLineS(unselect_de, de)) {
          results.insert({unselect_de, de});
        }
      }
    }
    return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, *arg2AsSynonym,
                                               results);
  }

  // Handle cases with no variables selected

  AllowedValuesPairOrBool handleDoubleUnderscore() override {
    return !pkb->isLineParentLineSSetEmpty();
  }
  AllowedValuesPairOrBool handleBothVarsUnselected() override {
    // Parent*(s1, s2)
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

    auto all_left_designentities = QueryExecutor::getSelect(pkb, left_arg_de);
    auto all_right_designentities = QueryExecutor::getSelect(pkb, right_arg_de);
    AllowedValuePairSet results;
    for (auto left_de : all_left_designentities) {
      for (auto right_de : all_right_designentities) {
        // Any satisfied relation would mean this clause is true overall
        if (pkb->isLineParentLineS(left_de, right_de)) {
          results.insert({left_de, right_de});
        }
      }
    }
    return ConstraintSolver::makeAllowedValues(*arg1AsSynonym, *arg2AsSynonym,
                                               results);
  }
  AllowedValuesPairOrBool handleLeftVarUnselectedRightBasic() override {
    // Parent*(s1, 3)
    return handleLeftVarSelectedRightBasic();
  }
  AllowedValuesPairOrBool handleRightVarUnselectedLeftBasic() override {
    // Parent*(3, s1)
    return handleRightVarSelectedLeftBasic();
  }

  AllowedValuesPairOrBool handleLeftBasicRightUnderscore() override {
    // Parent*(3, _)
    return pkb->getChildLineS(*arg1AsBasic).has_value();
  }
  AllowedValuesPairOrBool handleRightBasicLeftUnderscore() override {
    // Parent*(_, 3)
    return pkb->getParentLineS(*arg2AsBasic).has_value();
  }
  AllowedValuesPairOrBool handleLeftVarUnselectedRightUnderscore() override {
    // Parent*(s1, _) --> is there a statement that is a parent of anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleLeftVarSelectedRightUnderscore();
  }
  AllowedValuesPairOrBool handleRightVarUnselectedLeftUnderscore() override {
    // Parent*(_, s1) --> is there a statement that is a child of anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleRightVarSelectedLeftUnderscore();
  }
};