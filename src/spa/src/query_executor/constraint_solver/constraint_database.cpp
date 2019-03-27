#include "query_executor/constraint_solver/constraint_database.h"

//! Add the constraints for a single variable, e.g. a = {2, 3, 4}
void ConstraintDatabase::addToSingleVariableConstraints(
    const string& var_name, const SingleConstraintSet& constraint_values) {
  // Is an instance of this variable already stored?
  auto matching_table_idx = getTableIdxForVar(var_name);
  // If no table, create one (disjoint, so needs new table)
  if (!matching_table_idx) {
    createNewTable(var_name, constraint_values);
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
  auto matching_table_iterators = getTableIdxForVars(var1_name, var2_name);
  // If no table, create one (disjoint, so needs new table)
  if (!matching_table_iterators) {
    createNewTable(var1_name, var2_name, constraint_values);
  } else if ((matching_table_iterators->first != name_table_map.end()) &&
             (matching_table_iterators->second != name_table_map.end()) &&
             matching_table_iterators->first->second ==
                 matching_table_iterators->second->second) {
    // Exact match to same table, filter only
    size_t table_idx = matching_table_iterators->first->second;
    tables[table_idx].filterBy(var1_name, var2_name, constraint_values);
  } else {
    // 2 possibilities
    //  1. Only one variable is found, so only one join needs to be done
    //  2. Both variables are found in different tables.
    //     --> we have to do one join with table-set + one table-table join
    bool have_done_table_set_join = false;
    if (matching_table_iterators->first != name_table_map.end()) {
      // Join required on var1_name
      size_t table_idx = matching_table_iterators->first->second;
      doTableSetJoin(table_idx, var1_name, var2_name, constraint_values, true);
      have_done_table_set_join = true;
    }

    if (matching_table_iterators->second != name_table_map.end()) {
      // Join required on var2_name
      if (!have_done_table_set_join) {
        size_t table_idx = matching_table_iterators->second->second;
        doTableSetJoin(table_idx, var1_name, var2_name, constraint_values,
                       false);
      } else {
        // The first join happened, so we have a newly joined table (1)
        // We now need to join (1) with the table we found for this
        // second variable
        size_t table1_idx = matching_table_iterators->first->second;
        size_t table2_idx = matching_table_iterators->second->second;
        assert(false);
        // doTableTableJoin(table1_idx, table2_idx, var2_name);
      }
    }
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

void ConstraintDatabase::doTableSetJoin(
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
}

void ConstraintDatabase::doTableTableJoin(const size_t table1_idx,
                                          const size_t table2_idx,
                                          const string& var_to_join) {
  auto& table1 = tables[table1_idx];
  auto& table2 = tables[table2_idx];

  // Check which table is smaller
  if (table1.size() < table2.size()) {
    table1.joinWithTableBy(var_to_join, table2);
    removeTableFromDatabase(table2_idx);
  } else {
    table2.joinWithTableBy(var_to_join, table1);
    removeTableFromDatabase(table1_idx);
  }
}

void ConstraintDatabase::addVariableToTableMap(const string var_name,
                                               size_t table_idx) {
  auto insert_result = name_table_map.insert({var_name, table_idx});
  auto successful_insert = insert_result.second;
  if (!successful_insert) {
    if (tempMapping != std::nullopt) {
      std::cerr << "Potentially > 2 keys with the same name!";
      std::cerr << this;
      assert(false);
    }
    tempMapping = {var_name, table_idx};
  }
}

void ConstraintDatabase::removeTableFromDatabase(size_t table_idx) {
  // Remove all variables that are associated with this table
  ConstraintTable& ctable = tables[table_idx];
  for (const auto& [var_name, idx] : ctable.name_column_map) {
    name_table_map.erase(var_name);
  }

  // Insert the temp mapping that might have been stored during merging
  if (tempMapping) {
    addVariableToTableMap(tempMapping->first, tempMapping->second);
    tempMapping = std::nullopt;
  }
}