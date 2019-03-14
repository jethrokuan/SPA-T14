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

class ModifiesPEvaluator : public SuchThatEvaluator {
 public:
  ModifiesPEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : SuchThatEvaluator(query, pkb, qc){};

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // Modifies(p, "x")
    return pkb->getProcedureModifiesVar(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // Modifies("first", v)
    return pkb->getVarModifiedByProcedure(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // Modifies(p, _)
    return pkb->getVarModifiedByProcedure(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string&) override {
    std::cout << "Should not happen: ModifiesS first arg cannot be _\n";
    assert(false);
  }
  bool handleBothArgsSynonyms(std::string& arg_select,
                              std::string& arg_unselect) override {
    // Modifies(p, v)
    return pkb->isProcedureModifiesVar(arg_select, arg_unselect);
  }
  bool handleDoubleUnderscore() override {
    return !pkb->isProcedureModifiesVarSetEmpty();
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // Modifies("first", _)
    return pkb->getVarModifiedByProcedure(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // Modifies(_, "x")
    return pkb->getProcedureModifiesVar(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg1, std::string& arg2) override {
    // Modifies("first", "v")?
    return pkb->isProcedureModifiesVar(arg1, arg2);
  }
};