#include "query_executor/constraint_solver/constraint_database.h"
//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
void ConstraintTable::addVariable(
    const std::string& var_name, const SingleConstraintSet& constraint_values) {
  // We need to add a column of values
  // Update tracking data structure
  name_column_map.insert({var_name, 0});
  for (const auto& value : constraint_values) {
    table.push_back(vector{value});
  }
}

//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
void ConstraintDatabase::addToSingleVariableConstraints(
    const std::string& var_name, const SingleConstraintSet& constraint_values) {
  // Is an instance of this variable already stored?
  auto matching_table_idx = getTableIdxForVar(var_name);

  // If no table, create one (disjoint, so needs new table)
  if (!matching_table_idx) {
    // Add a new table with a single columns, update tracking structure
    ConstraintTable table;
    table.addVariable(var_name, constraint_values);
    size_t new_table_index = tables.size();
    tables.push_back(table);
    name_table_map.insert({var_name, new_table_index});
  } else {
    assert(false);
  }

  std::cout << *this;
}

std::optional<size_t> ConstraintDatabase::getTableIdxForVar(
    const string& var_name) {
  auto matching_table_idx = name_table_map.find(var_name);
  if (matching_table_idx == name_table_map.end()) {
    return std::nullopt;
  } else {
    return matching_table_idx->second;
  }
}
