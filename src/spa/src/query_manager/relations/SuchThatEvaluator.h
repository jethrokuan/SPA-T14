#pragma once
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/pql/pql.h"
#include "query_manager/query_manager.h"

using namespace PKB;
using namespace QE;

class SuchThatEvaluator {
 protected:
  Query* query;
  PKBManager* pkb;

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
  BoolOrStrings dispatch();

  BoolOrStrings dispatchSuchThatSelected();

  BoolOrStrings dispatchSuchThatNotSelected();

 public:
  SuchThatEvaluator(Query* query, PKBManager* pkb)
      : query(query),
        pkb(pkb),
        arg1(query->such_that->getFirstArg()),
        arg2(query->such_that->getSecondArg()){};
  BoolOrStrings evaluate();

  // These are the individual handler methods for each case
  // Read the .cpp file to see examples of each case
  // TODO: This really shouldn't be public

  // At least one variable is selected
  virtual BoolOrStrings handleLeftVarSelectedRightBasic() = 0;
  virtual BoolOrStrings handleRightVarSelectedLeftBasic() = 0;
  virtual BoolOrStrings handleLeftVarSelectedRightUnderscore() = 0;
  virtual BoolOrStrings handleRightVarSelectedLeftUnderscore() = 0;
  virtual BoolOrStrings handleLeftVarSelectedRightVarUnselected() = 0;
  virtual BoolOrStrings handleRightVarSelectedLeftVarUnselected() = 0;

  // No variable is selected
  virtual BoolOrStrings handleDoubleUnderscore() = 0;
  virtual BoolOrStrings handleBothVarsUnselected() = 0;
  virtual BoolOrStrings handleLeftVarUnselectedRightBasic() = 0;
  virtual BoolOrStrings handleRightVarUnselectedLeftBasic() = 0;
  virtual BoolOrStrings handleLeftVarUnselectedRightUnderscore() = 0;
  virtual BoolOrStrings handleRightVarUnselectedLeftUnderscore() = 0;
};