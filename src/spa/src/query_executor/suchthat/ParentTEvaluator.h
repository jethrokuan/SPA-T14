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

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Parent*(s, 3)
    return pkb->getParentLineS(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Parent*(3, s)
    return pkb->getChildLineS(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Parent*(s, _) (for each s)
    return pkb->getChildLineS(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // Parent*(_, s) (for each s)
    return pkb->getParentLineS(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_select,
                              std::string& arg_unselect) override {
    // Parent*(s, s1)
    return pkb->isLineParentLineS(arg_select, arg_unselect);
  }
  bool handleDoubleUnderscore() override {
    return !pkb->isLineFollowLineSSetEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Parent*(3, _)
    return pkb->getChildLineS(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Parent*(_, 3)
    return pkb->getParentLineS(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg1, std::string& arg2) override {
    // Parent*(2, 3)?
    return pkb->isLineParentLineS(arg1, arg2);
  }
};