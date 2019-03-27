#pragma once
#include <cassert>
#include <iostream>
#include <iterator>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "query_executor/constraint_solver/constraint_table.h"
#include "utils/utils.h"

using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using namespace std::string_literals;  // for "asdasd"s: const char* -->
                                       // std::string

/*
 *
 *  ConstraintDatabase invariants:
 *  1. Each synoynm can only be in ONE table at a time
 *  2. This is enforced by joining every time new variables are introduced
 *
 *  What this allows:
 *  1. We don't have to do joins until no more joins are possible at the end
 *  2. No need to map dependencies
 *
 */

class ConstraintDatabase {
 private:
  //! All tables representing constraints for this query
  vector<ConstraintTable> tables;
  //! Given a synonym, which table is it in?
  unordered_map<string, size_t> name_table_map;

  //! Temporary variable to hold a (variable -> idx) mapping during hash joins
  std::optional<pair<string, size_t>> tempMapping = std::nullopt;

  //! Get a reference to the table that contains this var already
  std::optional<size_t> getTableIdxForVar(const string& var_name);

  //! Get a reference to the table that contains these two vars already
  std::optional<pair<std::unordered_map<string, size_t>::iterator,
                     std::unordered_map<string, size_t>::iterator>>
  getTableIdxForVars(const string& var1_name, const string& var2_name);

  void createNewTable(const std::string& var_name,
                      const SingleConstraintSet& constraint_values);

  void createNewTable(const std::string& var1_name,
                      const std::string& var2_name,
                      const PairedConstraintSet& constraint_values);

  //! Essentially Hash-Join where the incoming hashmap is easy to compute
  void doTableSetJoin(const size_t table_idx, const std::string& var1_name,
                      const std::string& var2_name,
                      const PairedConstraintSet& constraint_values,
                      bool join_on_var1);

  //! Hash-Join algorithm to join two tables along a key
  void doTableTableJoin(const size_t table1_idx, const size_t table2_idx,
                        const string& var_to_join);

  void removeTableFromDatabase(size_t table_idx);

  //! Associate a variable name with a table index. If the variable name already
  //! exists, this is likely going to be a hash-join, so we need to save the
  //! variable we were going to add
  void addVariableToTableMap(const string var_name, size_t table_idx);

  //! Returns the column containing the variable of interest
  vector<string> selectOneColumn(const std::string var_to_select);

 public:
  //! Add the constraints for a single variable, e.g. a = {2, 3, 4}
  void addToSingleVariableConstraints(
      const std::string& var_name,
      const SingleConstraintSet& constraint_values);
  //! Add paired constraints for 2 vars, e.g. (a, v) = {(2, 3), (3, 4), (1, 2)}
  void addToPairedVariableConstraints(
      const std::string& var1_name, const std::string& var2_name,
      const PairedConstraintSet& constraint_values);

  //! Returns true if all synonyms have at least one valid value, else false
  bool selectBoolean();
  //! Returns the column containing the variable of interest
  vector<string> selectOne(const std::string var_to_select);
  //! Returns all columns containing the variables of interests in order
  vector<vector<string>> selectMultiple(const vector<string> vars_to_select);

  friend std::ostream& operator<<(std::ostream& os,
                                  ConstraintDatabase const& db) {
    os << "------------------------------------------------------------\n"s;
    os << "DB: Synonym to table idx mappings\n"s;
    // Print out the table headers
    for (auto [key, val] : db.name_table_map) {
      os << key << " => table index "s << std::to_string(val) << "\n"s;
    }
    os << "------------------------------------------------------------\n"s;

    os << "\n---------------------------------\n"s;
    os << "DB: Tables: \n"s;
    // Print out the table headers
    for (const auto& table : db.tables) {
      os << table << "\n\n"s;
    }
    os << "---------------------------------\n"s;

    return os;
  }
};