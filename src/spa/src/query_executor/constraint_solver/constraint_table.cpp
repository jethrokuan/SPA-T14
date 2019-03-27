#include "query_executor/constraint_solver/constraint_table.h"
#include <algorithm>
//! Adds a new column-name association to the table
void ConstraintTable::addNewColumnName(const string& column_name) {
  name_column_map.insert({column_name, next_column_idx});
  next_column_idx++;
}

//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
void ConstraintTable::initWithSingleVariable(
    const string& var_name, const SingleConstraintSet& constraint_values) {
  assert(table.size() == 0);  // invariant - initializer
  // We need to add a column of values
  // Update tracking data structure
  addNewColumnName(var_name);
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
  addNewColumnName(var1_name);
  addNewColumnName(var2_name);
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
    const unordered_map<string, vector<string>>& pair_map) {
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
    1   9
    4   100

    Remove rows that don't match the column we're supposed to match on.
    If there are duplicates - (a = 1) on LHS, (a = 1, a = 1) on RHS,
    we need to make sure we get both rows

  */
  size_t column_to_join_idx = name_column_map[var_to_join];
  bool added_new_col = false;

  vector<vector<string>> new_table;
  for (vector<string> row : table) {
    // Check if this value is present in the incoming set
    string& val_join = row[column_to_join_idx];
    bool must_delete = pair_map.find(val_join) == pair_map.end();
    if (must_delete) continue;

    // If we found the join value in the map, insert all other values
    auto values_to_append_for_this_key = pair_map.at(val_join);
    for (auto& value_to_insert : values_to_append_for_this_key) {
      vector<string> new_row = row;
      new_row.push_back(value_to_insert);
      new_table.push_back(new_row);
    }
    added_new_col = true;
  }

  // Replace the current table with the newly computed table
  // In-place modification is too tedious
  table = new_table;

  // We added a new columns ==> need to update table index mapping
  if (added_new_col) {
    addNewColumnName(other_var);
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

//! Cartesian product of two tables: t1.size() x t2.size() rows as output
ConstraintTable ConstraintTable::cartesianProduct(ConstraintTable& t1,
                                                  ConstraintTable& t2) {
  if (t1.size() == 0) return t2;
  if (t2.size() == 0) return t1;
  ConstraintTable out_table;
  // Construct new column headers
  for (auto& [name, col_idx] : t1.name_column_map) {
    out_table.addNewColumnName(name);
  }
  for (auto& [name, col_idx] : t2.name_column_map) {
    out_table.addNewColumnName(name);
  }

  // Cartesia product of rows in both tables
  for (auto row1 : t1.table) {
    for (auto row2 : t2.table) {
      vector<string> new_row;
      new_row.insert(new_row.end(), row1.begin(), row1.end());
      new_row.insert(new_row.end(), row2.begin(), row2.end());
      out_table.table.push_back(new_row);
    }
  }
  return out_table;
}

//! Get a single column of values from the table based on the column name
vector<string> ConstraintTable::getColumnByName(const string& name) {
  size_t col_idx = name_column_map[name];
  vector<string> out_column;
  for (auto& row : table) {
    out_column.push_back(row[col_idx]);
  }
  return out_column;
}

ConstraintTable ConstraintTable::getSubTable(
    const vector<string>& vars_to_select) {
  vector<string> vars_to_select_in_table;
  ConstraintTable out_table;
  for (const auto& var_to_select : vars_to_select) {
    // Which vars need to be selected?
    if (name_column_map.find(var_to_select) != name_column_map.end()) {
      vars_to_select_in_table.push_back(var_to_select);
      out_table.addNewColumnName(var_to_select);
    }
  }
  if (vars_to_select.empty()) return out_table;  // no vars to select
  for (auto& row : table) {
    vector<string> out_row;
    for (const auto& var_to_select : vars_to_select_in_table) {
      size_t to_select_idx = name_column_map[var_to_select];
      out_row.push_back(row[to_select_idx]);
    }
    out_table.table.push_back(out_row);
  }
  return out_table;
}