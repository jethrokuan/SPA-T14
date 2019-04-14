#include "query_executor/constraint_solver/constraint_database.h"
#include <algorithm>
#include <set>
using std::set;

//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
bool ConstraintDatabase::addToSingleVariableConstraints(
    const string& var_name, const SingleConstraintSet& constraint_values) {
  // Is an instance of this variable already stored?
  auto matching_table_idx = getTableIdxForVar(var_name);
  // If no table, create one (disjoint, so needs new table)
  if (!matching_table_idx) {
    createNewTable(var_name, constraint_values);
    return true;
  } else {
    // FILTER algorithm: filter existing table by this constraint set
    return tables[*matching_table_idx].filterBy(var_name, constraint_values);
  }
}

//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
bool ConstraintDatabase::addToPairedVariableConstraints(
    const string& var1_name, const string& var2_name,
    const PairedConstraintSet& constraint_values) {
  // Is an instance of this variable already stored?
  auto matching_table_iterators = getTableIdxForVars(var1_name, var2_name);
  // If no table, create one (disjoint, so needs new table)
  if (!matching_table_iterators) {
    createNewTable(var1_name, var2_name, constraint_values);
    return true;
  } else if ((matching_table_iterators->first != name_table_map.end()) &&
             (matching_table_iterators->second != name_table_map.end()) &&
             matching_table_iterators->first->second ==
                 matching_table_iterators->second->second) {
    // Exact match to same table, filter only
    size_t table_idx = matching_table_iterators->first->second;
    return tables[table_idx].filterBy(var1_name, var2_name, constraint_values);
  } else {
    // 2 possibilities
    //  1. Only one variable is found, so only one join needs to be done
    //  2. Both variables are found in different tables.
    //     --> we have to do one join with table-set + one table-table join
    bool have_done_table_set_join = false;
    if (matching_table_iterators->first != name_table_map.end()) {
      // Join required on var1_name
      size_t table_idx = matching_table_iterators->first->second;
      bool join_at_least_one = doTableSetJoin(table_idx, var1_name, var2_name,
                                              constraint_values, true);
      if (!join_at_least_one) return false;
      have_done_table_set_join = true;
    }

    if (matching_table_iterators->second != name_table_map.end()) {
      // Join required on var2_name
      if (!have_done_table_set_join) {
        size_t table_idx = matching_table_iterators->second->second;
        return doTableSetJoin(table_idx, var1_name, var2_name,
                              constraint_values, false);
      } else {
        // The first join happened, so we have a newly joined table (1)
        // We now need to join (1) with the table we found for this
        // second variable
        size_t table1_idx = matching_table_iterators->first->second;
        size_t table2_idx = matching_table_iterators->second->second;
        return doTableTableJoin(table1_idx, table2_idx, var2_name);
      }
    }

    return true;
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

std::optional<pair<std::unordered_map<string, size_t>::iterator,
                   std::unordered_map<string, size_t>::iterator>>
ConstraintDatabase::getTableIdxForVars(const string& var1_name,
                                       const string& var2_name) {
  auto matching_table_idx_1 = name_table_map.find(var1_name);
  auto matching_table_idx_2 = name_table_map.find(var2_name);

  if (matching_table_idx_1 == name_table_map.end() &&
      matching_table_idx_2 == name_table_map.end()) {
    // No such table found
    return std::nullopt;
  } else {
    return pair{matching_table_idx_1, matching_table_idx_2};
  }
}

void ConstraintDatabase::createNewTable(
    const string& var_name, const SingleConstraintSet& constraint_values) {
  ConstraintTable table;
  table.initWithSingleVariable(var_name, constraint_values);
  size_t new_table_index = tables.size();
  tables.push_back(table);
  addVariableToTableMap(var_name, new_table_index);
}

void ConstraintDatabase::createNewTable(
    const string& var1_name, const string& var2_name,
    const PairedConstraintSet& constraint_values) {
  ConstraintTable table;
  table.initWithPairedVariables(var1_name, var2_name, constraint_values);
  size_t new_table_index = tables.size();
  tables.push_back(table);
  addVariableToTableMap(var1_name, new_table_index);
  addVariableToTableMap(var2_name, new_table_index);
}

bool ConstraintDatabase::doTableSetJoin(
    const size_t table_idx, const string& var1_name, const string& var2_name,
    const PairedConstraintSet& constraint_values, bool join_on_var1) {
  bool join_at_least_one = false;

  if (join_on_var1) {
    join_at_least_one = tables[table_idx].joinWithSetBy(
        var1_name, var2_name,
        Utils::getMapFromPairSet(constraint_values,
                                 Utils::MapFromPairSetting::MAP_LEFT_TO_RIGHT));
    if (join_at_least_one) {
      addVariableToTableMap(var2_name, table_idx);
    }
  } else {
    join_at_least_one = tables[table_idx].joinWithSetBy(
        var2_name, var1_name,
        Utils::getMapFromPairSet(constraint_values,
                                 Utils::MapFromPairSetting::MAP_RIGHT_TO_LEFT));
    if (join_at_least_one) {
      addVariableToTableMap(var1_name, table_idx);
    }
  }

  return join_at_least_one;
}

bool ConstraintDatabase::doTableTableJoin(const size_t table1_idx,
                                          const size_t table2_idx,
                                          const string& var_to_join) {
  auto& table1 = tables[table1_idx];
  auto& table2 = tables[table2_idx];
  // Check which table is smaller
  if (table1.size() < table2.size()) {
    auto successful_join = table1.joinWithTableBy(var_to_join, table2);
    removeTableFromDatabase(table2_idx);
    return successful_join;
  } else {
    auto successful_join = table2.joinWithTableBy(var_to_join, table1);
    removeTableFromDatabase(table1_idx);
    return successful_join;
  }
}

void ConstraintDatabase::addVariableToTableMap(const string var_name,
                                               size_t table_idx) {
  // This can fail (returned iterator is a pair, second value is success value)
  // during hash-join, since in the middle we have two tables with the same col
  // name. This will be resolved when after merging, one of the tables is
  // removed.
  name_table_map.insert({var_name, table_idx});
}

void ConstraintDatabase::removeTableFromDatabase(size_t table_idx) {
  // Remove old table
  tables.erase(tables.begin() + table_idx);

  // Refresh var -> table mapping
  name_table_map.clear();
  for (size_t i = 0; i < tables.size(); i++) {
    for (auto [var_name, col_idx] : tables[i].name_column_map) {
      name_table_map.insert({var_name, i});
    }
  }
}

//! Returns true if all synonyms have at least one valid value, else false
bool ConstraintDatabase::selectBoolean() {
  // Look through all tables: all must have at least 1 row
  for (const auto& table : tables) {
    if (table.size() == 0) return false;
  }
  return true;
}

vector<string> ConstraintDatabase::selectOne(const std::string var_to_select) {
  // Unique-ify the column and return only unique results
  auto one_column = selectOneColumn(var_to_select);
  unordered_set<string> unique_values(one_column.begin(), one_column.end());
  auto unique_vec = vector<string>(unique_values.begin(), unique_values.end());
  std::sort(unique_vec.begin(), unique_vec.end());
  return unique_vec;
}

SingleConstraintSet ConstraintDatabase::selectOneAsSet(
    const std::string var_to_select) {
  // Unique-ify the column and return only unique results
  auto one_column = selectOneColumn(var_to_select);
  SingleConstraintSet unique_values(one_column.begin(), one_column.end());
  return unique_values;
}

vector<string> ConstraintDatabase::selectOneColumn(
    const std::string var_to_select) {
  size_t table_idx = name_table_map[var_to_select];
  ConstraintTable& ctable = tables[table_idx];
  return ctable.getColumnByName(var_to_select);
}

vector<vector<string>> ConstraintDatabase::selectMultiple(
    const vector<string> vars_to_select) {
  // Go through each table and get a filtered version of each depending on the
  // columns selected inside - try to select all variables
  ConstraintTable existing_table;
  for (auto& table : tables) {
    ConstraintTable new_table = table.getSubTable(vars_to_select);
    if (new_table.size() == 0) continue;

    // Cross product with existing columns
    existing_table =
        ConstraintTable::cartesianProduct(existing_table, new_table);
  }

  // Get only the variables we want (in order) from final table
  vector<vector<string>> out_values;
  for (auto& row : existing_table.table) {
    vector<string> out_row;
    for (auto& var : vars_to_select) {
      size_t var_idx = existing_table.name_column_map[var];
      out_row.push_back(row[var_idx]);
    }
    out_values.push_back(out_row);
  }

  set<vector<string>> unique_values(out_values.begin(), out_values.end());
  return vector<vector<string>>(unique_values.begin(), unique_values.end());
}