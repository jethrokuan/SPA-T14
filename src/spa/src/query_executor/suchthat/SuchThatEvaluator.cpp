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
  arg1InSelect = false;
  arg2InSelect = false;
  arg1IsUnderscore = QueryExecutor::isSuchThatArgUnderscore(arg1),
  arg2IsUnderscore = QueryExecutor::isSuchThatArgUnderscore(arg2);
  arg1AsBasic = QueryExecutor::getSuchThatArgAsBasic(arg1);
  arg2AsBasic = QueryExecutor::getSuchThatArgAsBasic(arg2);

  if (arg1AsSynonym) {
    arg1InSelect = query->selected_declaration->getSynonym() == arg1AsSynonym
                       ? true
                       : false;
    // Add entire set of values for variable into the overall constraints
    QueryExecutor::addAllValuesForVariableToConstraints(
        query->declarations, pkb, arg1AsSynonym->synonym, qc);
  }
  if (arg2AsSynonym) {
    arg2InSelect = query->selected_declaration->getSynonym() == arg2AsSynonym
                       ? true
                       : false;
    // Add entire set of values for variable into the overall constraints
    QueryExecutor::addAllValuesForVariableToConstraints(
        query->declarations, pkb, arg2AsSynonym->synonym, qc);
  }

  return dispatch();
}

//! Calls the correct branch based on whether vars are selected or not
bool SuchThatEvaluator::dispatch() {
  if (arg1InSelect || arg2InSelect) {
    // At least one synonym in such_that is selected
    return dispatchSuchThatSelected();
  } else {
    // Handle cases where nothing in such that is selected
    return dispatchSuchThatNotSelected();
  }
}

//! Handle 6 different cases where something is selected in the such_that
// View examples here
bool SuchThatEvaluator::dispatchSuchThatSelected() {
  if (arg1AsSynonym && arg1InSelect && arg2AsBasic) {
    // Case 1: Selected variable is in this such_that, left argument
    // Follows*(s, 3)
    return dispatchLeftVarSelectedRightBasic();
  } else if (arg2AsSynonym && arg2InSelect && arg1AsBasic) {
    // Case 2: Selected variable is in this such_that, right argument
    // Follows*(3, s)
    return dispatchRightVarSelectedLeftBasic();
  } else if (arg1AsSynonym && arg1InSelect && arg2IsUnderscore) {
    // Case 3: Selected variable is in this such_that, left argument, right arg
    // underscore Follows*(s, _)
    // Need to find all selected things and then run the correct follows fx
    return dispatchLeftVarSelectedRightUnderscore();
  } else if (arg2AsSynonym && arg2InSelect && arg1IsUnderscore) {
    // Case 4: Selected variable is in this such_that, right argument, left arg
    // underscore Follows*(_, s)
    return dispatchRightVarSelectedLeftUnderscore();
  } else if (arg1AsSynonym && arg2AsSynonym && arg1InSelect) {
    // Case 5: Selected variable is in this such_that, left argument, right arg
    // also is a variable, Follows*(s, p)
    return dispatchLeftVarSelectedRightVarUnselected();
  } else if (arg1AsSynonym && arg2AsSynonym && arg2InSelect) {
    // Case 6: Selected variable is in this such_that, right argument, left arg
    // also is a variable, Follows*(p, s)
    return dispatchRightVarSelectedLeftVarUnselected();
  } else {
    std::cout << "This case should not be triggered\n";
    assert(false);
  }
}

