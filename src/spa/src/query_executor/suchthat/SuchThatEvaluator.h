#pragma once
#include <optional>
#include <string>
#include <unordered_set>
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
  bool arg1IsUnderscore;
  bool arg2IsUnderscore;
  std::optional<std::string> arg1AsBasic;
  std::optional<std::string> arg2AsBasic;

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
  SuchThatEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : query(query),
        pkb(pkb),
        qc(qc),
        arg1(query->such_that->getFirstArg()),
        arg2(query->such_that->getSecondArg()){};

  //! External API to run the such-that query on the given PKB instance
  bool evaluate();
};