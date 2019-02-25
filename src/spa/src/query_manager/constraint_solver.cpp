#include "query_manager/constraint_solver.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
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

AllowedValuesPair ConstraintSolver::makeAllowedValues(Synonym& s1, Synonym& s2,
                                                      AllowedValueSet& avs) {
  SynonymPair syn_pair = std::make_pair(s1.synonym, s2.synonym);
  return std::make_pair(syn_pair, avs);
}

//! Actually constrain the set of values and select the synonym indicated
std::vector<std::string> ConstraintSolver::constrainAndSelect(
    std::vector<AllowedValuesPair> allowedValues, std::string toSelect) {
  auto synonym_constraints = intersectConstraints(allowedValues);

  for (auto m : synonym_constraints) {
    std::cout << "Intersected constraints for: " << m.first << std::endl;
    for (auto s : m.second) {
      std::cout << s << " ";
    }
    std::cout << "\n";
  }

  // With the constraints, need to remove pairs that don't fit the constraints
  std::vector<AllowedValuesPair> constrained_values;
  for (AllowedValuesPair allowedValuePair : allowedValues) {
    SynonymPair syn_pair = allowedValuePair.first;
    auto allowed_values_constrained = std::set<AllowedValue>();
    for (const AllowedValue& allowedValue : allowedValuePair.second) {
      // Both allowed values must be present in the intersection list
      bool val1_present =
          synonym_constraints[syn_pair.first].find(allowedValue.first) !=
          synonym_constraints[syn_pair.first].end();
      bool val2_present =
          synonym_constraints[syn_pair.second].find(allowedValue.second) !=
          synonym_constraints[syn_pair.second].end();
      if (val1_present && val2_present) {
        allowed_values_constrained.insert(allowedValue);
      }
    }
    constrained_values.push_back({syn_pair, allowed_values_constrained});
  }

  // Re-constrain this set
  synonym_constraints = intersectConstraints(constrained_values);

  for (auto m : synonym_constraints) {
    std::cout << "Re-Intersected constraints for: " << m.first << std::endl;
    for (auto s : m.second) {
      std::cout << s << " ";
    }
    std::cout << "\n";
  }

  // With

  // Select the one we want
  auto set_to_return = synonym_constraints[toSelect];
  std::vector<std::string> result(set_to_return.begin(), set_to_return.end());
  return result;
}

std::map<std::string, std::set<std::string>>
ConstraintSolver::intersectConstraints(
    std::vector<AllowedValuesPair> allowedValues) {
  std::map<std::string, std::set<std::string>> synonym_constraints;

  for (auto avp : allowedValues) {
    auto syn1 = avp.first.first;
    if (synonym_constraints.find(syn1) == synonym_constraints.end()) {
      // Get all first elements from set
      synonym_constraints[syn1] = getFirstsFromSet(avp.second);
    } else {
      std::cout << "Found syn1 again " << syn1 << '\n';
      auto new_constraints = getFirstsFromSet(avp.second);
      auto existing_constraints = synonym_constraints[syn1];
      synonym_constraints[syn1].clear();
      std::set_intersection(existing_constraints.begin(),
                            existing_constraints.end(), new_constraints.begin(),
                            new_constraints.end(),
                            std::inserter(synonym_constraints[syn1],
                                          synonym_constraints[syn1].begin()));
    }

    auto syn2 = avp.first.second;
    if (synonym_constraints.find(syn2) == synonym_constraints.end()) {
      // Get all first elements from set
      synonym_constraints[syn2] = getSecondsFromSet(avp.second);
    } else {
      std::cout << "Found syn2 again " << syn2 << '\n';
      auto new_constraints = getSecondsFromSet(avp.second);
      auto existing_constraints = synonym_constraints[syn2];
      synonym_constraints[syn2].clear();
      std::set_intersection(existing_constraints.begin(),
                            existing_constraints.end(), new_constraints.begin(),
                            new_constraints.end(),
                            std::inserter(synonym_constraints[syn2],
                                          synonym_constraints[syn2].begin()));
    }
  }

  return synonym_constraints;
}

std::set<std::string> ConstraintSolver::getFirstsFromSet(AllowedValueSet& avs) {
  std::set<std::string> result;
  for (auto a : avs) {
    result.insert(a.first);
  }
  return result;
}

std::set<std::string> ConstraintSolver::getSecondsFromSet(
    AllowedValueSet& avs) {
  std::set<std::string> result;
  for (auto a : avs) {
    result.insert(a.second);
  }
  return result;
}
