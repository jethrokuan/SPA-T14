#include "query_executor/constraint_solver/constraint_database.h"
#include <algorithm>

/***
 *
 *
 *
 *  CONSTRAINT TABLE
 *
 *
 *
 ***/
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

//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
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

/***
 *
 *
 *
 *  CONSTRAINT DATABASE
 *
 *
 *
 ***/

//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
void ConstraintDatabase::addToSingleVariableConstraints(
    const string& var_name, const SingleConstraintSet& constraint_values) {
  // Is an instance of this variable already stored?
  auto matching_table_idx = getTableIdxForVar(var_name);

  // If no table, create one (disjoint, so needs new table)
  if (!matching_table_idx) {
    ConstraintTable table;
    table.initWithSingleVariable(var_name, constraint_values);
    size_t new_table_index = tables.size();
    tables.push_back(table);
    name_table_map.insert({var_name, new_table_index});
  } else {
    // FILTER algorithm: filter existing table by this constraint set
    tables[*matching_table_idx].filterBy(var_name, constraint_values);
  }
}

//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
void ConstraintDatabase::addToPairedVariableConstraints(
    const string& var1_name, const string& var2_name,
    const PairedConstraintSet& constraint_values) {
  // Is an instance of this variable already stored?
  auto matching_table_idx = getTableIdxForVars(var1_name, var2_name);

  // If no table, create one (disjoint, so needs new table)
  if (!matching_table_idx) {
    ConstraintTable table;
    table.initWithPairedVariables(var1_name, var2_name, constraint_values);
    size_t new_table_index = tables.size();
    tables.push_back(table);
    name_table_map.insert({var1_name, new_table_index});
    name_table_map.insert({var2_name, new_table_index});
  } else {
    assert(false);
  }
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

std::optional<size_t> ConstraintDatabase::getTableIdxForVars(
    const string& var1_name, const string& var2_name) {
  auto matching_table_idx_1 = name_table_map.find(var1_name);
  auto matching_table_idx_2 = name_table_map.find(var2_name);

  if (matching_table_idx_1 == name_table_map.end() ||
      matching_table_idx_2 == name_table_map.end()) {
    // No such table found
    return std::nullopt;
  } else if (matching_table_idx_1->second != matching_table_idx_2->second) {
    // They are in two separate tables for now
    return std::nullopt;
  } else {
    // There is a table that contains these two variables right now
    return matching_table_idx_1->second;
  }
}
