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
                                       // std::strin

//! Part of a constraint set for a variable, e.g. ("x")
using SingleConstraint = string;
//! Set of possible values a variable can take
using SingleConstraintSet = std::unordered_set<SingleConstraint>;
//! Part of a constraint set for a pair of variable, e.g.(1, "x")
using PairedConstraint = std::pair<string, string>;
//! Set of possible values a pair of variables can take
using PairedConstraintSet =
    std::unordered_set<PairedConstraint, Utils::pair_hash>;

class ConstraintTable {
 private:
  size_t next_column_idx = 0;

 public:
  //! Each vector is a ROW in the table. Public needed for iteration for select
  vector<vector<string>> table;
  //! Given a synonym, which column index is it in? Public needed for
  //! deletion
  unordered_map<string, size_t> name_column_map;

  //! Get a single column of values from the table based on the column name
  vector<string> getColumnByName(const string& name);
  //! Initializes the table with a single variable's constraints
  void initWithSingleVariable(const string& var_name,
                              const SingleConstraintSet& constraint_values);
  //! Initializes the table with a paired variable constraint set
  void initWithPairedVariables(const string& var1_name, const string& var2_name,
                               const PairedConstraintSet& constraint_values);

  //! var_name is a column in this table. Filter the table by these constraint
  //! values
  void filterBy(const string& var_name,
                const SingleConstraintSet& constraint_values);

  void filterBy(const string& var1_name, const string& var2_name,
                const PairedConstraintSet& constraint_values);

  //! Join an existing table and an incoming constraint set on a variable
  bool joinWithSetBy(const string& var_to_join, const string& other_var,
                     const unordered_map<string, vector<string>>& pair_map);

  //! Join an existing table with another table on a variable
  bool joinWithTableBy(const string& var_to_join,
                       const ConstraintTable& other_table);

  //! Hashing step of Hash-Join algorithm (hash a column's values to its idxs)
  unordered_map<string, vector<size_t>> hashColumn(const string& var_to_join);

  //! Adds a new column-name association to the table
  void addNewColumnName(const string& column_name);

  //! Each vector is one row from this table that has been selected
  ConstraintTable getSubTable(const vector<string>& vars_to_select);

  //! Cartesian product of two tables: t1.size() x t2.size() rows as output
  static ConstraintTable cartesianProduct(ConstraintTable& t1,
                                          ConstraintTable& t2);

  inline size_t size() const { return table.size(); }

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