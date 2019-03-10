#include "query_executor/query_executor.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_builder/core/query_preprocessor.h"
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
    auto result_set =
        getSelect(pkb, query->selected_declaration->getDesignEntity());
    return std::vector<std::string>(result_set.begin(), result_set.end());
  }

  QueryConstraints query_constraints;

  if (query->such_that) {
    // This is a more complex such-that query, pass to individual handlers
    // This call also modifies the query_constraints
    // So only need to check for no results
    if (!handleSuchThat(query, query_constraints)) {
      return std::vector<std::string>();
    }
  }

  // Evaluate pattern results if they exist
  if (query->pattern) {
    // Same reasoning as such-that
    if (!handlePattern(query, query_constraints)) {
      return std::vector<std::string>();
    }
  }

  // All clauses returned true and potentially added constraints
  // Have to evaluate constraints now

  // Since the Select variable's values will either be returned or constrained
  // add all possible values for it to take in at the start
  // Case: Select v such that Follows(1, 2) [Follows(1, 2) == true]
  auto select_var = query->selected_declaration->getSynonym().synonym;
  // Add entire set of values for variable into the overall constraints
  addAllValuesForVariableToConstraints(query->declarations, pkb, select_var,
                                       query_constraints);

  auto result = ConstraintSolver::constrainAndSelect(
      query_constraints, query->selected_declaration->getSynonym().synonym);

  return result;
}

std::unordered_set<std::string> QueryExecutor::getSelect(PKBManager* pkb,
                                                         DesignEntity de) {
  // All possible return types from select all PKB calls are vector<string>
  // std::cout << "GetSelect: ";
  switch (de) {
    case DesignEntity::ASSIGN:
      // std::cout << "assign";
      return pkb->getAssignSet();
      break;
    case DesignEntity::CALL:
      // Next iteration
      // return pkb->getCallList();
      // std::cout << "call";
      break;
    case DesignEntity::CONSTANT:
      // std::cout << "constant";
      return pkb->getConstantSet();
      break;
    case DesignEntity::IF:
      // std::cout << "if";
      return pkb->getIfSet();
      break;
    case DesignEntity::PRINT:
      // std::cout << "print";
      return pkb->getPrintSet();
      break;
    case DesignEntity::PROCEDURE:
      // std::cout << "procedure: ";
      return pkb->getProcedureSet();
      break;
    case DesignEntity::READ:
      // std::cout << "read";
      return pkb->getReadSet();
      break;
    case DesignEntity::STMT:
      // std::cout << "stmt";
      return pkb->getStatementSet();
      break;
    case DesignEntity::VARIABLE:
      // std::cout << "variable";
      return pkb->getVariableSet();
      break;
    case DesignEntity::WHILE:
      // std::cout << "while";
      return pkb->getWhileSet();
      break;
    default:
      break;
      // This should never happen - we should have handled all cases
      assert(false);
  }
  return {};
}

bool QueryExecutor::handleSuchThat(Query* query, QueryConstraints& qc) {
  switch (query->such_that->at(0)->getRelation()) {
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

void QueryExecutor::addAllValuesForVariableToConstraints(
    std::vector<Declaration>* declarations, PKBManager* pkb,
    const std::string& var_name, QueryConstraints& qc) {
  // For optimizations's sake: if we spot the variable already in the constraint
  // list - do not re-execute getSelect and re-constrain.
  // If the variable is already in the list, we can assume that this function
  // was already run.
  // Because for a variable to be in the constraint list, it must have been
  // either in a such-that clause or pattern clause (ignoring select).
  // If it was in either of those clauses, this function would have run.
  if (qc.isVarInAllPossibleValues(var_name)) return;

  auto all_de = getAllDesignEntityValuesByVarName(declarations, pkb, var_name);
  qc.addToAllPossibleValues(var_name, all_de);
}

std::unordered_set<std::string>
QueryExecutor::getAllDesignEntityValuesByVarName(
    std::vector<Declaration>* declarations, PKBManager* pkb,
    const std::string& var_name) {
  auto var_de = QueryPreprocessor::findDeclaration(declarations, var_name)
                    ->getDesignEntity();
  return QueryExecutor::getSelect(pkb, var_de);
}