//! Handle 8 cases when nothing is selected in the such_that
// View examples here
bool SuchThatEvaluator::dispatchSuchThatNotSelected() {
  if (arg1IsUnderscore && arg2IsUnderscore) {
    // Case 7: Follows*(_, _)
    return dispatchDoubleUnderscore();
  } else if (arg1AsSynonym && arg2AsSynonym) {
    // Case 8: Selected variable is NOT in this such_that, need to check for
    // truth/falsity overall
    return dispatchBothVarsUnselected();
  } else if (arg1AsSynonym && arg2AsBasic) {
    // Case 9: Selected variable is NOT in this such_that, left argument
    // Follows(s1, 3)
    return dispatchLeftVarUnselectedRightBasic();
  } else if (arg2AsSynonym && arg1AsBasic) {
    // Case 10: Selected variable is NOT in this such_that, right argument
    // Follows(3, s1)
    return dispatchRightVarUnselectedLeftBasic();
  } else if (arg1AsBasic && arg2IsUnderscore) {
    // Case 11: Selected variable is NOT in this such_that, left basic,
    // right underscore: Follows(3, _)
    return dispatchLeftBasicRightUnderscore();
  } else if (arg2AsBasic && arg1IsUnderscore) {
    // Case 12: Selected variable is NOT in this such_that, right basic,
    // left underscore: Follows(_, 3)
    return dispatchRightBasicLeftUnderscore();
  } else if (arg1AsSynonym && arg2IsUnderscore) {
    // Case 13: Selected variable is NOT in this such_that, left basic,
    // right underscore: Follows(s1, _)
    return dispatchLeftVarUnselectedRightUnderscore();
  } else if (arg1IsUnderscore && arg2AsSynonym) {
    // Case 14: Selected variable is NOT in this such_that, right basic,
    // left underscore: Follows(_, s1)
    return dispatchRightVarUnselectedLeftUnderscore();
  } else if (arg1AsBasic && arg2AsBasic) {
    // Case 15: Both variables basic - boolean query
    // No dispatch necesary - simple case
    return handleBothVarsUnselected(*arg1AsBasic, *arg2AsBasic);
  } else {
    std::cout << "This case should not be triggered\n";
    assert(false);
  }
}
bool SuchThatEvaluator::dispatchLeftVarSelectedRightBasic() {
  auto results = handleLeftVarSelectedRightBasic(*arg2AsBasic);
  if (results.empty() ||
      qc.containsNoAllowedResults(results, arg1AsSynonym->synonym)) {
    return false;
  }
  qc.addToSingleVariableConstraints(arg1AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchRightVarSelectedLeftBasic() {
  auto results = handleRightVarSelectedLeftBasic(*arg1AsBasic);
  if (results.empty() ||
      qc.containsNoAllowedResults(results, arg2AsSynonym->synonym)) {
    return false;
  }
  qc.addToSingleVariableConstraints(arg2AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchLeftVarSelectedRightUnderscore() {
  auto lhs_designentities = QueryExecutor::getAllDesignEntityValuesByVarName(
      query->declarations, pkb, arg1AsSynonym->synonym);

  std::vector<std::string> results;
  for (auto de : lhs_designentities) {
    if (handleLeftVarSelectedRightUnderscore(de)) {
      results.push_back(de);
    }
  }
  if (results.empty()) return false;
  qc.addToSingleVariableConstraints(arg1AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchRightVarSelectedLeftUnderscore() {
  auto rhs_designentities = QueryExecutor::getAllDesignEntityValuesByVarName(
      query->declarations, pkb, arg2AsSynonym->synonym);
  std::vector<std::string> results;
  for (auto de : rhs_designentities) {
    if (handleRightVarSelectedLeftUnderscore(de)) {
      results.push_back(de);
    }
  }
  if (results.empty()) return false;
  qc.addToSingleVariableConstraints(arg2AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchLeftVarSelectedRightVarUnselected() {
  auto lhs_designentities = QueryExecutor::getAllDesignEntityValuesByVarName(
      query->declarations, pkb, arg1AsSynonym->synonym);
  auto rhs_designentities = QueryExecutor::getAllDesignEntityValuesByVarName(
      query->declarations, pkb, arg2AsSynonym->synonym);

  PairedConstraintSet results;
  for (auto lhs_de : lhs_designentities) {
    for (auto rhs_de : rhs_designentities) {
      if (handleLeftVarSelectedRightVarUnselected(lhs_de, rhs_de)) {
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
  return dispatchLeftVarSelectedRightVarUnselected();
}

// No variable is selected
bool SuchThatEvaluator::dispatchDoubleUnderscore() {
  return handleDoubleUnderscore();
}

bool SuchThatEvaluator::dispatchBothVarsUnselected() {
  return dispatchLeftVarSelectedRightVarUnselected();
}
bool SuchThatEvaluator::dispatchLeftVarUnselectedRightBasic() {
  return dispatchLeftVarSelectedRightBasic();
}

bool SuchThatEvaluator::dispatchRightVarUnselectedLeftBasic() {
  return dispatchRightVarSelectedLeftBasic();
}

bool SuchThatEvaluator::dispatchLeftBasicRightUnderscore() {
  return handleLeftBasicRightUnderscore(*arg1AsBasic);
}

bool SuchThatEvaluator::dispatchRightBasicLeftUnderscore() {
  return handleRightBasicLeftUnderscore(*arg2AsBasic);
}

bool SuchThatEvaluator::dispatchLeftVarUnselectedRightUnderscore() {
  return dispatchLeftVarSelectedRightUnderscore();
}

bool SuchThatEvaluator::dispatchRightVarUnselectedLeftUnderscore() {
  return dispatchRightVarSelectedLeftUnderscore();
}
