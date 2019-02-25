#include "query_manager/relations/SuchThatEvaluator.h"
#include <cassert>
#include <string>
#include <vector>
#include "query_manager/query_manager.h"

AllowedValuesPairOrBool SuchThatEvaluator::evaluate() {
  // Get all relevant variables so that further work with such_that can be
  // easily done
  // Get all relevant variables so that further work with such_that can be
  // easily done
  arg1AsSynonym = QueryManager::getSuchThatArgAsSynonym(arg1);
  arg2AsSynonym = QueryManager::getSuchThatArgAsSynonym(arg2);
  arg1InSelect = false;
  arg2InSelect = false;
  arg1IsUnderscore = QueryManager::isSuchThatArgUnderscore(arg1),
  arg2IsUnderscore = QueryManager::isSuchThatArgUnderscore(arg2);
  arg1AsBasic = QueryManager::getSuchThatArgAsBasic(arg1);
  arg2AsBasic = QueryManager::getSuchThatArgAsBasic(arg2);

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
AllowedValuesPairOrBool SuchThatEvaluator::dispatch() {
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
AllowedValuesPairOrBool SuchThatEvaluator::dispatchSuchThatSelected() {
  if (arg1AsSynonym && arg1InSelect && arg2AsBasic) {
    // Case 1: Selected variable is in this such_that, left argument
    // Follows*(s, 3)
    return handleLeftVarSelectedRightBasic();
  } else if (arg2AsSynonym && arg2InSelect && arg1AsBasic) {
    // Case 2: Selected variable is in this such_that, right argument
    // Follows*(3, s)
    return handleRightVarSelectedLeftBasic();
  } else if (arg1AsSynonym && arg1InSelect && arg2IsUnderscore) {
    // Case 3: Selected variable is in this such_that, left argument, right arg
    // underscore Follows*(s, _)
    // Need to find all selected things and then run the correct follows fx
    return handleLeftVarSelectedRightUnderscore();
  } else if (arg2AsSynonym && arg2InSelect && arg1IsUnderscore) {
    // Case 4: Selected variable is in this such_that, right argument, left arg
    // underscore Follows*(_, s)
    return handleRightVarSelectedLeftUnderscore();
  } else if (arg1AsSynonym && arg2AsSynonym && arg1InSelect) {
    // Case 5: Selected variable is in this such_that, left argument, right arg
    // also is a variable, Follows*(s, p)
    return handleLeftVarSelectedRightVarUnselected();
  } else if (arg1AsSynonym && arg2AsSynonym && arg2InSelect) {
    // Case 6: Selected variable is in this such_that, right argument, left arg
    // also is a variable, Follows*(p, s)
    return handleRightVarSelectedLeftVarUnselected();
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
AllowedValuesPairOrBool SuchThatEvaluator::dispatchSuchThatNotSelected() {
  if (arg1IsUnderscore && arg2IsUnderscore) {
    // Case 7: Follows*(_, _)
    // TODO: Need to check PKB for existence of any follows relationships
    return handleDoubleUnderscore();
  } else if (arg1AsSynonym && arg2AsSynonym) {
    // Case 8: Selected variable is NOT in this such_that, need to check for
    // truth/falsity overall
    return handleBothVarsUnselected();
  } else if (arg1AsSynonym && arg2AsBasic) {
    // Case 9: Selected variable is NOT in this such_that, left argument
    // Follows(s1, 3)
    return handleLeftVarUnselectedRightBasic();
  } else if (arg2AsSynonym && arg1AsBasic) {
    // Case 10: Selected variable is NOT in this such_that, right argument
    // Follows(3, s1)
    return handleRightVarUnselectedLeftBasic();
  } else if (arg1AsBasic && arg2IsUnderscore) {
    // Case 11: Selected variable is NOT in this such_that, left basic,
    // right underscore: Follows(3, _)
    return handleLeftBasicRightUnderscore();
  } else if (arg2AsBasic && arg1IsUnderscore) {
    // Case 12: Selected variable is NOT in this such_that, right basic,
    // left underscore: Follows(_, 3)
    return handleRightBasicLeftUnderscore();
  } else if (arg1AsSynonym && arg2IsUnderscore) {
    // Case 13: Selected variable is NOT in this such_that, left basic,
    // right underscore: Follows(s1, _)
    return handleLeftVarUnselectedRightUnderscore();
  } else if (arg1IsUnderscore && arg2AsSynonym) {
    // Case 14: Selected variable is NOT in this such_that, right basic,
    // left underscore: Follows(_, s1)
    return handleRightVarUnselectedLeftUnderscore();
  } else {
    std::cout << "This case should not be triggered\n";
    assert(false);
  }
}
