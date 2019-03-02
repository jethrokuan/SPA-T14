#include "query_executor/query_executor.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_executor/constraint_solver/query_constraints.h"
// #include "query_executor/pattern/PatternEvaluator.h"
//#include "query_executor/suchthat/FollowsEvaluator.h"
#include "query_executor/suchthat/FollowsTEvaluator.h"
//#include "query_executor/suchthat/ModifiesSEvaluator.h"
//#include "query_executor/suchthat/ParentEvaluator.h"
//#include "query_executor/suchthat/ParentTEvaluator.h"
//#include "query_executor/suchthat/UsesSEvaluator.h"

using namespace QE;

//! \brief Sorts the vector of strings returned by the query system
//! This decouples the return order of PKB with the result to screen
std::vector<std::string> QueryExecutor::makeQuery(Query* query) {
  auto result = makeQueryUnsorted(query);
  std::sort(result.begin(), result.end());
  return result;
}

std::vector<std::string> QueryExecutor::makeQueryUnsorted(Query* query) {
  // If no such-that and pattern clauses - run just the select
  if (query->such_that == nullptr && query->pattern == nullptr) {
    return getSelect(pkb, query->selected_declaration->getDesignEntity());
  }

  // If either clause doesn't exist, don't constrain
  // AllowedValuesPairOrBool such_that_result = {true};
  // AllowedValuesPairOrBool pattern_result = {true};
  // AllowedValuesList allowed_values;

  QueryConstraints query_constraints;

  if (query->such_that) {
    // Handle such_thats that return a simple boolean value
    if (isBooleanSuchThat(query->such_that)) {
      if (!isBooleanSuchThatTrue(query->such_that)) {
        return std::vector<std::string>();
      }
    } else {
      // This is a more complex such-that query, pass to individual handlers
      handleNonBooleanSuchThat(query, query_constraints);
    }
  }

  /*
    // Check if an early return is necessary
    if (auto such_that_bool_result = std::get_if<bool>(&such_that_result)) {
      if (!*such_that_bool_result) {
        return std::vector<std::string>();
      }  // Nothing to do if true - handle later
    } else if (auto such_that_constrain_result =
                   std::get_if<TupledConstraint>(&such_that_result)) {
      // This only works now - check for empty allowed list and return
    immediately
      // if so. Works because a constraint list indicates at least one variable
      // was selected - else it would be a `false` value
      if (such_that_constrain_result->second.empty()) {
        return std::vector<std::string>();
      }
      // Add it to constraints
      //allowed_values.push_back(*such_that_constrain_result);
    }
    */

  // Evaluate pattern results if they exist
  if (query->pattern) {
    // handlePattern(query, query_constraints);
    // pattern_result = handlePattern(query, query_constraints);
    // allowed_values.push_back(std::get<TupledConstraint>(pattern_result));
  }

  std::cout << "Final Query Constraints: \n";
  std::cout << query_constraints;

  // Add select to the list of constraints
  /*
  auto select_allowed =
      getSelect(pkb, query->selected_declaration->getDesignEntity());
  auto select_synonym = query->selected_declaration->getSynonym();
  auto select_constraint =
      ConstraintSolver::makeAllowedValues(select_synonym, select_allowed);
  allowed_values.push_back(select_constraint);

  return ConstraintSolver::constrainAndSelect(
      allowed_values, query->selected_declaration->getSynonym().synonym);
      */
  return std::vector<std::string>();
}

std::vector<std::string> QueryExecutor::getSelect(PKBManager* pkb,
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

bool QueryExecutor::handleNonBooleanSuchThat(Query* query,
                                             QueryConstraints& qc) {
  switch (query->such_that->getRelation()) {
    case Relation::FollowsT:
      return FollowsTEvaluator(query, pkb, qc).evaluate();
      /*
    case Relation::Follows:
      return FollowsEvaluator(query, pkb, qc).evaluate();
    case Relation::Parent:
      return ParentEvaluator(query, pkb, qc).evaluate();
    case Relation::ParentT:
      return ParentTEvaluator(query, pkb, qc).evaluate();
      break;
    case Relation::ModifiesS:
      return ModifiesSEvaluator(query, pkb, qc).evaluate();
    case Relation::UsesS:
      return UsesSEvaluator(query, pkb, qc).evaluate();
      */
    default:
      assert(false);
  }
}

bool QueryExecutor::handlePattern(Query* query, QueryConstraints& qc) {
  return true;
  // return PatternEvaluator(query, pkb, qc).evaluate();
}