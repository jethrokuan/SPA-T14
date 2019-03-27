#pragma once
#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "utils/utils.h"

using std::string;
using std::unordered_map;
using std::vector;
using namespace std::string_literals;  // for "asdasd"s: const char* -->
                                       // std::string

//! Part of a constraint set for a variable, e.g. ("x")
using SingleConstraint = std::string;
//! Set of possible values a variable can take
using SingleConstraintSet = std::unordered_set<SingleConstraint>;
//! Part of a constraint set for a pair of variable, e.g.(1, "x")
using PairedConstraint = std::pair<std::string, std::string>;
//! Set of possible values a pair of variables can take
using PairedConstraintSet =
    std::unordered_set<PairedConstraint, Utils::pair_hash>;

class ConstraintTable {
 private:
  //! Each vector is a ROW in the table
  vector<vector<string>> table;
  //! Given a synonym, which column index is it in?
  unordered_map<string, size_t> name_column_map;

 public:
  //! Get a single column of values from the table based on the column name
  vector<string> getColumnByName(const string& name);
  //! Initializes the table with a single variable's constraints
  void initWithSingleVariable(const std::string& var_name,
                              const SingleConstraintSet& constraint_values);
  //! Initializes the table with a paired variable constraint set
  void initWithPairedVariables(const std::string& var1_name,
                               const std::string& var2_name,
                               const PairedConstraintSet& constraint_values);
  //! Check if this table can be merged with another, else they are disjoint
  //! i.e. they have synonyms in common
  bool canMergeWith(ConstraintTable& constraintTable);

  friend std::ostream& operator<<(std::ostream& os,
                                  ConstraintTable const& ctable) {
    // Print out the table headers
    os << "\n*************************\n"s;
    os << "Columns for this table: \n"s;
    for (auto [key, val] : ctable.name_column_map) {
      os << key << " => column idx "s << std::to_string(val) << "\n"s;
    }
    os << "*************************\n"s;

    // Print out the table headers
    for (const auto& row : ctable.table) {
      for (const auto& val : row) {
        os << val << "\t"s;
      }
      os << "\n"s;
    }

    return os;
  }
};

class ConstraintDatabase {
 private:
  //! All tables representing constraints for this query
  vector<ConstraintTable> tables;
  //! Given a synonym, which table is it in?
  unordered_map<string, size_t> name_table_map;

  //! Get a reference to the table that contains this var already
  std::optional<size_t> getTableIdxForVar(const string& var_name);

  //! Get a reference to the table that contains these two vars already
  std::optional<size_t> getTableIdxForVars(const string& var1_name,
                                           const string& var2_name);

 public:
  //! Add the constraints for a single variable, e.g. a = {2, 3, 4}
  void addToSingleVariableConstraints(
      const std::string& var_name,
      const SingleConstraintSet& constraint_values);
  //! Add paired constraints for 2 vars, e.g. (a, v) = {(2, 3), (3, 4), (1, 2)}
  void addToPairedVariableConstraints(
      const std::string& var1_name, const std::string& var2_name,
      const PairedConstraintSet& constraint_values);

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