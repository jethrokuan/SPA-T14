#include "query_manager/constraint_solver.h"
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <string>
#include <vector>

const std::string ConstraintSolver::DUMMY_SYNONYM = "@";

//! Powerful template to do functional-style map
template <typename InType,
          template <typename U, typename alloc = std::allocator<U>>
          class InContainer,
          template <typename V, typename alloc = std::allocator<V>>
          class OutContainer = InContainer,
          typename OutType = InType>
OutContainer<OutType> mapf(const InContainer<InType>& input,
                           std::function<OutType(const InType&)> func) {
  OutContainer<OutType> output;
  output.resize(input.size());
  transform(input.begin(), input.end(), output.begin(), func);
  return output;
}

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
  return {};
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
      auto new_constraints = getFirstsFromSet(avp.second);
      auto existing_constraints = synonym_constraints[syn1];
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
      auto new_constraints = getSecondsFromSet(avp.second);
      auto existing_constraints = synonym_constraints[syn2];
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
