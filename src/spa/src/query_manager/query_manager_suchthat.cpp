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
  return std::visit(
      overload{
          [](StmtRef& s) { return std::holds_alternative<StatementNumber>(s); },
          [](EntRef& e) { return std::holds_alternative<QuoteIdent>(e); }},
      such_that_firstarg);
}

bool QueryManager::isBooleanSuchThatTrue(SuchThat* such_that) {
  // Must be true from earlier query processing
  assert(such_that != nullptr);

  // Assuming we have a boolean such_that - send the correct API call to PKB
  return true;
}