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

  std::vector<std::string> handleLeftVarSynonymRightBasic(
      std::string& basic_value) override {
    // Parent*(s, 3)
    return pkb->getParentLineS(basic_value)
        .value_or(std::vector<std::string>());
  }
  std::vector<std::string> handleRightVarSynonymLeftBasic(
      std::string& basic_value) override {
    // Parent*(3, s)
    return pkb->getChildLineS(basic_value).value_or(std::vector<std::string>());
  }
  bool handleLeftVarSynonymRightUnderscore(std::string& arg_value) override {
    // Parent*(s, _) (for each s)
    return pkb->getChildLineS(arg_value) ? true : false;
  }
  bool handleRightVarSynonymLeftUnderscore(std::string& arg_value) override {
    // Parent*(_, s) (for each s)
    return pkb->getParentLineS(arg_value) ? true : false;
  }
  bool handleBothVarsSynonyms(std::string& arg_select,
                              std::string& arg_unselect) override {
    // Parent*(s, s1)
    return pkb->isLineParentLineS(arg_select, arg_unselect) ? true : false;
  }

  bool handleRightVarSelectedLeftVarUnselected(
      std::string& arg_unselect, std::string& arg_select) override {
    // Parent*(s1, s)
    return pkb->isLineParentLineS(arg_unselect, arg_select) ? true : false;
  }

  // Handle cases with no variables selected
  bool handleDoubleUnderscore() override {
    return !pkb->isLineFollowLineSSetEmpty();
  }
  bool handleBothVarsUnselected(std::string& left_arg,
                                std::string& right_arg) override {
    // Parent*(s1, s2)
    return pkb->isLineParentLineS(left_arg, right_arg) ? true : false;
  }
  std::vector<std::string> handleLeftVarUnselectedRightBasic(
      std::string& arg) override {
    // Parent*(s1, 3)
    return handleLeftVarSynonymRightBasic(arg);
  }
  std::vector<std::string> handleRightVarUnselectedLeftBasic(
      std::string& arg) override {
    // Parent*(3, s1)
    return handleRightVarSynonymLeftBasic(arg);
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Parent*(3, _)
    return pkb->getChildLineS(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Parent*(_, 3)
    return pkb->getParentLineS(arg).has_value();
  }
  bool handleLeftVarUnselectedRightUnderscore(std::string& arg) override {
    // Parent*(s1, _) --> is there a statement that is followed by anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleLeftVarSynonymRightUnderscore(arg);
  }
  bool handleRightVarUnselectedLeftUnderscore(std::string& arg) override {
    // Parent*(_, s1) --> is there a statement that follows anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleRightVarSynonymLeftUnderscore(arg);
  }
  bool handleDoubleBasic(std::string& arg1, std::string& arg2) override {
    // Parent*(2, 3)?
    return pkb->isLineParentLineS(arg1, arg2);
  }
};