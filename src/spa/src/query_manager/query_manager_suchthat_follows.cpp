#include <cassert>
#include <string>
#include <vector>
#include "query_evaluator/pql/declaration.h"
#include "query_manager/query_manager.h"
#include "utils/utils.h"

using namespace QE;

std::vector<std::string> QueryManager::handleFollowsSuchThat(
    Query* query, std::optional<Synonym> arg1AsSynonym,
    std::optional<Synonym> arg2AsSynonym, bool arg1InSelect, bool arg2InSelect,
    bool arg1IsUnderscore, bool arg2IsUnderscore,
    std::optional<std::string> arg1AsBasic,
    std::optional<std::string> arg2AsBasic) {
  if (arg1AsSynonym && arg1InSelect && arg2AsBasic) {
    // Case 1: Selected variable is in this such_that, left argument
    // Follows(s, 3)
    return pkb->getBeforeLineS(*arg2AsBasic);
  } else if (arg2AsSynonym && arg2InSelect && arg1AsBasic) {
    // Case 2: Selected variable is in this such_that, left argument
    // Follows(3, s)
    return pkb->getFollowingLineS(*arg1AsBasic);
  } else if (arg1AsSynonym && arg1InSelect && arg2IsUnderscore) {
    // Case 3: Selected variable is in this such_that, left argument, right arg underscore
    // Follows(s, _)

  }
  std::cout << "No cases matched - this is a problem\n";
  return std::vector<std::string>();
}