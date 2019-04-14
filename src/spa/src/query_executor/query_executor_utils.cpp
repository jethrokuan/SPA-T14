#include <cassert>
#include <string>
#include <vector>
#include "query_executor/query_executor.h"
#include "utils/utils.h"

using namespace QE;

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
  // if (db.hasVariable(var_name)) return;

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

std::string QueryExecutor::applyAttrToDesignEntityValue(
    PKBManager* pkb, const DesignEntity& de_type, const std::string& de,
    const AttrName& attrName) {
  // Only take this seriously  if we're asking for proc name or var name
  // Rest are no-ops for sure

  if (attrName == AttrName::PROC_NAME || attrName == AttrName::VAR_NAME) {
    return applyAttrToDesignEntityValue(pkb, de_type, de);
  } else {
    return de;
  }
}

std::string QueryExecutor::applyAttrToDesignEntityValue(
    PKBManager* pkb, const DesignEntity& de_type, const std::string& de) {
  // This function does not check for the attribute name since there is no
  // ambiguity - should be of type NAME, so only one possibility

  switch (de_type) {
    case DesignEntity::CALL:
      return pkb->getProcedureCalleeFromLine(de).value();
    case DesignEntity::READ:
      return pkb->getReadVariableFromLine(de).value();
    case DesignEntity::PRINT:
      return pkb->getPrintVariableFromLine(de).value();
    case DesignEntity::VARIABLE:
      // no-op
      return de;
    case DesignEntity::PROCEDURE:
      // no-op
      return de;
    default:
      std::cerr
          << "Received unexpected design entity type to apply attrName to\n";
      assert(false);
  }
}

unordered_map<std::string, DesignEntity>
QueryExecutor::getSynoynmToDesignEntityTypeMap(
    std::vector<Declaration>* decls,
    std::vector<QE::ResultItem>* selected_declarations) {
  unordered_map<std::string, DesignEntity> synattr_de_map;
  for (auto& selected : *selected_declarations) {
    if (auto synattr = std::get_if<SynAttr>(&selected)) {
      auto de = Declaration::findDeclarationForSynonym(decls, synattr->synonym)
                    ->getDesignEntity();
      synattr_de_map.insert({synattr->synonym.synonym, de});
    }
  }
  return synattr_de_map;
}

void QueryExecutor::applyAttributesToResults(
    std::vector<QE::ResultItem>* selected_declarations,
    std::vector<std::vector<std::string>>& results,
    unordered_map<std::string, DesignEntity>& synonym_de_map) {
  // Apply the synattr transform on each element that needs it
  for (size_t row_idx = 0; row_idx < results.size(); row_idx++) {
    for (size_t col_idx = 0; col_idx < results[row_idx].size(); col_idx++) {
      // If this column is for a SynAttr result, transform it.
      if (auto synattr =
              std::get_if<SynAttr>(&selected_declarations->at(col_idx))) {
        std::string synonym = synattr->synonym.synonym;
        std::string value = results[row_idx][col_idx];
        results[row_idx][col_idx] = applyAttrToDesignEntityValue(
            pkb, synonym_de_map[synonym], value, synattr->attrName);
      }
    }
  }
}

std::vector<std::string> QueryExecutor::joinResults(
    const std::vector<std::vector<std::string>>& results) {
  std::vector<std::string> out_result;
  out_result.reserve(results.size());
  // Join all vectors by comma
  for (const auto& row : results) {
    out_result.push_back(Utils::join(row.begin(), row.end()));
  }
  return out_result;
}