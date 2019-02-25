#include "query_manager/query_manager.h"
#include <cassert>
#include <string>
#include <vector>
#include "query_manager/relations/FollowsEvaluator.h"
#include "query_manager/relations/FollowsTEvaluator.h"
#include "query_manager/relations/ModifiesSEvaluator.h"
#include "query_manager/relations/ParentEvaluator.h"
#include "query_manager/relations/ParentTEvaluator.h"
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

  // If either clause doesn't exist, don't constrain
  AllowedValuesPairOrBool such_that_result = {true};
  AllowedValuesPairOrBool pattern_result = {true};

  if (query->such_that) {
    // Handle such_thats that return a simple boolean value
    if (isBooleanSuchThat(query->such_that)) {
      such_that_result = isBooleanSuchThatTrue(query->such_that);
    } else {
      // This is a more complex such-that query, pass to individual handlers
      such_that_result = handleNonBooleanSuchThat(query);
    }
  }

  // Check if an early return is necessary
  if (auto bool_result = std::get_if<bool>(&such_that_result)) {
    if (!*bool_result) {
      return std::vector<std::string>();
    } else {
      // TODO: FOR PATTERN If true, do nothing til later
      return getSelect(pkb, query->selected_declaration->getDesignEntity());
    }
  } else if (auto constrain_result =
                 std::get_if<AllowedValuesPair>(&such_that_result)) {
    // This only works now - check for empty allowed list and return immediately
    // if so. Works because a constraint list indicates at least one variable
    // was selected
    if (constrain_result->second.empty()) {
      return std::vector<std::string>();
    }
  }

  // Evaluate pattern results if they exist

  // Evaluate final results (TODO: pattern - now just use such that)
  if (auto such_that_constraint =
          std::get_if<AllowedValuesPair>(&such_that_result)) {
    auto select_allowed =
        getSelect(pkb, query->selected_declaration->getDesignEntity());
    auto select_synonym = query->selected_declaration->getSynonym();
    auto select_constraint =
        ConstraintSolver::makeAllowedValues(select_synonym, select_allowed);
    std::cout << "All Select Values: \n";
    ConstraintSolver::printAllowedValuesPair(select_constraint);
    std::cout << "All Such-That Constrained Values: \n";
    ConstraintSolver::printAllowedValuesPair(*such_that_constraint);
    return ConstraintSolver::constrainAndSelect(
        {select_constraint, *such_that_constraint},
        query->selected_declaration->getSynonym().synonym);
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
    case Relation::Follows:
      return FollowsEvaluator(query, pkb).evaluate();
    case Relation::Parent:
      return ParentEvaluator(query, pkb).evaluate();
    case Relation::ParentT:
      return ParentTEvaluator(query, pkb).evaluate();
      break;
    case Relation::ModifiesS:
      return ModifiesSEvaluator(query, pkb).evaluate();
      /*
    case Relation::UsesS:
      return UsesSEvaluator(query, pkb).evaluate();
      */
    default:
      // assert(false);
      return false;
  }
}