#include <cassert>
#include <string>
#include <vector>
#include "query_manager/query_manager.h"
#include "utils/utils.h"

using namespace QE;

//! If all args are statement number / quoted ident - can return bool from PKB
bool QueryManager::isBooleanSuchThat(SuchThat* such_that) {
  // Must be true from earlier query processing
  assert(such_that != nullptr);

  // Both arguments are of type StmtOrEntRef
  // Check one level deeper in first arg to check for underscore
  auto such_that_firstarg = such_that->getFirstArg();
  auto such_that_secondarg = such_that->getSecondArg();
  // Both arguments need to be either a statement number or quoted identifier to
  // be a boolean result
  return std::visit(
             overload{[](StmtRef& s) {
                        return std::holds_alternative<StatementNumber>(s);
                      },
                      [](EntRef& e) {
                        return std::holds_alternative<QuoteIdent>(e);
                      }},
             such_that_firstarg) &&
         std::visit(
             overload{[](StmtRef& s) {
                        return std::holds_alternative<StatementNumber>(s);
                      },
                      [](EntRef& e) {
                        return std::holds_alternative<QuoteIdent>(e);
                      }},
             such_that_secondarg);
}

bool QueryManager::isBooleanSuchThatTrue(SuchThat* such_that) {
  // Must be true from earlier query processing
  assert(such_that != nullptr);

  auto relation = such_that->getRelation();
  auto arg1 = suchThatArgToString(such_that->getFirstArg());
  auto arg2 = suchThatArgToString(such_that->getSecondArg());

  switch (relation) {
    case Relation::Follows:
      // std::cout << "FOLLOWS BOOLEAN DETECTED";
      return pkb->isLineFollowLine(arg1, arg2);
    case Relation::FollowsT:
      // std::cout << "FOLLOWS* BOOLEAN DETECTED";
      return pkb->isLineFollowLineS(arg1, arg2);
    case Relation::Parent:
      // std::cout << "PARENT BOOLEAN DETECTED";
      return pkb->isLineParentLine(arg1, arg2);
    case Relation::ParentT:
      // std::cout << "PARENT* BOOLEAN DETECTED";
      return pkb->isLineParentLineS(arg1, arg2);
    case Relation::ModifiesS:
      // std::cout << "Modifies-Statement BOOLEAN DETECTED";
      return pkb->isLineModifiesVar(arg1, arg2);
    case Relation::UsesS:
      // std::cout << "Uses-Statement BOOLEAN DETECTED";
      return pkb->isLineUsesVar(arg1, arg2);
    default:
      assert(false);
  }

  // Should never reach here
  assert(false);
}

std::string QueryManager::suchThatArgToString(StmtOrEntRef arg) {
  return std::visit(overload{[](StmtRef& s) { return stmtRefToString(s); },
                             [](EntRef& e) { return entRefToString(e); }},
                    arg);
}

std::string QueryManager::stmtRefToString(StmtRef arg) {
  return std::visit(
      overload{[](Synonym& s) { return s.synonym; },
               [](Underscore&) { return std::string("_"); },
               [](StatementNumber& s) { return std::to_string(s); }},
      arg);
}

std::string QueryManager::entRefToString(EntRef arg) {
  return std::visit(overload{[](Synonym& s) { return s.synonym; },
                             [](Underscore&) { return std::string("_"); },
                             [](QuoteIdent& s) {
                               // Remove quotes
                               return s.quote_ident.substr(
                                   1, s.quote_ident.size() - 2);
                             }},
                    arg);
}

std::optional<Synonym> QueryManager::getSuchThatArgAsSynonym(StmtOrEntRef arg) {
  auto add_pointer_synonym =
      std::visit(overload{[](StmtRef& s) { return std::get_if<Synonym>(&s); },
                          [](EntRef& e) { return std::get_if<Synonym>(&e); }},
                 arg);
  return add_pointer_synonym ? std::make_optional<Synonym>(*add_pointer_synonym)
                             : std::nullopt;
}

bool QueryManager::isSuchThatArgUnderscore(StmtOrEntRef arg) {
  return std::visit(
      overload{[](StmtRef& s) { return std::holds_alternative<Underscore>(s); },
               [](EntRef& e) { return std::holds_alternative<Underscore>(e); }},
      arg);
}

//! Returns argument as a basic statementref or quoteident as string
std::optional<std::string> QueryManager::getSuchThatArgAsBasic(
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
