#include "query_executor/constraint_solver/constraint_table.h"
#include <algorithm>

//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
void ConstraintTable::initWithSingleVariable(
    const string& var_name, const SingleConstraintSet& constraint_values) {
  assert(table.size() == 0);  // invariant - initializer
  // We need to add a column of values
  // Update tracking data structure
  name_column_map.insert({var_name, 0});
  for (const auto& value : constraint_values) {
    table.push_back(vector{value});
  }
}

//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
void ConstraintTable::initWithPairedVariables(
    const string& var1_name, const string& var2_name,
    const PairedConstraintSet& constraint_values) {
  assert(table.size() == 0);  // invariant - initializer
  // We need to add a column of values
  // Update tracking data structure
  name_column_map.insert({var1_name, 0});
  name_column_map.insert({var2_name, 1});
  for (const auto& value : constraint_values) {
    table.push_back(vector{value.first, value.second});
  }
}

//! Filter an existing table based on an incoming single-var constraint
void ConstraintTable::filterBy(const string& var_name,
                               const SingleConstraintSet& constraint_values) {
  size_t column_idx = name_column_map[var_name];
  table.erase(std::remove_if(table.begin(), table.end(),
                             [&](vector<string>& row) {
                               string& val = row[column_idx];
                               // Remove if it's not in the incoming constraints
                               return constraint_values.find(val) ==
                                      constraint_values.end();
                             }),
              table.end());
}

//! Filter an existing table based on an incoming paired-var constraint
void ConstraintTable::filterBy(const string& var1_name, const string& var2_name,
                               const PairedConstraintSet& constraint_values) {
  size_t column1_idx = name_column_map[var1_name];
  size_t column2_idx = name_column_map[var2_name];
  table.erase(std::remove_if(table.begin(), table.end(),
                             [&](vector<string>& row) {
                               string& val1 = row[column1_idx];
                               string& val2 = row[column2_idx];
                               // Remove if it's not in the incoming
                               // constraints
                               return constraint_values.find({val1, val2}) ==
                                      constraint_values.end();
                             }),
              table.end());
}