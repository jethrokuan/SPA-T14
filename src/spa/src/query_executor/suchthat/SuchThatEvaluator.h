#pragma once
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/constraint_solver/constraint_solver.h"
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
  AllowedValuesPairOrBool dispatch();

  AllowedValuesPairOrBool dispatchSuchThatSelected();

  AllowedValuesPairOrBool dispatchSuchThatNotSelected();

 public:
  SuchThatEvaluator(Query* query, PKBManager* pkb, QueryConstraints& qc)
      : query(query),
        pkb(pkb),
        qc(qc),
        arg1(query->such_that->getFirstArg()),
        arg2(query->such_that->getSecondArg()){};
  AllowedValuesPairOrBool evaluate();

  // These are the individual handler methods for each case
  // Read the .cpp file to see examples of each case
  // TODO: This really shouldn't be public

  // At least one variable is selected
  virtual AllowedValuesPairOrBool handleLeftVarSelectedRightBasic() = 0;
  virtual AllowedValuesPairOrBool handleRightVarSelectedLeftBasic() = 0;
  virtual AllowedValuesPairOrBool handleLeftVarSelectedRightUnderscore() = 0;
  virtual AllowedValuesPairOrBool handleRightVarSelectedLeftUnderscore() = 0;
  virtual AllowedValuesPairOrBool handleLeftVarSelectedRightVarUnselected() = 0;
  virtual AllowedValuesPairOrBool handleRightVarSelectedLeftVarUnselected() = 0;

  // No variable is selected
  virtual AllowedValuesPairOrBool handleDoubleUnderscore() = 0;
  virtual AllowedValuesPairOrBool handleBothVarsUnselected() = 0;
  virtual AllowedValuesPairOrBool handleLeftVarUnselectedRightBasic() = 0;
  virtual AllowedValuesPairOrBool handleRightVarUnselectedLeftBasic() = 0;
  virtual AllowedValuesPairOrBool handleLeftBasicRightUnderscore() = 0;
  virtual AllowedValuesPairOrBool handleRightBasicLeftUnderscore() = 0;
  virtual AllowedValuesPairOrBool handleLeftVarUnselectedRightUnderscore() = 0;
  virtual AllowedValuesPairOrBool handleRightVarUnselectedLeftUnderscore() = 0;
};