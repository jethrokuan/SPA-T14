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

class NextPEvaluator : public SuchThatEvaluator {
 public:
  NextPEvaluator(std::vector<QE::Declaration>* decls, QE::RelCond* relCond,
                 PKBManager* pkb, ConstraintDatabase& db)
      : SuchThatEvaluator(decls, relCond, pkb, db){};

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // NextP(s, 3)
    if (auto beforeLine = pkb->getPreviousLineBip(basic_value)) {
      return {*beforeLine};
    } else {
      return {};
    }
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // NextP(3, s)
    if (auto afterLine = pkb->getNextLineBip(basic_value)) {
      return {*afterLine};
    } else {
      return {};
    }
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // NextP(s, _) (for each s)
    return pkb->getNextLineBip(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // NextP(_, s) (for each s)
    return pkb->getPreviousLineBip(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_left,
                              std::string& arg_right) override {
    // NextP(s1, s2)
    return pkb->isLineNextLineBip(arg_left, arg_right);
  }
  bool handleDoubleUnderscore() override {
    // NextP(_, _)
    return !pkb->isLineNextLineMapEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // NextP(3, _)
    return pkb->getNextLineBip(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // NextP(_, 3)
    return pkb->getPreviousLineBip(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg_left,
                           std::string& arg_right) override {
    // NextP(2, 3)
    return pkb->isLineNextLineBip(arg_left, arg_right);
  }
};