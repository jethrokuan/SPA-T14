#include "query_executor/suchthat/SuchThatEvaluator.h"
#include <cassert>
#include <string>
#include <vector>
// #include "query_executor/constraint_solver/constraint_solver.h"
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
  }
  if (arg2AsSynonym) {
    arg2InSelect = query->selected_declaration->getSynonym() == arg2AsSynonym
                       ? true
                       : false;
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
// e.g.
// Follows(s, 3), Follows(3, s),
// Follows(s, _), Follows(_, s)
// Follows(s, p), Follows(p, s)
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

//! Handle 6 cases when nothing is selected in the such_that
// e.g.
// Follows(_, _), Follows(s5, s6)
// Follows(s5, 3), Follows(3, s5)
// Follows(s5, _), Follows(_, s5)
bool SuchThatEvaluator::dispatchSuchThatNotSelected() {
  if (arg1IsUnderscore && arg2IsUnderscore) {
    // Case 7: Follows*(_, _)
    // TODO: Need to check PKB for existence of any follows relationships
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
  } else {
    std::cout << "This case should not be triggered\n";
    assert(false);
  }
}
bool SuchThatEvaluator::dispatchLeftVarSelectedRightBasic() {
  auto results = handleLeftVarSelectedRightBasic(*arg2AsBasic);
  if (results.empty()) return false;
  qc.addToSingleVariableConstraints(arg1AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchRightVarSelectedLeftBasic() {
  auto results = handleRightVarSelectedLeftBasic(*arg1AsBasic);
  if (results.empty()) return false;
  qc.addToSingleVariableConstraints(arg2AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchLeftVarSelectedRightUnderscore() {
  auto all_selected_designentities = QueryExecutor::getSelect(
      pkb, query->selected_declaration->getDesignEntity());
  std::vector<std::string> results;
  for (auto de : all_selected_designentities) {
    if (handleLeftVarSelectedRightUnderscore(de)) {
      results.push_back(de);
    }
  }
  qc.addToSingleVariableConstraints(arg1AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchRightVarSelectedLeftUnderscore() {
  auto all_selected_designentities = QueryExecutor::getSelect(
      pkb, query->selected_declaration->getDesignEntity());
  std::vector<std::string> results;
  for (auto de : all_selected_designentities) {
    if (handleRightVarSelectedLeftUnderscore(de)) {
      results.push_back(de);
    }
  }
  qc.addToSingleVariableConstraints(arg2AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchLeftVarSelectedRightVarUnselected() {
  if (arg1AsSynonym == arg2AsSynonym) {
    // Cannot follow/modify yourself
    // TODO: maybe this one and the one below sets the variable to nullset?
    return false;
  }

  auto all_selected_designentities = QueryExecutor::getSelect(
      pkb, query->selected_declaration->getDesignEntity());
  auto right_arg_de = Declaration::findDeclarationForSynonym(
                          query->declarations, *arg2AsSynonym)
                          ->getDesignEntity();
  auto all_unselected_designentities =
      QueryExecutor::getSelect(pkb, right_arg_de);
  PairedConstraintSet results;
  for (auto de : all_selected_designentities) {
    for (auto unselect_de : all_unselected_designentities) {
      if (handleLeftVarSelectedRightVarUnselected(de, unselect_de)) {
        results.insert({de, unselect_de});
      }
    }
  }
  qc.addToPairedVariableConstraints(arg1AsSynonym->synonym,
                                    arg2AsSynonym->synonym, results);
  return true;
}

bool SuchThatEvaluator::dispatchRightVarSelectedLeftVarUnselected() {
  if (arg1AsSynonym == arg2AsSynonym) {
    return false;
  }
  auto all_selected_designentities = QueryExecutor::getSelect(
      pkb, query->selected_declaration->getDesignEntity());
  auto left_arg_de = Declaration::findDeclarationForSynonym(query->declarations,
                                                            *arg1AsSynonym)
                         ->getDesignEntity();
  auto all_unselected_designentities =
      QueryExecutor::getSelect(pkb, left_arg_de);
  PairedConstraintSet results;
  for (auto de : all_selected_designentities) {
    for (auto unselect_de : all_unselected_designentities) {
      if (handleRightVarSelectedLeftVarUnselected(unselect_de, de)) {
        results.insert({unselect_de, de});
      }
    }
  }
  qc.addToPairedVariableConstraints(arg1AsSynonym->synonym,
                                    arg2AsSynonym->synonym, results);
  return true;
}

// No variable is selected
bool SuchThatEvaluator::dispatchDoubleUnderscore() {
  if (arg1AsSynonym == arg2AsSynonym) {
    // Cannot follow yourself
    return false;
  }
  return handleDoubleUnderscore();
}

bool SuchThatEvaluator::dispatchBothVarsUnselected() {
  if (arg1AsSynonym == arg2AsSynonym) {
    // Cannot follow yourself
    return false;
  }
  auto left_arg_de = Declaration::findDeclarationForSynonym(query->declarations,
                                                            *arg1AsSynonym)
                         ->getDesignEntity();
  auto right_arg_de = Declaration::findDeclarationForSynonym(
                          query->declarations, *arg1AsSynonym)
                          ->getDesignEntity();

  auto all_left_designentities = QueryExecutor::getSelect(pkb, left_arg_de);
  auto all_right_designentities = QueryExecutor::getSelect(pkb, right_arg_de);
  PairedConstraintSet results;
  for (auto left_de : all_left_designentities) {
    for (auto right_de : all_right_designentities) {
      // Any satisfied relation would mean this clause is true overall
      if (handleBothVarsUnselected(left_de, right_de)) {
        results.insert({left_de, right_de});
      }
    }
  }
  qc.addToPairedVariableConstraints(arg1AsSynonym->synonym,
                                    arg2AsSynonym->synonym, results);
  return true;
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
