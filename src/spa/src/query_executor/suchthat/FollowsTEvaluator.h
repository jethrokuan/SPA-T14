#pragma once
#include <cassert>
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
// #include "query_executor/constraint_solver/constraint_solver.h"
#include "query_executor/query_executor.h"
#include "query_executor/suchthat/SuchThatEvaluator.h"

using namespace PKB;
using namespace QE;

class FollowsTEvaluator : public SuchThatEvaluator {
 public:
  FollowsTEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  std::vector<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Follows*(s, 3)
    return pkb->getBeforeLineS(basic_value)
        .value_or(std::vector<std::string>());
  }
  std::vector<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Follows*(3, s)
    return pkb->getFollowingLineS(basic_value)
        .value_or(std::vector<std::string>());
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Follows*(s, _) (for each s)
    return pkb->getFollowingLineS(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // Follows*(_, s) (for each s)
    return pkb->getBeforeLineS(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_select,
                              std::string& arg_unselect) override {
    // Follows*(s, s1)
    return pkb->isLineFollowLineS(arg_select, arg_unselect);
  }
  // Handle cases with no variables selected
  bool handleDoubleUnderscore() override {
    return !pkb->isLineFollowLineSSetEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Follows*(3, _)
    return pkb->getFollowingLineS(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Follows*(_, 3)
    return pkb->getBeforeLineS(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg1, std::string& arg2) override {
    // Follows*(2, 3)?
    return pkb->isLineFollowLineS(arg1, arg2);
  }
};