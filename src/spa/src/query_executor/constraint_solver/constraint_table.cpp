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

//! Filter an existing table based on an incoming paired-var constraint
bool ConstraintTable::joinWithSetBy(
    const string& var_to_join, const string& other_var,
    const unordered_map<string, string>& pair_map) {
  /*
    Table

    a | v | x
    ---------
    1   2   3
    3   4   5

    Incoming paired constraint
    a | z
    -----
    1   17
    4   9

    Remove rows that don't match the column we're supposed to match on

  */
  size_t column_to_join_idx = name_column_map[var_to_join];
  bool added_new_col = false;
  table.erase(std::remove_if(table.begin(), table.end(),
                             [&](vector<string>& row) {
                               string& val_join = row[column_to_join_idx];

                               bool must_delete =
                                   pair_map.find(val_join) == pair_map.end();

                               // THIS IS SUPER SKETCHY - modifying row if no
                               // delete, since this is a reference
                               // Is this even possible?
                               // But this removes needs for copying/2x iter
                               if (!must_delete) {
                                 row.push_back(pair_map.at(val_join));
                                 added_new_col = true;
                               }

                               // Remove if it's not in the incoming
                               // constraints
                               return must_delete;
                             }),
              table.end());
  // We added a new columns ==> need to update table index mapping
  if (added_new_col) {
    size_t new_col_idx = table[0].size() - 1;
    name_column_map.insert({other_var, new_col_idx});
    return true;
  } else {
    // TODO
    // join failed - no results should be left in table??
    // should do something??
    return true;
  }
}

//! Filter an existing table based on an incoming paired-var constraint
bool ConstraintTable::joinWithTableBy(const string& var_to_join,
                                      const ConstraintTable& other_table) {
  /*
    Existing Table

    a | v | x
    ---------
    1   2   3
    3   4   5

    Incoming table
    v | s | y
    ---------
    1   2   3
    2   4   5

    Hash-Join algorithm:
    1. Create a hashmap of v from value to row index: 2 -> 1, 4 -> 2 for
    existing table
    2. For each v in incoming table (larger), check if v exists in hashmap
    3. If no match, both rows are no longer considered
    4. If match, merge both rows (append incoming to existing without the common
    col)
    5. Update the column index for the existing table

  */
}