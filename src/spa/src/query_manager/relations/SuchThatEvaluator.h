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
  AllowedValuesPair dispatch();

  AllowedValuesPair dispatchSuchThatSelected();

  AllowedValuesPair dispatchSuchThatNotSelected();

 public:
  SuchThatEvaluator(Query* query, PKBManager* pkb)
      : query(query),
        pkb(pkb),
        arg1(query->such_that->getFirstArg()),
        arg2(query->such_that->getSecondArg()){};
  AllowedValuesPair evaluate();

  // These are the individual handler methods for each case
  // Read the .cpp file to see examples of each case
  // TODO: This really shouldn't be public

  // At least one variable is selected
  virtual AllowedValuesPair handleLeftVarSelectedRightBasic() = 0;
  virtual AllowedValuesPair handleRightVarSelectedLeftBasic() = 0;
  virtual AllowedValuesPair handleLeftVarSelectedRightUnderscore() = 0;
  virtual AllowedValuesPair handleRightVarSelectedLeftUnderscore() = 0;
  virtual AllowedValuesPair handleLeftVarSelectedRightVarUnselected() = 0;
  virtual AllowedValuesPair handleRightVarSelectedLeftVarUnselected() = 0;

  // No variable is selected
  virtual AllowedValuesPair handleDoubleUnderscore() = 0;
  virtual AllowedValuesPair handleBothVarsUnselected() = 0;
  virtual AllowedValuesPair handleLeftVarUnselectedRightBasic() = 0;
  virtual AllowedValuesPair handleRightVarUnselectedLeftBasic() = 0;
  virtual AllowedValuesPair handleLeftBasicRightUnderscore() = 0;
  virtual AllowedValuesPair handleRightBasicLeftUnderscore() = 0;
  virtual AllowedValuesPair handleLeftVarUnselectedRightUnderscore() = 0;
  virtual AllowedValuesPair handleRightVarUnselectedLeftUnderscore() = 0;
};