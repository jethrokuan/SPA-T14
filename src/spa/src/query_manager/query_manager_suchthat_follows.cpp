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
  // Case 1: Selected variable is in this such_that, left argument
  return std::vector<std::string>();
}