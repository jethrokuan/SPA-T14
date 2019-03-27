#include "query_executor/constraint_solver/constraint_database.h"

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
  auto matching_table_iterators = getTableIdxForVars(var1_name, var2_name);

  // If no table, create one (disjoint, so needs new table)
  if (!matching_table_iterators) {
    ConstraintTable table;
    table.initWithPairedVariables(var1_name, var2_name, constraint_values);
    size_t new_table_index = tables.size();
    tables.push_back(table);
    name_table_map.insert({var1_name, new_table_index});
    name_table_map.insert({var2_name, new_table_index});
  } else if ((matching_table_iterators->first != name_table_map.end()) &&
             (matching_table_iterators->second != name_table_map.end()) &&
             matching_table_iterators->first->second ==
                 matching_table_iterators->second->second) {
    // Exact match to same table, filter only
    // -> first gets the matched iterator for first var, ->second gets the value
    // (size_t index)
    tables[matching_table_iterators->first->second].filterBy(
        var1_name, var2_name, constraint_values);
  } else {
    // 2 possibilities
    //  1. Only one variable is found, so only one join needs to be done
    //  2. Both variables are found in different tables.
    //     --> we have to do one join with table-set + one table-table join
    bool have_done_table_set_join = false;
    if (matching_table_iterators->first != name_table_map.end()) {
      // Merge required on var1_name
      size_t table_idx = matching_table_iterators->first->second;
      bool join_at_least_one = tables[table_idx].joinBy(
          var1_name, var2_name,
          Utils::getMapFromPairSet(
              constraint_values, Utils::MapFromPairSetting::MAP_LEFT_TO_RIGHT));
      have_done_table_set_join = true;
      if (join_at_least_one) {
        name_table_map.insert({var2_name, table_idx});
      }
    }

    if (matching_table_iterators->second != name_table_map.end()) {
      // Merge required on var2_name
      if (!have_done_table_set_join) {
        size_t table_idx = matching_table_iterators->second->second;
        bool join_at_least_one = tables[table_idx].joinBy(
            var2_name, var1_name,
            Utils::getMapFromPairSet(
                constraint_values,
                Utils::MapFromPairSetting::MAP_RIGHT_TO_LEFT));
        have_done_table_set_join = true;
        if (join_at_least_one) {
          name_table_map.insert({var1_name, table_idx});
        }
      } else {
        assert(false);
      }
    }
    /*
        if (matching_table_iterators->second != name_table_map.end()) {
          // Either need to join directly (table-set) or do table-table join
          if (!have_done_table_set_join) {
            tables[matching_table_iterators->second->second].joinBy(
                var2_name, var1_name,
                Utils::getMapFromPairSet(
                    constraint_values,
                    Utils::MapFromPairSetting::MAP_RIGHT_TO_LEFT));
          } else {
            // Needs table-table join
            assert(false);
          }

  }
  */
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
