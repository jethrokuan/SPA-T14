#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_executor/query_executor.h"

#include "query_executor/clause_prioritizer.h"
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
#include "utils/utils.h"

using namespace QE;

std::vector<std::string> QueryExecutor::makeQuery(Query* query) {
  ConstraintDatabase db;

  std::vector<Clause> clauses = ClausePrioritizer(query).getClauses();

  // Execute each clause on the PKB
  for (auto clause : clauses) {
    if (!executeClause(query->declarations, clause, db)) {
      return getNegativeResult(query->result->T);
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
  return selectFromDB(query->declarations, query->result, db);
}

bool QueryExecutor::executeClause(std::vector<QE::Declaration>* decls,
                                  Clause clause, ConstraintDatabase& db) {
  return std::visit([&](auto& cl) { return executeClause(decls, cl, db); },
                    clause);
}

bool QueryExecutor::executeClause(std::vector<QE::Declaration>* decls,
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

bool QueryExecutor::executeClause(std::vector<QE::Declaration>* decls,
                                  QE::PatternCond* pattern,
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

bool QueryExecutor::executeClause(std::vector<QE::Declaration>* decls,
                                  QE::WithCond* withcond,
                                  ConstraintDatabase& db) {
  return WithEvaluator(decls, withcond, pkb, db).evaluate();
}

//! Asks the DB for the result type corresponding to the Select clause
std::vector<std::string> QueryExecutor::selectFromDB(
    std::vector<QE::Declaration>* decls, Result* result,
    ConstraintDatabase& db) {
  if (result->T == ResultType::TUPLE) {
    // Get vector of vector of results - one for each selected var
    auto results =
        db.selectMultiple(getSynonymsFromSelect(result->selected_declarations));

    // Get all synattrs mapped to their design entities for later lookup in loop
    auto synonym_de_map =
        getSynoynmToDesignEntityTypeMap(decls, result->selected_declarations);

    // Apply the appropriate function over the return result columns
    // E.g. If c.procName is a columns, do .procName on that col's values
    applyAttributesToResults(result->selected_declarations, results,
                             synonym_de_map);
    // Formats results by adding a space between them
    return joinResults(results);
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
