#include "query_manager/query_manager.h"
#include <cassert>
#include <string>
#include <vector>
#include "query_manager/relations/FollowsEvaluator.h"
#include "query_manager/relations/FollowsTEvaluator.h"
#include "query_manager/relations/ModifiesSEvaluator.h"
#include "query_manager/relations/ParentTEvaluator.h"
#include "query_manager/relations/UsesSEvaluator.h"

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
    return getSelect(pkb, query->selected_declaration->getDesignEntity());
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
        return getSelect(pkb, query->selected_declaration->getDesignEntity());
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
              getSelect(pkb, query->selected_declaration->getDesignEntity());
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
          return getSelect(pkb, query->selected_declaration->getDesignEntity());
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

std::vector<std::string> QueryManager::getSelect(PKBManager* pkb,
                                                 DesignEntity de) {
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

BoolOrStrings QueryManager::handleNonBooleanSuchThat(Query* query) {
  switch (query->such_that->getRelation()) {
    case Relation::FollowsT:
      return FollowsTEvaluator(query, pkb).evaluate();
    case Relation::ParentT:
      return ParentTEvaluator(query, pkb).evaluate();
    case Relation::ModifiesS:
      return ModifiesSEvaluator(query, pkb).evaluate();
    case Relation::UsesS:
      return UsesSEvaluator(query, pkb).evaluate();
    case Relation::Follows:
      return FollowsEvaluator(query, pkb).evaluate();
    case Relation::Parent:
      std::cout
          << "THIS RELATION IS NOT HANDLED YET - but not crashing to allow "
             "autotester to run\n";
      return false;
      break;
    default:
      assert(false);
  }
}