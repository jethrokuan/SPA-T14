#pragma once
#include <optional>
#include <string>
#include <unordered_set>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/query_executor.h"

using namespace PKB;
using namespace QE;

class SuchThatEvaluator {
 protected:
  std::vector<QE::Declaration>* declarations;
  QE::RelCond* relCond;
  PKBManager* pkb;
  ConstraintDatabase& db;

  // Calculated from the Query object
  QE::Ref argLeft;
  QE::Ref argRight;
  std::optional<Synonym> argLeftAsSynonym;
  std::optional<Synonym> argRightAsSynonym;
  bool argLeftIsUnderscore;
  bool argRightIsUnderscore;
  std::optional<std::string> argLeftAsBasic;
  std::optional<std::string> argRightAsBasic;

  //! Dispatches such that query to individual methods to handle it
  bool dispatch();

  // These are the individual handler methods for each case
  // Read the .cpp file to see examples of each case

  bool dispatchLeftSynonymRightBasic();
  bool dispatchRightSynonymLeftBasic();
  bool dispatchLeftVarSynonymRightUnderscore();
  bool dispatchRightVarSynonymLeftUnderscore();
  bool dispatchBothVarsSynonyms();
  bool dispatchDoubleUnderscore();
  bool dispatchLeftBasicRightUnderscore();
  bool dispatchRightBasicLeftUnderscore();
  bool dispatchBothBasic();

  // These are the PKB calls that need to be overloaded for each subclass
  virtual std::unordered_set<std::string> handleLeftSynonymRightBasic(
      std::string&) = 0;
  virtual std::unordered_set<std::string> handleRightSynonymLeftBasic(
      std::string&) = 0;
  virtual bool handleLeftSynonymRightUnderscore(std::string&) = 0;
  virtual bool handleRightSynonymLeftUnderscore(std::string&) = 0;
  virtual bool handleBothArgsSynonyms(std::string&, std::string&) = 0;
  virtual bool handleDoubleUnderscore() = 0;
  virtual bool handleLeftBasicRightUnderscore(std::string&) = 0;
  virtual bool handleRightBasicLeftUnderscore(std::string&) = 0;
  virtual bool handleBothArgsBasic(std::string&, std::string&) = 0;

 public:
  SuchThatEvaluator(std::vector<QE::Declaration>* decls, QE::RelCond* relCond,
                    PKBManager* pkb, ConstraintDatabase& db)
      : declarations(decls),
        relCond(relCond),
        pkb(pkb),
        db(db),
        argLeft(relCond->arg1),
        argRight(relCond->arg2){};

  //! External API to run the such-that query on the given PKB instance
  bool evaluate();
};