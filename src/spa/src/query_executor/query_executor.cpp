#include "query_executor/query_executor.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_executor/constraint_solver/query_constraints.h"
#include "query_executor/pattern/PatternEvaluator.h"
#include "query_executor/suchthat/FollowsEvaluator.h"
#include "query_executor/suchthat/FollowsTEvaluator.h"
#include "query_executor/suchthat/ModifiesSEvaluator.h"
#include "query_executor/suchthat/ParentEvaluator.h"
#include "query_executor/suchthat/ParentTEvaluator.h"
#include "query_executor/suchthat/UsesSEvaluator.h"

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

  QueryConstraints query_constraints;

  if (query->such_that) {
    // Handle such_thats that return a simple boolean value
    if (isBooleanSuchThat(query->such_that)) {
      if (!isBooleanSuchThatTrue(query->such_that)) {
        return std::vector<std::string>();
      }
    } else {
      // This is a more complex such-that query, pass to individual handlers
      if (!handleNonBooleanSuchThat(query, query_constraints)) {
        return std::vector<std::string>();
      }
    }
  }

  // Evaluate pattern results if they exist
  if (query->pattern) {
    handlePattern(query, query_constraints);
  }

  // Add the SeleAct clause - seems to be required (TODO: CHECK THIS)
  auto select_var = query->selected_declaration->getSynonym().synonym;
  auto select_values =
      getSelect(pkb, query->selected_declaration->getDesignEntity());
  query_constraints.addToSingleVariableConstraints(select_var, select_values);

  auto result = ConstraintSolver::constrainAndSelect(
      query_constraints, query->selected_declaration->getSynonym().synonym);

  return result;
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
    case Relation::ModifiesS:
      return ModifiesSEvaluator(query, pkb, qc).evaluate();
    case Relation::UsesS:
      return UsesSEvaluator(query, pkb, qc).evaluate();
    case Relation::ParentT:
      return ParentTEvaluator(query, pkb, qc).evaluate();
    case Relation::Follows:
      return FollowsEvaluator(query, pkb, qc).evaluate();
    case Relation::Parent:
      return ParentEvaluator(query, pkb, qc).evaluate();
    default:
      assert(false);
  }
}

bool QueryExecutor::handlePattern(Query* query, QueryConstraints& qc) {
  return PatternEvaluator(query, pkb, qc).evaluate();
}