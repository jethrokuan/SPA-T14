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

class UsesSEvaluator : public SuchThatEvaluator {
 public:
  UsesSEvaluator(Query* query, PKBManager* pkb)
      : SuchThatEvaluator(query, pkb){};

  // Handle cases with at least one variable selected

  BoolOrStrings handleLeftVarSelectedRightBasic() override {
    // Uses(s, "x")
    return pkb->getLineUsesVar(*arg2AsBasic)
        .value_or(std::vector<std::string>());
  }
  BoolOrStrings handleRightVarSelectedLeftBasic() override {
    // Uses(3, v)
    return pkb->getVarUsedByLine(*arg1AsBasic)
        .value_or(std::vector<std::string>());
  }
  BoolOrStrings handleLeftVarSelectedRightUnderscore() override {
    // Uses(s, _)
    // Note that this should select all whiles and ifs
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (pkb->getVarUsedByLine(de)) {
        results.push_back(de);
      }
    }
    return results;
  }
  BoolOrStrings handleRightVarSelectedLeftUnderscore() override {
    std::cout << "Should not happen: UsesS first arg cannot be _\n";
    assert(false);
  }
  BoolOrStrings handleLeftVarSelectedRightVarUnselected() override {
    // Uses*(s, v)
    if (arg1AsSynonym == arg2AsSynonym) {
      std::cout << "Should not happen: UsesS cannot have identical args\n";
      assert(false);
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
        if (pkb->isLineUsesVar(de, unselect_de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
  }
  BoolOrStrings handleRightVarSelectedLeftVarUnselected() override {
    // Uses(s1, s)
    if (arg1AsSynonym == arg2AsSynonym) {
      std::cout << "Should not happen: UsesS cannot have identical args\n";
      assert(false);
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
        if (pkb->isLineUsesVar(unselect_de, de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
  }

  // Handle cases with no variables selected

  BoolOrStrings handleDoubleUnderscore() override {
    return !pkb->isLineUsesVarSetEmpty();
  }
  BoolOrStrings handleBothVarsUnselected() override {
    // Uses(s1, s2)
    if (arg1AsSynonym == arg2AsSynonym) {
      std::cout << "Should not happen: UsesS cannot have identical args\n";
      assert(false);
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
        if (pkb->isLineUsesVar(left_de, right_de)) {
          return true;
        }
      }
    }
    return false;
  }
  BoolOrStrings handleLeftVarUnselectedRightBasic() override {
    // Uses(s1, "x")
    return pkb->getLineUsesVar(*arg2AsBasic).has_value();
  }
  BoolOrStrings handleRightVarUnselectedLeftBasic() override {
    // Uses(3, v1)
    return pkb->getVarUsedByLine(*arg1AsBasic).has_value();
  }

  // Unlikely that these last 4 will need to be changed
  BoolOrStrings handleLeftBasicRightUnderscore() override {
    // Uses(3, _)
    return handleRightVarUnselectedLeftBasic();
  }
  BoolOrStrings handleRightBasicLeftUnderscore() override {
    // Uses(_, "x")
    return handleLeftVarUnselectedRightBasic();
  }
  BoolOrStrings handleLeftVarUnselectedRightUnderscore() override {
    // Uses*(s1, _) --> is there a statement that uses any variable?
    // Reuse the left-var selected results until an optimized PKB query can help
    return !std::get<std::vector<std::string>>(
                handleLeftVarSelectedRightUnderscore())
                .empty();
  }
  BoolOrStrings handleRightVarUnselectedLeftUnderscore() override {
    // Uses*(_, v) --> is there a variable that is used?
    // Reuse the left-var selected results until an optimized PKB query can help
    return !std::get<std::vector<std::string>>(
                handleRightVarSelectedLeftUnderscore())
                .empty();
  }
};