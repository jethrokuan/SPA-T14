#pragma once
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
// #include "query_executor/constraint_solver/constraint_solver.h"
#include "query_executor/query_executor.h"

using namespace PKB;
using namespace QE;

class SuchThatEvaluator {
 protected:
  Query* query;
  PKBManager* pkb;
  QueryConstraints& qc;

  // Calculated from the Query object
  QE::StmtOrEntRef arg1;
  QE::StmtOrEntRef arg2;
  std::optional<Synonym> arg1AsSynonym;
  std::optional<Synonym> arg2AsSynonym;
  bool arg1InSelect;
  bool arg2InSelect;
  bool arg1IsUnderscore;
  bool arg2IsUnderscore;
  std::optional<std::string> arg1AsBasic;
  std::optional<std::string> arg2AsBasic;

  //! Dispatches such that query to individual methods to handle it
  bool dispatch();

  bool dispatchSuchThatSelected();

  bool dispatchSuchThatNotSelected();

  // These are the individual handler methods for each case
  // Read the .cpp file to see examples of each case

  // At least one variable is selected
  bool dispatchLeftVarSelectedRightBasic();
  bool dispatchRightVarSelectedLeftBasic();
  bool dispatchLeftVarSelectedRightUnderscore();
  bool dispatchRightVarSelectedLeftUnderscore();
  bool dispatchLeftVarSelectedRightVarUnselected();
  bool dispatchRightVarSelectedLeftVarUnselected();

  // No variable is selected
  bool dispatchDoubleUnderscore();
  bool dispatchBothVarsUnselected();
  bool dispatchLeftVarUnselectedRightBasic();
  bool dispatchRightVarUnselectedLeftBasic();
  bool dispatchLeftBasicRightUnderscore();
  bool dispatchRightBasicLeftUnderscore();
  bool dispatchLeftVarUnselectedRightUnderscore();
  bool dispatchRightVarUnselectedLeftUnderscore();

  // These are the PKB calls that need to be overloaded for each subclass
  // At least one variable is selected
  virtual std::vector<std::string> handleLeftVarSelectedRightBasic(
      std::string&) = 0;
  virtual std::vector<std::string> handleRightVarSelectedLeftBasic(
      std::string&) = 0;
  virtual bool handleLeftVarSelectedRightUnderscore(std::string&) = 0;
  virtual bool handleRightVarSelectedLeftUnderscore(std::string&) = 0;
  virtual bool handleLeftVarSelectedRightVarUnselected(std::string&,
                                                       std::string&) = 0;
  virtual bool handleRightVarSelectedLeftVarUnselected(std::string&,
                                                       std::string&) = 0;

  // No variable is selected
  virtual bool handleDoubleUnderscore() = 0;
  virtual bool handleBothVarsUnselected(std::string&, std::string&) = 0;
  virtual std::vector<std::string> handleLeftVarUnselectedRightBasic(
      std::string&) = 0;
  virtual std::vector<std::string> handleRightVarUnselectedLeftBasic(
      std::string&) = 0;
  virtual bool handleLeftBasicRightUnderscore(std::string&) = 0;
  virtual bool handleRightBasicLeftUnderscore(std::string&) = 0;
  virtual bool handleLeftVarUnselectedRightUnderscore(std::string&) = 0;
  virtual bool handleRightVarUnselectedLeftUnderscore(std::string&) = 0;

 public:
  SuchThatEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : query(query),
        pkb(pkb),
        qc(qc),
        arg1(query->such_that->getFirstArg()),
        arg2(query->such_that->getSecondArg()){};

  //! External API to run the such-that query on the given PKB instance
  bool evaluate();
};