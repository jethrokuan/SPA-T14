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

class ModifiesSEvaluator : public SuchThatEvaluator {
 public:
  ModifiesSEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  // Handle cases with at least one variable selected
  std::vector<std::string> handleLeftVarSynonymRightBasic(
      std::string& basic_value) override {
    // Modifies(s, "x")
    return pkb->getLineModifiesVar(basic_value)
        .value_or(std::vector<std::string>());
  }
  std::vector<std::string> handleRightVarSynonymLeftBasic(
      std::string& basic_value) override {
    // Modifies(3, v)
    return pkb->getVarModifiedByLine(basic_value)
        .value_or(std::vector<std::string>());
  }
  bool handleLeftVarSynonymRightUnderscore(std::string& arg_value) override {
    // Modifies(s, _)
    return pkb->getVarModifiedByLine(arg_value) ? true : false;
  }
  bool handleRightVarSynonymLeftUnderscore(std::string&) override {
    std::cout << "Should not happen: ModifiesS first arg cannot be _\n";
    assert(false);
  }
  bool handleBothVarsSynonyms(std::string& arg_select,
                              std::string& arg_unselect) override {
    // Modifies(s, v)
    return pkb->isLineModifiesVar(arg_select, arg_unselect) ? true : false;
  }

  bool handleRightVarSelectedLeftVarUnselected(
      std::string& arg_unselect, std::string& arg_select) override {
    // Modifies(s1, s)
    return pkb->isLineModifiesVar(arg_unselect, arg_select) ? true : false;
  }

  // Handle cases with no variables selected
  bool handleDoubleUnderscore() override {
    return !pkb->isLineModifiesVarSetEmpty();
  }
  bool handleBothVarsUnselected(std::string& left_arg,
                                std::string& right_arg) override {
    // Modifies(s1, s2)
    return pkb->isLineModifiesVar(left_arg, right_arg) ? true : false;
  }
  std::vector<std::string> handleLeftVarUnselectedRightBasic(
      std::string& arg) override {
    // Modifies(s1, "x")
    return handleLeftVarSynonymRightBasic(arg);
  }
  std::vector<std::string> handleRightVarUnselectedLeftBasic(
      std::string& arg) override {
    // Modifies(3, v1)
    return handleRightVarSynonymLeftBasic(arg);
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Modifies(3, _)
    return pkb->getVarModifiedByLine(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Modifies(_, "x")
    return pkb->getLineModifiesVar(arg).has_value();
  }
  bool handleLeftVarUnselectedRightUnderscore(std::string& arg) override {
    // Modifies(s1, _) --> is there a statement that modifies anything?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleLeftVarSynonymRightUnderscore(arg);
  }
  bool handleRightVarUnselectedLeftUnderscore(std::string& arg) override {
    // Modifies(_, v1) --> is there a variable that is modified?
    // Reuse the left-var selected results until an optimized PKB query can help
    return handleRightVarSynonymLeftUnderscore(arg);
  }
  bool handleDoubleBasic(std::string& arg1, std::string& arg2) override {
    // Modifies(2, "v")?
    return pkb->isLineModifiesVar(arg1, arg2);
  }
};