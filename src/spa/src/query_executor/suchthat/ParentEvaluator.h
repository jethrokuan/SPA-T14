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

class ParentEvaluator : public SuchThatEvaluator {
 public:
  ParentEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  std::vector<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Parent(s, 3)
    if (auto beforeLine = pkb->getParentLine(basic_value)) {
      return {*beforeLine};
    } else {
      return {};
    }
  }
  std::vector<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Parent(3, s)
    if (auto afterLine = pkb->getChildLine(basic_value)) {
      return {*afterLine};
    } else {
      return {};
    }
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Parent(s, _) (for each s)
    return pkb->getChildLine(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // Parent(_, s) (for each s)
    return pkb->getParentLine(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_select,
                              std::string& arg_unselect) override {
    // Parent(s, s1)
    return pkb->isLineParentLine(arg_select, arg_unselect);
  }
  bool handleDoubleUnderscore() override {
    return !pkb->isLineFollowLineSSetEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Parent(3, _)
    return pkb->getChildLine(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Parent(_, 3)
    return pkb->getParentLine(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg1, std::string& arg2) override {
    // Parent(2, 3)?
    return pkb->isLineParentLine(arg1, arg2);
  }
};