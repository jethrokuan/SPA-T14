
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

class AffectsPEvaluator : public SuchThatEvaluator {
 public:
  AffectsPEvaluator(std::vector<QE::Declaration>* decls, QE::RelCond* relCond,
                    PKBManager* pkb, ConstraintDatabase& db)
      : SuchThatEvaluator(decls, relCond, pkb, db){};

  std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string& basic_value) override {
    // AffectsP(a, 1)
    return pkb->getAffectModifiesLineBip(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string& basic_value) override {
    // AffectsP(1, a)
    return pkb->getAffectUsesLineBip(basic_value)
        .value_or(std::unordered_set<std::string>());
  }
  bool handleLeftSynonymRightUnderscore(std::string& arg_value) override {
    // AffectsP(a, _)  (for each a)
    return pkb->getAffectUsesLineBip(arg_value).has_value();
  }
  bool handleRightSynonymLeftUnderscore(std::string& arg_value) override {
    // AffectsP(_, a) (for each a)
    return pkb->getAffectModifiesLineBip(arg_value).has_value();
  }
  bool handleBothArgsSynonyms(std::string& arg_left,
                              std::string& arg_right) override {
    // AffectsP(a1, 2) (for each a1, a2)
    return pkb->isLineAffectsLineBip(arg_left, arg_right);
  }
  bool handleDoubleUnderscore() override {
    // AffectsP(_, _)
    // Must check all possible combinations to see if one is true

    SingleConstraintSet lhs_designentities =
        QueryExecutor::getSelect(pkb, DesignEntity::ASSIGN);
    SingleConstraintSet rhs_designentities =
        QueryExecutor::getSelect(pkb, DesignEntity::ASSIGN);

    PairedConstraintSet results;
    for (auto lhs_de : lhs_designentities) {
      for (auto rhs_de : rhs_designentities) {
        if (pkb->isLineAffectsLineBip(lhs_de, rhs_de)) {
          return true;
        }
      }
    }
    return false;
  }
  bool handleLeftBasicRightUnderscore(std::string& arg) override {
    // AffectsP(1, _)
    return pkb->getAffectUsesLineBip(arg).has_value();
  }
  bool handleRightBasicLeftUnderscore(std::string& arg) override {
    // AffectsP(_, 1)
    return pkb->getAffectModifiesLineBip(arg).has_value();
  }
  bool handleBothArgsBasic(std::string& arg_left,
                           std::string& arg_right) override {
    // AffectsP(1, 2)
    return pkb->isLineAffectsLineBip(arg_left, arg_right);
  }
};