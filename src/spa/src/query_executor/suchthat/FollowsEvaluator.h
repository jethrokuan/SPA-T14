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

class FollowsEvaluator : public SuchThatEvaluator {
 public:
  FollowsEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Follows(s, 3)
    if (auto beforeLine = pkb->getBeforeLine(basic_value)) {
      return {*beforeLine};
    } else {
      return {};
    }
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Follows(3, s)
    if (auto afterLine = pkb->getFollowingLine(basic_value)) {
      return {*afterLine};
    } else {
      return {};
    }
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Follows(s, _) (for each s)
    return pkb->getFollowingLine(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // Follows(_, s) (for each s)
    return pkb->getBeforeLine(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_left,
                              std::string& arg_right) override {
    // Follows(s1, s2)
    return pkb->isLineFollowLine(arg_left, arg_right);
  }
  bool handleDoubleUnderscore() override {
    // Follows(_, _)
    return !pkb->isLineFollowLineSetEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Follows(3, _)
    return pkb->getFollowingLine(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Follows(_, 3)
    return pkb->getBeforeLine(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg_left,
                           std::string& arg_right) override {
    // Follows(2, 3)
    return pkb->isLineFollowLine(arg_left, arg_right);
  }
};