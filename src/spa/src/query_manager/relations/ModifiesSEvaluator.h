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

class ModifiesSEvaluator : public SuchThatEvaluator {
 public:
  ModifiesSEvaluator(Query* query, PKBManager* pkb)
      : SuchThatEvaluator(query, pkb){};

  // Handle cases with at least one variable selected

  AllowedValuesPair handleLeftVarSelectedRightBasic() override {
    // Modifies(s, "x")
    return pkb->getLineModifiesVar(*arg2AsBasic)
        .value_or(std::vector<std::string>());
  }
  AllowedValuesPair handleRightVarSelectedLeftBasic() override {
    // Modifies(3, v)
    return pkb->getVarModifiedByLine(*arg1AsBasic)
        .value_or(std::vector<std::string>());
  }
  AllowedValuesPair handleLeftVarSelectedRightUnderscore() override {
    // Modifies(s, _)
    // Note that this should select all whiles and ifs
    auto all_selected_designentities = QueryManager::getSelect(
        pkb, query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (pkb->getVarModifiedByLine(de)) {
        results.push_back(de);
      }
    }
    return results;
  }
  AllowedValuesPair handleRightVarSelectedLeftUnderscore() override {
    std::cout << "Should not happen: ModifiesS first arg cannot be _\n";
    assert(false);
  }
  AllowedValuesPair handleLeftVarSelectedRightVarUnselected() override {
    // Modifies*(s, v)
    if (arg1AsSynonym == arg2AsSynonym) {
      std::cout << "Should not happen: ModifiesS cannot have identical args\n";
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
        if (pkb->isLineModifiesVar(de, unselect_de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
  }
  AllowedValuesPair handleRightVarSelectedLeftVarUnselected() override {
    // Modifies(s1, s)
    if (arg1AsSynonym == arg2AsSynonym) {
      std::cout << "Should not happen: ModifiesS cannot have identical args\n";
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
        if (pkb->isLineModifiesVar(unselect_de, de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
  }

  // Handle cases with no variables selected

  AllowedValuesPair handleDoubleUnderscore() override {
    return !pkb->isLineModifiesVarSetEmpty();
  }
  AllowedValuesPair handleBothVarsUnselected() override {
    // Modifies(s1, s2)
    if (arg1AsSynonym == arg2AsSynonym) {
      std::cout << "Should not happen: ModifiesS cannot have identical args\n";
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
        if (pkb->isLineModifiesVar(left_de, right_de)) {
          return true;
        }
      }
    }
    return false;
  }
  AllowedValuesPair handleLeftVarUnselectedRightBasic() override {
    // Modifies(s1, "x")
    return pkb->getLineModifiesVar(*arg2AsBasic).has_value();
  }
  AllowedValuesPair handleRightVarUnselectedLeftBasic() override {
    // Modifies(3, v1)
    return pkb->getVarModifiedByLine(*arg1AsBasic).has_value();
  }

  // Unlikely that these last 4 will need to be changed
  AllowedValuesPair handleLeftBasicRightUnderscore() override {
    // Modifies(3, _)
    return handleRightVarUnselectedLeftBasic();
  }
  AllowedValuesPair handleRightBasicLeftUnderscore() override {
    // Modifies(_, "x")
    return handleLeftVarUnselectedRightBasic();
  }
  AllowedValuesPair handleLeftVarUnselectedRightUnderscore() override {
    // Modifies*(s1, _) --> is there a statement that modifies anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return !std::get<std::vector<std::string>>(
                handleLeftVarSelectedRightUnderscore())
                .empty();
  }
  AllowedValuesPair handleRightVarUnselectedLeftUnderscore() override {
    // Modifies*(_, v1) --> is there a variable that is modified?
    // Reuse the left-var selected results until an optimized PKB query can help
    return !std::get<std::vector<std::string>>(
                handleRightVarSelectedLeftUnderscore())
                .empty();
  }
};