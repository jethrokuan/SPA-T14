#include "query_manager/query_manager.h"
#include <cassert>
#include <string>
#include <vector>
//#include "query_manager/relations/FollowsEvaluator.h"
#include "query_manager/relations/FollowsTEvaluator.h"
//#include "query_manager/relations/ModifiesSEvaluator.h"
//#include "query_manager/relations/ParentEvaluator.h"
//#include "query_manager/relations/ParentTEvaluator.h"
//#include "query_manager/relations/UsesSEvaluator.h"

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
    return getSelect(pkb, query->selected_declaration->getDesignEntity());
  }

  AllowedValuesPair such_that_result;
  AllowedValuesPair pattern_result;
  if (query->such_that) {
    // Handle such_thats that return a simple boolean value
    if (isBooleanSuchThat(query->such_that)) {
      if (isBooleanSuchThatTrue(query->such_that)) {
        // TODO: This shouldn't be a return - pattern still matters
        auto all_selected =
            getSelect(pkb, query->selected_declaration->getDesignEntity());
        auto selected_synonym = query->selected_declaration->getSynonym();
        such_that_result =
            ConstraintSolver::makeAllowedValues(selected_synonym, all_selected);
        ConstraintSolver::printAllowedValuesPair(such_that_result);
      } else {
        // Empty result if our such_that is false - at least one clause is
        // false!
        return std::vector<std::string>();
      }
    } else {
      // This is a more complex such-that query, pass to individual handlers
      // We expect a vector of strings from each
      auto result = handleNonBooleanSuchThat(query);
      if (auto bool_result = std::get_if<bool>(&result)) {
        std::cout << "Such That Complex Boolean Result: " << std::boolalpha
                  << *bool_result << "\n";
      } else if (auto constrain_result =
                     std::get_if<AllowedValuesPair>(&result)) {
        ConstraintSolver::printAllowedValuesPair(*constrain_result);
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

AllowedValuesPairOrBool QueryManager::handleNonBooleanSuchThat(Query* query) {
  switch (query->such_that->getRelation()) {
    case Relation::FollowsT:
      return FollowsTEvaluator(query, pkb).evaluate();
      /*
    case Relation::ParentT:
      return ParentTEvaluator(query, pkb).evaluate();
    case Relation::ModifiesS:
      return ModifiesSEvaluator(query, pkb).evaluate();
    case Relation::UsesS:
      return UsesSEvaluator(query, pkb).evaluate();
    case Relation::Follows:
      return FollowsEvaluator(query, pkb).evaluate();
    case Relation::Parent:
      return ParentEvaluator(query, pkb).evaluate();
      break;
      */
    default:
      assert(false);
  }
}