#include "query_executor/constraint_solver/query_constraints.h"
#include <iostream>
#include <optional>

#include <string>
#include <unordered_set>
#include <vector>
#include "utils/utils.h"

void QueryConstraints::addToSingleVariableConstraints(
    std::string var_name, std::unordered_set<std::string> constraint_values) {
  bool varExistsInMap = singleVariableConstraintMap.find(var_name) !=
                        singleVariableConstraintMap.end();
  if (varExistsInMap) {
    auto existing_constraints = singleVariableConstraintMap.at(var_name);
    SingleConstraintSet intersected_set = Utils::unorderedSetIntersection(
        existing_constraints, constraint_values);
    singleVariableConstraintMap[var_name] = intersected_set;
  } else {
    singleVariableConstraintMap[var_name] = constraint_values;
  }
}

void QueryConstraints::addToPairedVariableConstraints(
    std::string var1_name, std::string var2_name,
    PairedConstraintSet constraint_values) {
  // Assumes constraint set in form (var1_name, var2_name)
  // Need to insert in alphabetical order to allow for consistent intersection
  // Do it stably: only swap if var1_name > var2_name
  if (var1_name > var2_name) {
    constraint_values = swapElementsInSet(constraint_values);
    std::string temp = var1_name;
    var1_name = var2_name;
    var2_name = temp;
  }
  // Intersect + add to existing constraints, or just create new set
  bool varExistsInMap =
      pairedVariableConstraintMap.find({var1_name, var2_name}) !=
      pairedVariableConstraintMap.end();
  if (varExistsInMap) {
    auto existing_constraints =
        pairedVariableConstraintMap.at({var1_name, var2_name});
    PairedConstraintSet intersected_set = Utils::unorderedSetIntersection(
        existing_constraints, constraint_values);
    pairedVariableConstraintMap[{var1_name, var2_name}] = intersected_set;
  } else {
    pairedVariableConstraintMap[{var1_name, var2_name}] = constraint_values;
  }
}

void QueryConstraints::addToAllPossibleValues(
    std::string var_name, std::unordered_set<std::string> constraint_values) {
  allPossibleValues.insert({var_name, constraint_values});
  addToSingleVariableConstraints(var_name, constraint_values);
}

bool QueryConstraints::containsNoAllowedResults(
    const std::vector<std::string> constraint_values,
    const std::string var_name) {
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

bool QueryConstraints::containsNoAllowedResults(
    const std::unordered_set<std::string> constraint_values,
    const std::string var_name) {
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
