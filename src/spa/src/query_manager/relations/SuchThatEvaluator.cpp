#include "query_manager/relations/SuchThatEvaluator.h"
#include <cassert>
#include <string>
#include <vector>
#include "query_manager/query_manager.h"

BoolOrStrings SuchThatEvaluator::evaluate() {
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

BoolOrStrings SuchThatEvaluator::dispatch() {
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
BoolOrStrings SuchThatEvaluator::dispatchSuchThatSelected() {
  if (arg1AsSynonym && arg1InSelect && arg2AsBasic) {
    // Case 1: Selected variable is in this such_that, left argument
    // Follows*(s, 3)
    // return pkb->getBeforeLineS(*arg2AsBasic);
    return handleLeftVarSelectedRightBasic();
  } else if (arg2AsSynonym && arg2InSelect && arg1AsBasic) {
    // Case 2: Selected variable is in this such_that, right argument
    // Follows*(3, s)
    // return pkb->getFollowingLineS(*arg1AsBasic);
    return handleRightVarSelectedLeftBasic();
  } else if (arg1AsSynonym && arg1InSelect && arg2IsUnderscore) {
    // Case 3: Selected variable is in this such_that, left argument, right arg
    // underscore Follows*(s, _)
    // Need to find all selected things and then run the correct follows fx
    /*
    auto all_selected_designentities =
        getSelect(query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (!pkb->getFollowingLineS(de).empty()) {
        results.push_back(de);
      }
    }
    return results;
    */
    return handleLeftVarSelectedRightUnderscore();
  } else if (arg2AsSynonym && arg2InSelect && arg1IsUnderscore) {
    // Case 4: Selected variable is in this such_that, right argument, left arg
    // underscore Follows*(_, s)
    /*
    auto all_selected_designentities =
        getSelect(query->selected_declaration->getDesignEntity());
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      if (!pkb->getBeforeLineS(de).empty()) {
        results.push_back(de);
      }
    }
    return results;
    */
    return handleRightVarSelectedLeftUnderscore();
  } else if (arg1AsSynonym && arg2AsSynonym && arg1InSelect) {
    // Case 5: Selected variable is in this such_that, left argument, right arg
    // also is a variable, Follows*(s, p)
    /*
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return std::vector<std::string>();
    }
    auto all_selected_designentities =
        getSelect(query->selected_declaration->getDesignEntity());
    auto right_arg_de = Declaration::findDeclarationForSynonym(
                            query->declarations, *arg2AsSynonym)
                            ->getDesignEntity();
    auto all_unselected_designentities = getSelect(right_arg_de);
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      for (auto unselect_de : all_unselected_designentities) {
        if (pkb->isLineFollowLineS(de, unselect_de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
    */
    return handleLeftVarSelectedRightVarUnselected();
  } else if (arg1AsSynonym && arg2AsSynonym && arg2InSelect) {
    // Case 6: Selected variable is in this such_that, right argument, left arg
    // also is a variable, Follows*(p, s)
    /*
    if (arg1AsSynonym == arg2AsSynonym) {
      // Cannot follow yourself
      return std::vector<std::string>();
    }
    auto all_selected_designentities =
        getSelect(query->selected_declaration->getDesignEntity());
    auto left_arg_de = Declaration::findDeclarationForSynonym(
                           query->declarations, *arg1AsSynonym)
                           ->getDesignEntity();
    auto all_unselected_designentities = getSelect(left_arg_de);
    std::vector<std::string> results;
    for (auto de : all_selected_designentities) {
      for (auto unselect_de : all_unselected_designentities) {
        if (pkb->isLineFollowLineS(unselect_de, de) &&
            std::find(results.begin(), results.end(), de) == results.end()) {
          results.push_back(de);
        }
      }
    }
    return results;
    */
    return handleRightVarSelectedLeftVarUnselected();
  } else {
    std::cout << "This case should not be triggered\n";
    assert(false);
  }
}
