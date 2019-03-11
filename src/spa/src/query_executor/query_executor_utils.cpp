#include <cassert>
#include <string>
#include <vector>
#include "query_executor/query_executor.h"
#include "utils/utils.h"

using namespace QE;

std::string QueryExecutor::suchThatArgToString(StmtOrEntRef arg) {
  return std::visit(overload{[](StmtRef& s) { return stmtRefToString(s); },
                             [](EntRef& e) { return entRefToString(e); }},
                    arg);
}

std::string QueryExecutor::stmtRefToString(StmtRef arg) {
  return std::visit(
      overload{[](Synonym& s) { return s.synonym; },
               [](Underscore&) { return std::string("_"); },
               [](StatementNumber& s) { return std::to_string(s); }},
      arg);
}

std::string QueryExecutor::entRefToString(EntRef arg) {
  return std::visit(overload{[](Synonym& s) { return s.synonym; },
                             [](Underscore&) { return std::string("_"); },
                             [](QuoteIdent& s) {
                               // Remove quotes
                               return s.quote_ident.substr(
                                   1, s.quote_ident.size() - 2);
                             }},
                    arg);
}

std::optional<Synonym> QueryExecutor::getSuchThatArgAsSynonym(
    StmtOrEntRef arg) {
  auto add_pointer_synonym =
      std::visit(overload{[](StmtRef& s) { return std::get_if<Synonym>(&s); },
                          [](EntRef& e) { return std::get_if<Synonym>(&e); }},
                 arg);
  return add_pointer_synonym ? std::make_optional<Synonym>(*add_pointer_synonym)
                             : std::nullopt;
}

bool QueryExecutor::isSuchThatArgUnderscore(StmtOrEntRef arg) {
  return std::visit(
      overload{[](StmtRef& s) { return std::holds_alternative<Underscore>(s); },
               [](EntRef& e) { return std::holds_alternative<Underscore>(e); }},
      arg);
}

//! Returns argument as a basic statementref or quoteident as string
std::optional<std::string> QueryExecutor::getSuchThatArgAsBasic(
    StmtOrEntRef arg) {
  return std::visit(
      overload{[](StmtRef& s) -> std::optional<std::string> {
                 if (auto res = std::get_if<StatementNumber>(&s)) {
                   return std::make_optional<std::string>(stmtRefToString(s));
                 } else {
                   return std::nullopt;
                 }
               },
               [](EntRef& e) -> std::optional<std::string> {
                 if (auto res = std::get_if<QuoteIdent>(&e)) {
                   return std::make_optional<std::string>(entRefToString(e));
                 } else {
                   return std::nullopt;
                 }
               }},
      arg);
}

std::vector<std::string> QueryExecutor::getSynonymsFromSelect(
    std::vector<Declaration*>* decls) {
  std::vector<std::string> result;
  for (const auto& decl : *decls) {
    result.push_back(decl->getSynonym().synonym);
  }
  return result;
}
