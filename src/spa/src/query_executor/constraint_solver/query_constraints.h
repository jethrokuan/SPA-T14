#pragma once
#include <algorithm>
#include <map>
#include <optional>

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include "utils/utils.h"

//! Part of a constraint set for a variable, e.g. ("x")
using SingleConstraint = std::string;
//! Part of a constraint set for a pair of variable, e.g.(1, "x")
using PairedConstraint = std::pair<std::string, std::string>;
//! Representing two variables tied together
using PairedVariables = std::pair<std::string, std::string>;

//! Set of possible values a variable can take
using SingleConstraintSet = std::unordered_set<SingleConstraint>;
//! Set of possible values a pair of variables can take
using PairedConstraintSet =
    std::unordered_set<PairedConstraint, Utils::pair_hash>;

//! A specific set of constraints tied to a variable name e.g. a->{1, 2}
using SingleVariableConstraints = std::pair<std::string, SingleConstraintSet>;
//! A specific set of constraints tied to a variable pair e.g. (a, b)->{(1,2)}
using PairedVariableConstraints =
    std::pair<PairedVariables, PairedConstraintSet>;

// NOTE: These are not necessarily in a "solved state".
//! Aggregation of multiple constraints on paired variables
using PairedVariableConstraintList = std::vector<PairedVariableConstraints>;

using SingleVariableConstraintMap = std::map<std::string, SingleConstraintSet>;

//! Defines the allowed set of values for a given Query
class QueryConstraints {
 private:
  SingleVariableConstraintMap allPossibleValues;
  SingleVariableConstraintMap singleVariableConstraintMap;
  PairedVariableConstraintList pairedVariableConstraintList;

  template <class T>
  std::unordered_set<std::pair<T, T>, Utils::pair_hash> swapElementsInSet(
      std::unordered_set<std::pair<T, T>, Utils::pair_hash> s) {
    std::unordered_set<std::pair<T, T>, Utils::pair_hash> outSet;
    for (std::pair<T, T> el : s) {
      swapElements(outSet, el);
    }
    return outSet;
  }

  template <class T>
  void swapElements(
      std::unordered_set<std::pair<T, T>, Utils::pair_hash>& outSet,
      std::pair<T, T> pair) {
    outSet.insert({pair.second, pair.first});
  }

 public:
  //! \brief Add to the list of all possible values this variable can take
  //! If the variable already exists in the set, the existing set is
  //! intersected with the incoming set of constraints
  void addToAllPossibleValues(std::string var_name,
                              SingleConstraintSet constraint_values);
  //! Add the constraints for a single variable, e.g. a = {2, 3, 4}
  void addToSingleVariableConstraints(std::string var_name,
                                      SingleConstraintSet constraint_values);
  //! Add paired constraints for 2 vars, e.g. (a, v) = {(2, 3), (3, 4), (1, 2)}
  void addToPairedVariableConstraints(std::string var1_name,
                                      std::string var2_name,
                                      PairedConstraintSet constraint_values);

  void addToSingleVariableConstraints(
      std::string var_name, std::vector<std::string> constraint_values) {
    addToSingleVariableConstraints(
        var_name, std::unordered_set<std::string>(constraint_values.begin(),
                                                  constraint_values.end()));
  }

  void addToAllPossibleValues(std::string var_name,
                              std::vector<std::string> constraint_values) {
    addToAllPossibleValues(
        var_name, std::unordered_set<std::string>(constraint_values.begin(),
                                                  constraint_values.end()));
  }

  SingleVariableConstraintMap& getSingleVariableConstraintMapRef() {
    return singleVariableConstraintMap;
  }

  PairedVariableConstraintList& getPairedVariableConstraintListRef() {
    return pairedVariableConstraintList;
  }

  void setSingleVariableConstraintMapRef(SingleVariableConstraintMap svcm) {
    singleVariableConstraintMap = svcm;
  }

  void setPairedVariableConstraintListRef(PairedVariableConstraintList pvcl) {
    pairedVariableConstraintList = pvcl;
  }

  //! Checks if a particular variable is already inside the SVCL
  bool isVarInAllPossibleValues(const std::string& var_name) {
    // This function is necessary because we don't want to re-add an entire set
    // of possible values a variable can take if we have already done so before
    return allPossibleValues.find(var_name) != allPossibleValues.end();
  }

  //! Checks if a particular variable is already inside the SVCL
  bool isVarInAllPossibleValues(const char* var_name) {
    return isVarInAllPossibleValues(std::string(var_name));
  }

  //! \brief Checks if a vector of constraint values has at least one result in
  //! the domain of all possible values that this variable can take
  bool containsNoAllowedResults(
      const std::vector<std::string> constraint_values,
      const std::string var_name);
  bool containsNoAllowedResults(const SingleConstraintSet constraint_values,
                                const std::string var_name);

  friend std::ostream& operator<<(std::ostream& os,
                                  QueryConstraints const& qc) {
    os << std::string("Printing QueryConstraints:\n");
    os << std::string("Printing allPossibleValues:\n");
    for (const auto& el : qc.allPossibleValues) {
      os << el.first << std::string(": {");
      for (const auto& el2 : el.second) {
        os << el2 << std::string(",");
      }
      os << "}\n";
    }
    os << std::string("Printing singleVariableConstraints:\n");
    for (const SingleVariableConstraints& el : qc.singleVariableConstraintMap) {
      os << el.first << std::string(": {");
      for (const auto& el2 : el.second) {
        os << el2 << std::string(",");
      }
      os << "}\n";
    }
    os << std::string("Printing pairedVariableConstraints:\n");
    for (const PairedVariableConstraints& el :
         qc.pairedVariableConstraintList) {
      os << std::string("(") << el.first.first << std::string(",")
         << el.first.second << std::string("): {");
      for (const auto& el2 : el.second) {
        os << std::string("(") << el2.first << std::string(",") << el2.second
           << std::string("),");
      }
      os << "}\n";
    }
    return os;
  }
};