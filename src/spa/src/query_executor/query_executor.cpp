#include "query_executor/query_executor.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#include "query_executor/constraint_solver/constraint_database.h"
#include "query_executor/pattern/AssignPatternEvaluator.h"
#include "query_executor/pattern/IfPatternEvaluator.h"
#include "query_executor/pattern/WhilePatternEvaluator.h"
#include "query_executor/suchthat/CallsEvaluator.h"
#include "query_executor/suchthat/CallsTEvaluator.h"
#include "query_executor/suchthat/FollowsEvaluator.h"
#include "query_executor/suchthat/FollowsTEvaluator.h"
#include "query_executor/suchthat/ModifiesPEvaluator.h"
#include "query_executor/suchthat/ModifiesSEvaluator.h"
#include "query_executor/suchthat/NextEvaluator.h"
#include "query_executor/suchthat/NextTEvaluator.h"
#include "query_executor/suchthat/ParentEvaluator.h"
#include "query_executor/suchthat/ParentTEvaluator.h"
#include "query_executor/suchthat/UsesPEvaluator.h"
#include "query_executor/suchthat/UsesSEvaluator.h"
#include "query_executor/with/WithEvaluator.h"

using namespace QE;

std::vector<std::string> QueryExecutor::makeQuery(Query* query) {
  auto result = makeQueryUnsorted(query);
  std::sort(result.begin(), result.end());
  return result;
}

std::vector<std::string> QueryExecutor::makeQueryUnsorted(Query* query) {
  ConstraintDatabase db;

  // Executes each such-that clause one by one
  if (!query->rel_cond->empty()) {
    for (auto& rel_cond : *(query->rel_cond)) {
      if (!handleSuchThat(query->declarations, rel_cond, db)) {
        return getNegativeResult(query->result->T);
      }
    }
  }

  // Executes each pattern clause one by one
  if (!query->patternb->empty()) {
    for (auto& pattern : *(query->patternb)) {
      if (!handlePattern(query->declarations, pattern, db)) {
        return getNegativeResult(query->result->T);
      }
    }
  }

  // Executes each pattern clause one by one
  if (!query->with_cond->empty()) {
    for (auto& with_cond : *(query->with_cond)) {
      if (!handleWithCond(query->declarations, with_cond, db)) {
        return getNegativeResult(query->result->T);
      }
    }
  }

  // All clauses returned true and potentially added constraints
  // Have to evaluate constraints now

  // Since the Select variable's values will either be returned or constrained
  // add all possible values for it to take in at the start
  // Case: Select v such that Follows(1, 2) [Follows(1, 2) == true]
  // Do this for all selected variables
  addAllSelectedVarsToConstraints(query, db);

  // Runs the correct constraint solver method (BOOLEAN vs non-BOOLEAN)
  return selectFromDB(query->result, db);
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
      return pkb->getCallSet();
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
    case DesignEntity::PROG_LINE:
      // std::cout << "prog_line: ";
      return pkb->getStatementSet();
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
      // This should never happen - we should have handled all cases
      assert(false);
  }
  return {};
}

bool QueryExecutor::handleSuchThat(std::vector<QE::Declaration>* decls,
                                   QE::RelCond* relCond,
                                   ConstraintDatabase& db) {
  switch (relCond->relation) {
    case Relation::FollowsT:
      return FollowsTEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::ModifiesS:
      return ModifiesSEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::UsesS:
      return UsesSEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::ParentT:
      return ParentTEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::Follows:
      return FollowsEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::Parent:
      return ParentEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::Next:
      return NextEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::NextT:
      return NextTEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::Calls:
      return CallsEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::CallsT:
      return CallsTEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::ModifiesP:
      return ModifiesPEvaluator(decls, relCond, pkb, db).evaluate();
    case Relation::UsesP:
      return UsesPEvaluator(decls, relCond, pkb, db).evaluate();
    default:
      assert(false);
  }
}

bool QueryExecutor::handlePattern(std::vector<QE::Declaration>* decls,
                                  QE::PatternB* pattern,
                                  ConstraintDatabase& db) {
  auto pattern_syn = pattern->getSynonym();
  auto pattern_de = Declaration::findDeclarationForSynonym(decls, pattern_syn)
                        ->getDesignEntity();
  switch (pattern_de) {
    case DesignEntity::ASSIGN:
      return AssignPatternEvaluator(decls, pattern, pkb, db).evaluate();
    case DesignEntity::IF:
      return IfPatternEvaluator(decls, pattern, pkb, db).evaluate();
    case DesignEntity::WHILE:
      return WhilePatternEvaluator(decls, pattern, pkb, db).evaluate();
    default:
      std::cout << "No design entity matches for pattern!\n";
      assert(false);
  }
}

bool QueryExecutor::handleWithCond(std::vector<QE::Declaration>* decls,
                                   QE::WithCond* withcond,
                                   ConstraintDatabase& db) {
  return WithEvaluator(decls, withcond, pkb, db).evaluate();
}

//! Asks the DB for the result type corresponding to the Select clause
std::vector<std::string> QueryExecutor::selectFromDB(Result* result,
                                                     ConstraintDatabase& db) {
  if (result->T == ResultType::TUPLE) {
    // Get vector of vector of results - one for each selected var
    return db.selectMultiple(
        getSynonymsFromSelect(result->selected_declarations));
  } else if (result->T == ResultType::BOOLEAN) {
    // Check truth/falsity instead of getting a vector of result values
    if (db.selectBoolean()) {
      return std::vector<std::string>{"TRUE"};
    } else {
      return std::vector<std::string>{"FALSE"};
    }
  } else {
    // No other result types allowed
    assert(false);
  }
}
