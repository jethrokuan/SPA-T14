#include "query_manager/query_manager.h"
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "query_manager/relations/FollowsEvaluator.h"
#include "query_manager/relations/FollowsTEvaluator.h"
#include "query_manager/relations/ModifiesSEvaluator.h"
#include "query_manager/relations/ParentEvaluator.h"
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
    return getSelect(pkb, query->selected_declaration->getDesignEntity());
  }

  // If either clause doesn't exist, don't constrain
  AllowedValuesPairOrBool such_that_result = {true};
  AllowedValuesPairOrBool pattern_result = {true};
  AllowedValuesList allowed_values;

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
  if (auto such_that_bool_result = std::get_if<bool>(&such_that_result)) {
    if (!*such_that_bool_result) {
      return std::vector<std::string>();
    }  // Nothing to do if true - handle later
  } else if (auto such_that_constrain_result =
                 std::get_if<AllowedValuesPair>(&such_that_result)) {
    // This only works now - check for empty allowed list and return immediately
    // if so. Works because a constraint list indicates at least one variable
    // was selected - else it would be a `false` value
    if (such_that_constrain_result->second.empty()) {
      return std::vector<std::string>();
    }
    // Add it to constraints
    allowed_values.push_back(*such_that_constrain_result);
  }

  // Evaluate pattern results if they exist
  if (query->pattern) {
    pattern_result = handlePattern(query);
    allowed_values.push_back(std::get<AllowedValuesPair>(pattern_result));
  }

  // Add select to the list of constraints
  auto select_allowed =
      getSelect(pkb, query->selected_declaration->getDesignEntity());
  auto select_synonym = query->selected_declaration->getSynonym();
  auto select_constraint =
      ConstraintSolver::makeAllowedValues(select_synonym, select_allowed);
  allowed_values.push_back(select_constraint);

  return ConstraintSolver::constrainAndSelect(
      allowed_values, query->selected_declaration->getSynonym().synonym);
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
    case Relation::UsesS:
      return UsesSEvaluator(query, pkb).evaluate();
    default:
      assert(false);
  }
}

AllowedValuesPairOrBool QueryManager::handlePattern(Query* query) {
  // TODO: This doesn't handle cases with LHS of pattern being a synonyn
  // Need a PKB interface for it
  auto pattern = query->pattern;
  auto pattern_syn = pattern->getSynonym();
  auto pattern_lhs = pattern->getFirstArg();
  auto pattern_rhs = pattern->getSecondArg();
  // pattern-cl : ‘pattern’ syn-assign ‘(‘ entRef ‘,’ expression-spec ’)’
  // entRef : synonym | ‘_’ | ‘"’ IDENT ‘"’
  // expression-spec : ‘"‘ expr’"’ | ‘_’ ‘"’ expr ‘"’ ‘_’ | ‘_’ [BASIC-SPA]
  // expression-spec: ‘_’ ‘"’ factor ‘"’ ‘_’ | ‘_’ [ITER1]
  // factor: var_name | const_value [ITER1]

  // Clearly: 6 possible cases (cross product of entRef and expression-spec)
  // Match on entRef
  if (std::get_if<Underscore>(&pattern_lhs)) {
    return handlePatternLHSUnderscore(query, pattern_syn, pattern_rhs);
  } else if (std::get_if<QuoteIdent>(&pattern_lhs)) {
    return handlePatternLHSQuoteIdent(query, pattern_syn,
                                      entRefToString(pattern_lhs), pattern_rhs);
  } else if (auto lhs_syn = std::get_if<Synonym>(&pattern_lhs)) {
    return handlePatternLHSSynonym(query, pattern_syn, *lhs_syn, pattern_rhs);
  } else {
    assert(false);
  }
}

AllowedValuesPairOrBool QueryManager::handlePatternLHSUnderscore(
    Query* query, Synonym& syn, ExpressionSpec& pattern_rhs) {
  // pattern a (_, <...>)
  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a (_, _) --> all assignments
    auto all_assigns = getSelect(pkb, DesignEntity::ASSIGN);
    return ConstraintSolver::makeAllowedValues(syn, all_assigns);
  } else if (auto duf = std::get_if<DoubleUnderscoreFactor>(&pattern_rhs)) {
    // pattern a (_, _"x + y"_) --> partial match, no var constraint
    std::ostringstream stream;
    stream << *duf;
    auto matching_assigns = pkb->getPartialMatchLines(stream.str())
                                .value_or(std::vector<std::string>());

    return ConstraintSolver::makeAllowedValues(syn, matching_assigns);
  } else {
    // nothing else allowed for now
    assert(false);
  }
}

AllowedValuesPairOrBool QueryManager::handlePatternLHSQuoteIdent(
    Query* query, Synonym& syn, std::string lhs, ExpressionSpec& pattern_rhs) {
  // pattern a ("x", <...>)
  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a ("x", _) --> all assignments with LHS "x"
    auto allowed_lines =
        pkb->getLineForAssignVar(lhs).value_or(std::vector<std::string>());
    return ConstraintSolver::makeAllowedValues(syn, allowed_lines);
  } else if (auto duf = std::get_if<DoubleUnderscoreFactor>(&pattern_rhs)) {
    // pattern a ("x", _"x + y"_) --> partial match, no var constraint
    std::ostringstream rhs_partial;
    rhs_partial << *duf;
    auto matching_assigns =
        pkb->getPartialMatchLinesWithVar(lhs, rhs_partial.str())
            .value_or(std::vector<std::string>());
    std::cout << "Running this case\n";
    return ConstraintSolver::makeAllowedValues(syn, matching_assigns);
  } else {
    // nothing else allowed for now
    assert(false);
  }
}

AllowedValuesPairOrBool QueryManager::handlePatternLHSSynonym(
    Query* query, Synonym& syn, Synonym& lhs, ExpressionSpec& pattern_rhs) {
  // pattern a (v, <...>)
  // Get all variables so that we can make this query
  auto all_vars = getSelect(pkb, DesignEntity::VARIABLE);

  if (std::get_if<Underscore>(&pattern_rhs)) {
    // pattern a (v, _) --> all assignments
    auto allowed_values = pkb->getAllPatternLinesAndVars();
    AllowedValueSet avs(allowed_values.begin(), allowed_values.end());
    return ConstraintSolver::makeAllowedValues(syn, lhs, avs);
    // assert(false);
  } else if (auto duf = std::get_if<DoubleUnderscoreFactor>(&pattern_rhs)) {
    // pattern a (v, _"x + y"_) --> partial match, no var constraint
    std::ostringstream rhs_partial;
    rhs_partial << *duf;

    // Constrain (a,v) together
    auto allowed_values = pkb->getPartialMatchLinesAndVars(rhs_partial.str())
                              .value_or(std::vector<SynonymPair>());

    AllowedValueSet avs(allowed_values.begin(), allowed_values.end());
    return ConstraintSolver::makeAllowedValues(syn, lhs, avs);
  } else {
    // nothing else allowed for now
    assert(false);
  }
}