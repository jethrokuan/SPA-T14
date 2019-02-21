#include "query_manager/query_manager.h"
#include <cassert>
#include <string>
#include <vector>

using namespace QE;

//! \brief Sorts the vector of strings returned by the query system
//! This decouples the return order of PKB with the result to screen
std::vector<std::string> QueryManager::makeQuery(Query* query) {
  auto result = makeQueryUnsorted(query);
  std::sort(result.begin(), result.end());
  return result;
}

std::vector<std::string> QueryManager::makeQueryUnsorted(Query* query) {
  // If no such-that and pattern clauses - run just the select
  if (query->such_that == nullptr && query->pattern == nullptr) {
    // std::cout << "Such that and pattern does not exist - returning "
    //             "unconditional Select\n";

    // std::cout << "Running getSelect\n";
    return getSelect(query->selected_declaration->getDesignEntity());
  }

  // std::cout << "Such that or pattern exists\n";
  // If such-that + pattern is true/false answer, can immediately return the
  // select statement
  // The nesting is horrible here, hope to improve it
  if (query->such_that) {
    // Handle such_thats that return a simple boolean value
    if (isBooleanSuchThat(query->such_that)) {
      if (isBooleanSuchThatTrue(query->such_that)) {
        // TODO: This shouldn't be a return - pattern still matters
        return getSelect(query->selected_declaration->getDesignEntity());
      } else {
        // Empty result if our such_that is false - at least one clause is
        // false!
        return std::vector<std::string>();
      }
    } else {
      // This is a more complex such-that query, pass to individual handlers
      // We expect a vector of strings from each
      auto result = handleNonBooleanSuchThat(query);

      // If result is empty - entire clause is false - can return immediately
      // If not, need to filter Select with the return result
      // E.g. Select v ---> {"i", "j", "k"}, such that returns {"i"}
      // if (result.) }
      return result;
    }
  }
  // TODO: HANDLE PATTERN
  return std::vector<std::string>();
}

std::vector<std::string> QueryManager::getSelect(DesignEntity de) {
  // All possible return types from select all PKB calls are vector<string>
  // std::cout << "GetSelect: ";
  switch (de) {
    case DesignEntity::ASSIGN:
      // std::cout << "assign";
      return pkb->getAssignList();
      break;
    case DesignEntity::CALL:
      // Next iteration
      // return pkb->getCallList();
      // std::cout << "call";
      break;
    case DesignEntity::CONSTANT:
      // std::cout << "constant";
      return pkb->getConstantList();
      break;
    case DesignEntity::IF:
      // std::cout << "if";
      return pkb->getIfList();
      break;
    case DesignEntity::PRINT:
      // std::cout << "print";
      return pkb->getPrintList();
      break;
    case DesignEntity::PROCEDURE:
      // std::cout << "procedure: ";
      return pkb->getProcedureList();
      break;
    case DesignEntity::READ:
      // std::cout << "read";
      return pkb->getReadList();
      break;
    case DesignEntity::STMT:
      // std::cout << "stmt";
      return pkb->getStatementList();
      break;
    case DesignEntity::VARIABLE:
      // std::cout << "variable";
      return pkb->getVariableList();
      break;
    case DesignEntity::WHILE:
      // std::cout << "while";
      return pkb->getWhileList();
      break;
    default:
      break;
      // This should never happen - we should have handled all cases
      assert(false);
  }

  std::cout << std::endl;
  return std::vector<std::string>();
}

std::vector<std::string> QueryManager::handleNonBooleanSuchThat(Query* query) {
  // Get all relevant variables so that further work with such_that can be
  // easily done
  auto suchthat = query->such_that;
  auto arg1 = query->such_that->getFirstArg();
  auto arg2 = query->such_that->getSecondArg();
  auto arg1AsSynonym = getSuchThatArgAsSynonym(arg1);
  auto arg2AsSynonym = getSuchThatArgAsSynonym(arg2);
  bool arg1InSelect = false, arg2InSelect = false;
  bool arg1IsUnderscore = isSuchThatArgUnderscore(arg1),
       arg2IsUnderscore = isSuchThatArgUnderscore(arg2);
  auto arg1AsBasic = getSuchThatArgAsBasic(arg1),
       arg2AsBasic = getSuchThatArgAsBasic(arg2);

  if (arg1AsSynonym) {
    std::cout << "Arg 1: " << *arg1AsSynonym << "\n";
    arg1InSelect = query->selected_declaration->getSynonym() == arg1AsSynonym
                       ? true
                       : false;
  }
  if (arg2AsSynonym) {
    std::cout << "Arg 2: " << *arg2AsSynonym << "\n";
    arg2InSelect = query->selected_declaration->getSynonym() == arg2AsSynonym
                       ? true
                       : false;
  }

  if (arg1InSelect) {
    std::cout << "Arg 1 is selected\n";
  }
  if (arg2InSelect) {
    std::cout << "Arg 2 is selected\n";
  }

  if (arg1IsUnderscore) {
    std::cout << "Arg 1 is underscore\n";
  }
  if (arg2IsUnderscore) {
    std::cout << "Arg 2 is underscore\n";
  }

  if (arg1AsBasic) {
    std::cout << "Arg 1 is basic: " << *arg1AsBasic << "\n";
  }
  if (arg2AsBasic) {
    std::cout << "Arg 2 is basic: " << *arg2AsBasic << "\n";
  }

  // Pass each type of relation to its own independent handler
  switch (query->such_that->getRelation()) {
    case Relation::FollowsT:
      return handleFollowsTSuchThat(
          query, arg1AsSynonym, arg2AsSynonym, arg1InSelect, arg2InSelect,
          arg1IsUnderscore, arg2IsUnderscore, arg1AsBasic, arg2AsBasic);
      break;
    default:
      assert(false);
  }
}