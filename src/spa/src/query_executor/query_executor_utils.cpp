#include <cassert>
#include <string>
#include <vector>
#include "query_executor/query_executor.h"
#include "utils/utils.h"

using namespace QE;

std::optional<Synonym> QueryExecutor::getRefAsSynonym(Ref arg) {
  if (auto syn = std::get_if<Synonym>(&arg)) {
    return *syn;
  } else {
    return std::nullopt;
  }
}

bool QueryExecutor::isRefUnderscore(Ref arg) {
  return std::holds_alternative<Underscore>(arg);
}

//! Returns argument as a basic statementref or quoteident as string
std::optional<std::string> QueryExecutor::getRefAsBasic(Ref arg) {
  return std::visit(
      overload{
          [](Synonym&) -> std::optional<std::string> { return std::nullopt; },
          [](Underscore&) -> std::optional<std::string> {
            return std::nullopt;
          },
          [](StatementNumber& s) -> std::optional<std::string> {
            return std::to_string(s);
          },
          [](QuoteIdent& s) -> std::optional<std::string> {
            return s.quote_ident;
          }},
      arg);
}

std::vector<std::string> QueryExecutor::getSynonymsFromSelect(
    std::vector<ResultItem>* resultItems) {
  std::vector<std::string> result;
  for (const auto& resultItem : *resultItems) {
    result.push_back(resultItemToString(resultItem));
  }
  return result;
}

std::string QueryExecutor::resultItemToString(const ResultItem& resultItem) {
  if (auto syn = std::get_if<Synonym>(&resultItem)) {
    return syn->synonym;
  } else if (auto syn_attr = std::get_if<SynAttr>(&resultItem)) {
    return syn_attr->synonym.synonym;
  } else {
    // Only these two types are expected
    assert(false);
  }
}

std::vector<std::string> QueryExecutor::getNegativeResult(
    const ResultType& resultType) {
  switch (resultType) {
    case ResultType::TUPLE:
      return std::vector<std::string>();
    case ResultType::BOOLEAN:
      return std::vector<std::string>{"FALSE"};
    default:
      // No other result types should be possible
      assert(false);
  }
}

void QueryExecutor::addAllValuesForVariableToConstraints(
    std::vector<Declaration>* declarations, PKBManager* pkb,
    const std::string& var_name, ConstraintDatabase& db) {
  // For optimizations's sake: if we spot the variable already in the
  // constraint list - do not re-execute getSelect and re-constrain. If the
  // variable is already in the list, we can assume that this function was
  // already run. Because for a variable to be in the constraint list, it must
  // have been either in a such-that clause or pattern clause (ignoring
  // select). If it was in either of those clauses, this function would have
  // run.
  if (db.hasVariable(var_name)) return;

  auto all_de = getAllDesignEntityValuesByVarName(declarations, pkb, var_name);
  db.addToSingleVariableConstraints(var_name, all_de);
}

void QueryExecutor::addAllSelectedVarsToConstraints(Query* query,
                                                    ConstraintDatabase& db) {
  // No variables to constrain if this is a BOOLEAN query
  if (query->result->T == ResultType::BOOLEAN) return;
  // Otherwise add each variable's full set of values to constraints
  for (const ResultItem& select_var : *(query->result->selected_declarations)) {
    // Get the selected variable's string representation
    std::string select_var_str;
    if (auto syn = std::get_if<Synonym>(&select_var)) {
      select_var_str = syn->synonym;
    } else if (auto syn_attr = std::get_if<SynAttr>(&select_var)) {
      select_var_str = syn_attr->synonym.synonym;
    } else {
      // Only these two types are expected
      assert(false);
    }
    addAllValuesForVariableToConstraints(query->declarations, pkb,
                                         select_var_str, db);
  }
}

std::unordered_set<std::string>
QueryExecutor::getAllDesignEntityValuesByVarName(
    std::vector<Declaration>* declarations, PKBManager* pkb,
    const std::string& var_name) {
  auto var_de = Declaration::findDeclarationForString(declarations, var_name)
                    ->getDesignEntity();
  return QueryExecutor::getSelect(pkb, var_de);
}
