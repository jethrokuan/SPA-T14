#include "query_executor/constraint_solver/query_constraints.h"
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>

void QueryConstraints::addToSingleVariableConstraints(
    std::string var_name, SingleConstraintSet constraint_values) {
  singleVariableConstraintList.push_back({var_name, constraint_values});
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

void QueryConstraints::addToAllPossibleValuesList(
    std::string var_name, std::set<std::string> constraint_values) {
  allPossibleValuesList.push_back({var_name, constraint_values});
  addToSingleVariableConstraints(var_name, constraint_values);
}
