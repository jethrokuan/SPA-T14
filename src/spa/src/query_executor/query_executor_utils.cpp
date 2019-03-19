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