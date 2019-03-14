#pragma once
#include <cassert>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/query_executor.h"
#include "query_executor/suchthat/SuchThatEvaluator.h"

using namespace PKB;
using namespace QE;

class NextEvaluator : public SuchThatEvaluator {
 public:
  NextEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Next(s, 3)
    if (auto beforeLine = pkb->getPreviousLine(basic_value)) {
      return {*beforeLine};
    } else {
      return {};
    }
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Next(3, s)
    if (auto afterLine = pkb->getNextLine(basic_value)) {
      return {*afterLine};
    } else {
      return {};
    }
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Next(s, _) (for each s)
    return pkb->getNextLine(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // Next(_, s) (for each s)
    return pkb->getPreviousLine(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_left,
                              std::string& arg_right) override {
    // Next(s1, s2)
    return pkb->isLineNextLine(arg_left, arg_right);
  }
  bool handleDoubleUnderscore() override {
    // Next(_, _)
    return !pkb->isLineNextLineSetEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Next(3, _)
    return pkb->getNextLine(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Next(_, 3)
    return pkb->getPreviousLine(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg_left,
                           std::string& arg_right) override {
    // Next(2, 3)
    return pkb->isLineNextLine(arg_left, arg_right);
  }
};