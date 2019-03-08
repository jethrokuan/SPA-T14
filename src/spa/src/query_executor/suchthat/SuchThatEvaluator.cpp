#include "query_executor/suchthat/SuchThatEvaluator.h"
#include <cassert>
#include <string>
#include <vector>
#include "query_builder/core/query_preprocessor.h"
#include "query_executor/query_executor.h"

bool SuchThatEvaluator::evaluate() {
  // Get all relevant variables so that further work with such_that can be
  // easily done
  // Get all relevant variables so that further work with such_that can be
  // easily done
  arg1AsSynonym = QueryExecutor::getSuchThatArgAsSynonym(arg1);
  arg2AsSynonym = QueryExecutor::getSuchThatArgAsSynonym(arg2);
  arg1IsUnderscore = QueryExecutor::isSuchThatArgUnderscore(arg1),
  arg2IsUnderscore = QueryExecutor::isSuchThatArgUnderscore(arg2);
  arg1AsBasic = QueryExecutor::getSuchThatArgAsBasic(arg1);
  arg2AsBasic = QueryExecutor::getSuchThatArgAsBasic(arg2);

  if (arg1AsSynonym) {
    // Add entire set of values for variable into the overall constraints
    QueryExecutor::addAllValuesForVariableToConstraints(
        query->declarations, pkb, arg1AsSynonym->synonym, qc);
  }
  if (arg2AsSynonym) {
    // Add entire set of values for variable into the overall constraints
    QueryExecutor::addAllValuesForVariableToConstraints(
        query->declarations, pkb, arg2AsSynonym->synonym, qc);
  }

  return dispatch();
}

//! Dispatch correct such that case to correct handler based on arg types
bool SuchThatEvaluator::dispatch() {
  if (arg1AsSynonym && arg2AsBasic) {
    // Case 1:  Follows*(s, 3)
    return dispatchLeftVarSynonymRightBasic();
  } else if (arg2AsSynonym && arg1AsBasic) {
    // Case 2:  Follows*(3, s)
    return dispatchRightVarSynonymLeftBasic();
  } else if (arg1AsSynonym && arg2IsUnderscore) {
    // Case 3: Follows*(s, _)
    return dispatchLeftVarSynonymRightUnderscore();
  } else if (arg2AsSynonym && arg1IsUnderscore) {
    // Case 4: Follows*(_, s)
    return dispatchRightVarSynonymLeftUnderscore();
  } else if (arg1AsSynonym && arg2AsSynonym) {
    // Case 5:  Follows*(s1, s2)
    return dispatchBothVarsSynonyms();
  } else if (arg1IsUnderscore && arg2IsUnderscore) {
    // Case 6: Follows*(_, _)
    return dispatchDoubleUnderscore();
  } else if (arg1AsBasic && arg2IsUnderscore) {
    // Case 7: Follows(3, _)
    return dispatchLeftBasicRightUnderscore();
  } else if (arg2AsBasic && arg1IsUnderscore) {
    // Case 8:  Follows(_, 3)
    return dispatchRightBasicLeftUnderscore();
  } else if (arg1AsBasic && arg2AsBasic) {
    // Case 9: Follows(2, 3)
    return handleBothVarsUnselected(*arg1AsBasic, *arg2AsBasic);
  } else {
    std::cout << "This case should not be triggered\n";
    assert(false);
  }
}

bool SuchThatEvaluator::dispatchLeftVarSynonymRightBasic() {
  auto results = handleLeftVarSynonymRightBasic(*arg2AsBasic);
  if (results.empty() ||
      qc.containsNoAllowedResults(results, arg1AsSynonym->synonym)) {
    return false;
  }
  qc.addToSingleVariableConstraints(arg1AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchRightVarSynonymLeftBasic() {
  auto results = handleRightVarSynonymLeftBasic(*arg1AsBasic);
  if (results.empty() ||
      qc.containsNoAllowedResults(results, arg2AsSynonym->synonym)) {
    return false;
  }
  qc.addToSingleVariableConstraints(arg2AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchLeftVarSynonymRightUnderscore() {
  auto lhs_designentities = QueryExecutor::getAllDesignEntityValuesByVarName(
      query->declarations, pkb, arg1AsSynonym->synonym);

  std::vector<std::string> results;
  for (auto de : lhs_designentities) {
    if (handleLeftVarSynonymRightUnderscore(de)) {
      results.push_back(de);
    }
  }
  if (results.empty()) return false;
  qc.addToSingleVariableConstraints(arg1AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchRightVarSynonymLeftUnderscore() {
  auto rhs_designentities = QueryExecutor::getAllDesignEntityValuesByVarName(
      query->declarations, pkb, arg2AsSynonym->synonym);
  std::vector<std::string> results;
  for (auto de : rhs_designentities) {
    if (handleRightVarSynonymLeftUnderscore(de)) {
      results.push_back(de);
    }
  }
  if (results.empty()) return false;
  qc.addToSingleVariableConstraints(arg2AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchBothVarsSynonyms() {
  auto lhs_designentities = QueryExecutor::getAllDesignEntityValuesByVarName(
      query->declarations, pkb, arg1AsSynonym->synonym);
  auto rhs_designentities = QueryExecutor::getAllDesignEntityValuesByVarName(
      query->declarations, pkb, arg2AsSynonym->synonym);

  PairedConstraintSet results;
  for (auto lhs_de : lhs_designentities) {
    for (auto rhs_de : rhs_designentities) {
      if (handleBothVarsSynonyms(lhs_de, rhs_de)) {
        results.insert({lhs_de, rhs_de});
      }
    }
  }
  if (results.empty()) return false;
  qc.addToPairedVariableConstraints(arg1AsSynonym->synonym,
                                    arg2AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchRightVarSelectedLeftVarUnselected() {
  return dispatchBothVarsSynonyms();
}

// No variable is selected
bool SuchThatEvaluator::dispatchDoubleUnderscore() {
  return handleDoubleUnderscore();
}

bool SuchThatEvaluator::dispatchBothVarsUnselected() {
  return dispatchBothVarsSynonyms();
}
bool SuchThatEvaluator::dispatchLeftVarUnselectedRightBasic() {
  return dispatchLeftVarSynonymRightBasic();
}

bool SuchThatEvaluator::dispatchRightVarUnselectedLeftBasic() {
  return dispatchRightVarSynonymLeftBasic();
}

bool SuchThatEvaluator::dispatchLeftBasicRightUnderscore() {
  return handleLeftBasicRightUnderscore(*arg1AsBasic);
}

bool SuchThatEvaluator::dispatchRightBasicLeftUnderscore() {
  return handleRightBasicLeftUnderscore(*arg2AsBasic);
}

bool SuchThatEvaluator::dispatchLeftVarUnselectedRightUnderscore() {
  return dispatchLeftVarSynonymRightUnderscore();
}

bool SuchThatEvaluator::dispatchRightVarUnselectedLeftUnderscore() {
  return dispatchRightVarSynonymLeftUnderscore();
}
