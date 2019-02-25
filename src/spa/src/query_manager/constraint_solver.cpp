#include "query_manager/constraint_solver.h"
#include <cassert>
#include <set>
#include <string>
#include <vector>

const std::string ConstraintSolver::DUMMY_SYNONYM = "@";

AllowedValuesPair ConstraintSolver::makeAllowedValues(
    Synonym& syn, std::vector<std::string>& vals) {
  SynonymPair syn_pair = std::make_pair(syn.synonym, DUMMY_SYNONYM);
  AllowedValueSet allowed_values;
  for (auto val : vals) {
    allowed_values.insert(std::make_pair(val, DUMMY_SYNONYM));
  }
  return std::make_pair(syn_pair, allowed_values);
}

std::string ConstraintSolver::getStringPairAsString(
    std::pair<std::string, std::string> string_pair) {
  return "(" + string_pair.first + ", " + string_pair.second + ")";
}

void ConstraintSolver::printAllowedValuesPair(AllowedValuesPair& avp) {
  std::cout << "Constraining values " << getStringPairAsString(avp.first)
            << "\n";

  for (auto allowed_value : avp.second) {
    std::cout << getStringPairAsString(allowed_value) << " ";
  }

  std::cout << "\n";
}