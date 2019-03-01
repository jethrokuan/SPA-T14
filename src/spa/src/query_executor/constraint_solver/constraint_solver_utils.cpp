#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <string>
#include <vector>
#include "query_executor/constraint_solver/constraint_solver.h"

/***
 *
 * Helper methods to initialize constraints
 *
 ***/

TupledConstraint ConstraintSolver::makeAllowedValues(
    Synonym& syn, std::vector<std::string>& vals) {
  return makeAllowedValues(syn.synonym, vals);
}

TupledConstraint ConstraintSolver::makeAllowedValues(
    std::string& syn, std::vector<std::string>& vals) {
  SynonymPair syn_pair = std::make_pair(syn, DUMMY_SYNONYM);
  AllowedValuePairSet allowed_values;
  for (auto val : vals) {
    allowed_values.insert(std::make_pair(val, DUMMY_SYNONYM));
  }
  return std::make_pair(syn_pair, allowed_values);
}

TupledConstraint ConstraintSolver::makeAllowedValues(Synonym& s1, Synonym& s2,
                                                     AllowedValuePairSet& avs) {
  SynonymPair syn_pair = std::make_pair(s1.synonym, s2.synonym);
  return std::make_pair(syn_pair, avs);
}

/***
 *
 * Functional-style manipulation of constraint sets
 *
 ***/

std::set<std::string> ConstraintSolver::getFirstsFromSet(
    AllowedValuePairSet& avs) {
  std::set<std::string> result;
  for (auto a : avs) {
    result.insert(a.first);
  }
  return result;
}

std::set<std::string> ConstraintSolver::getSecondsFromSet(
    AllowedValuePairSet& avs) {
  std::set<std::string> result;
  for (auto a : avs) {
    result.insert(a.second);
  }
  return result;
}

/***
 *
 * Printing
 *
 ***/

std::string ConstraintSolver::getStringPairAsString(
    std::pair<std::string, std::string> string_pair) {
  return "(" + string_pair.first + ", " + string_pair.second + ")";
}

void ConstraintSolver::printAllowedValuesPair(TupledConstraint& avp) {
  std::cout << "Constraining values " << getStringPairAsString(avp.first)
            << "\n";

  for (auto allowed_value : avp.second) {
    std::cout << getStringPairAsString(allowed_value) << " ";
  }

  std::cout << "\n";
}

void ConstraintSolver::printConstraints(SingleVariableConstraints constraints) {
  for (auto m : constraints) {
    std::cout << "Intersected constraints for: " << m.first << std::endl;
    for (auto s : m.second) {
      std::cout << s << " ";
    }
    std::cout << "\n";
  }
}
