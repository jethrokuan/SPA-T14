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

  // Select the one we want
  auto set_to_return = synonym_constraints[toSelect];
  std::vector<std::string> result(set_to_return.begin(), set_to_return.end());
  return result;
  // TODO: WE NEED TO RUN THIS UNTIL NO DIFFERENCES SPOTTED (IN FUTURE ITERS)
}

std::map<std::string, std::set<std::string>>
ConstraintSolver::intersectConstraints(
    std::vector<AllowedValuesPair> allowedValues) {
  std::map<std::string, std::set<std::string>> synonym_constraints;

  for (auto avp : allowedValues) {
    auto syn1 = avp.first.first;
    intersectTwoConstraints(synonym_constraints, syn1,
                            getFirstsFromSet(avp.second));
    auto syn2 = avp.first.second;
    intersectTwoConstraints(synonym_constraints, syn2,
                            getSecondsFromSet(avp.second));
  }

  return synonym_constraints;
}

void ConstraintSolver::intersectTwoConstraints(
    std::map<std::string, std::set<std::string>>& synonym_constraints,
    std::string& syn, std::set<std::string> new_constraints) {
  if (synonym_constraints.find(syn) == synonym_constraints.end()) {
    synonym_constraints[syn] = new_constraints;
  } else {
    auto existing_constraints = synonym_constraints[syn];
    synonym_constraints[syn].clear();
    std::set_intersection(existing_constraints.begin(),
                          existing_constraints.end(), new_constraints.begin(),
                          new_constraints.end(),
                          std::inserter(synonym_constraints[syn],
                                        synonym_constraints[syn].begin()));
  }
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

void ConstraintSolver::printConstraints(
    std::map<std::string, std::set<std::string>> constraints) {
  for (auto m : constraints) {
    std::cout << "Intersected constraints for: " << m.first << std::endl;
    for (auto s : m.second) {
      std::cout << s << " ";
    }
    std::cout << "\n";
  }
}
