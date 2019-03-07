#include "query_executor/constraint_solver/query_constraints.h"
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>

void QueryConstraints::addToSingleVariableConstraints(
    std::string var_name, SingleConstraintSet constraint_values) {
  bool varExistsInMap = singleVariableConstraintMap.find(var_name) !=
                        singleVariableConstraintMap.end();
  if (varExistsInMap) {
    auto existing_constraints = singleVariableConstraintMap.at(var_name);
    SingleConstraintSet intersected_set;
    std::set_intersection(
        existing_constraints.begin(), existing_constraints.end(),
        constraint_values.begin(), constraint_values.end(),
        std::inserter(intersected_set, intersected_set.begin()));
    singleVariableConstraintMap[var_name] = intersected_set;
  } else {
    singleVariableConstraintMap[var_name] = constraint_values;
  }
}

void QueryConstraints::addToPairedVariableConstraints(
    std::string var1_name, std::string var2_name,
    PairedConstraintSet constraint_values) {
  // Assumes constraint set in form (var1_name, var2_name)
  // Need to insert in alphabetical order to have some consistency
  // Do it stably: only swap if var1_name > var2_name
  if (var1_name > var2_name) {
    pairedVariableConstraintList.push_back(
        {{var2_name, var1_name}, swapElementsInSet(constraint_values)});
  } else {
    pairedVariableConstraintList.push_back(
        {{var1_name, var2_name}, constraint_values});
  }
}

void QueryConstraints::addToAllPossibleValues(
    std::string var_name, std::set<std::string> constraint_values) {
  allPossibleValues.insert({var_name, constraint_values});
  addToSingleVariableConstraints(var_name, constraint_values);
}

bool QueryConstraints::containsNoAllowedResults(
    std::vector<std::string> constraint_values, std::string var_name) {
  // Doing this for efficiency - no need to actually do the intersection
  for (auto c : constraint_values) {
    if (allPossibleValues.at(var_name).find(c) !=
        allPossibleValues.at(var_name).end()) {
      // One of the values in the constraint set is in the domain of all
      // possible values
      return false;
    }
  }
  return true;
}
