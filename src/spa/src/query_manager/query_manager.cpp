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
      if (auto result_vec = std::get_if<std::vector<std::string>>(&result)) {
        // Find set intersection between these two vectors
        if (result_vec->empty()) {
          // Clause returned no viable matches - return nothing
          return std::vector<std::string>();
        } else {
          auto final_result = std::vector<std::string>();
          auto select_statement_results =
              getSelect(query->selected_declaration->getDesignEntity());
          std::sort(select_statement_results.begin(),
                    select_statement_results.end());
          std::sort(result_vec->begin(), result_vec->end());
          // Calcualate intersection between select and such that caluse
          std::set_intersection(select_statement_results.begin(),
                                select_statement_results.end(),
                                result_vec->begin(), result_vec->end(),
                                std::back_inserter(final_result));
          // TODO: Have to change this for pattern
          return final_result;
        }
      } else if (auto clause_is_true = std::get_if<bool>(&result)) {
        if (*clause_is_true) {
          // If clause only returns true - can just return all selected stuff
          // TODO: change for pattern
          return getSelect(query->selected_declaration->getDesignEntity());
        } else {
          // Clause is false, immediate break and return nothig
          return std::vector<std::string>();
        }
      }
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

std::variant<bool, std::vector<std::string>>
QueryManager::handleNonBooleanSuchThat(Query* query) {
  // Get all relevant variables so that further work with such_that can be
  // easily done
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
    arg1InSelect = query->selected_declaration->getSynonym() == arg1AsSynonym
                       ? true
                       : false;
  }
  if (arg2AsSynonym) {
    arg2InSelect = query->selected_declaration->getSynonym() == arg2AsSynonym
                       ? true
                       : false;
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