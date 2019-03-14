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

class NextTEvaluator : public SuchThatEvaluator {
 public:
  NextTEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Next*(s, 3)
    return pkb->getPreviousLineT(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Next*(3, s)
    return pkb->getNextLineT(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Next*(s, _) (for each s)
    return pkb->getNextLineT(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // Next*(_, s) (for each s)
    return pkb->getPreviousLineT(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_left,
                              std::string& arg_right) override {
    // Next*(s, s1)
    // TODO: PKB implementation
    assert(false);
    // return pkb->isLineNextLineT(arg_left, arg_right);
  }
  // Handle cases with no variables selected
  bool handleDoubleUnderscore() override {
    // Next*(_, _)
    // This is not using a potential isLineNextLineTSetEmpty
    // Cannot compute beforehand, and the info is same for both sets
    // Next(1,2) ==> Next*(1,2)
    // Next*(1,2) ==> some combination of Next
    // Either both are empty or neither
    return !pkb->isLineNextLineSetEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Next*(3, _)
    return pkb->getNextLineT(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Next*(_, 3)
    return pkb->getPreviousLineT(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg_left, std::string& arg_right) override {
    // Next*(2, 3)?
    // TODO: PKB implementation
    assert(false);
    // return pkb->isLineNextLineT(arg_left, arg_right);
  }
};