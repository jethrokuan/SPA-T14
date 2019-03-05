#pragma once
#include <algorithm>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <vector>

//! Part of a constraint set for a variable, e.g. ("x")
using SingleConstraint = std::string;
//! Part of a constraint set for a pair of variable, e.g.(1, "x")
using PairedConstraint = std::pair<std::string, std::string>;
//! Representing two variables tied together
using PairedVariables = std::pair<std::string, std::string>;

//! Set of possible values a variable can take
using SingleConstraintSet = std::set<SingleConstraint>;
//! Set of possible values a pair of variables can take
using PairedConstraintSet = std::set<PairedConstraint>;

//! A specific set of constraints tied to a variable name e.g. a->{1, 2}
using SingleVariableConstraints = std::pair<std::string, SingleConstraintSet>;
//! A specific set of constraints tied to a variable pair e.g. (a, b)->{(1,2)}
using PairedVariableConstraints =
    std::pair<PairedVariables, PairedConstraintSet>;

// NOTE: These two lists are not necessarily in a "solved state".
//! Aggregation of multiple constraints on single variables
using SingleVariableConstraintList = std::vector<SingleVariableConstraints>;
//! Aggregation of multiple constraints on paired variables
using PairedVariableConstraintList = std::vector<PairedVariableConstraints>;

//! Defines the allowed set of values for a given Query
class QueryConstraints {
 private:
  SingleVariableConstraintList singleVariableConstraintList;
  PairedVariableConstraintList pairedVariableConstraintList;

  template <class T>
  std::set<std::pair<T, T>> swapElementsInSet(std::set<std::pair<T, T>> s) {
    std::set<std::pair<T, T>> outSet;
    for (std::pair<T, T> el : s) {
      swapElements(outSet, el);
    }
    return outSet;
  }

  template <class T>
  void swapElements(std::set<std::pair<T, T>>& outSet, std::pair<T, T> pair) {
    outSet.insert({pair.second, pair.first});
  }

 public:
  //! Add the constraints for a single variable, e.g. a = {2, 3, 4}
  void addToSingleVariableConstraints(std::string var_name,
                                      std::set<std::string> constraint_values);
  //! Add paired constraints for 2 vars, e.g. (a, v) = {(2, 3), (3, 4), (1, 2)}
  void addToPairedVariableConstraints(
      std::string var1_name, std::string var2_name,
      std::set<std::pair<std::string, std::string>> constraint_values);

  void addToSingleVariableConstraints(
      std::string var_name, std::vector<std::string> constraint_values) {
    addToSingleVariableConstraints(
        var_name, std::set<std::string>(constraint_values.begin(),
                                        constraint_values.end()));
  }

  SingleVariableConstraintList& getSingleVariableConstraintListRef() {
    return singleVariableConstraintList;
  }

  PairedVariableConstraintList& getPairedVariableConstraintListRef() {
    return pairedVariableConstraintList;
  }

  void setSingleVariableConstraintListRef(SingleVariableConstraintList svcl) {
    singleVariableConstraintList = svcl;
  }

  void setPairedVariableConstraintListRef(PairedVariableConstraintList pvcl) {
    pairedVariableConstraintList = pvcl;
  }

  //! Checks if a particular variable is already inside the SVCL
  bool isVarInSingleConstraintList(const std::string& var_name) {
    // This function is necessary because we don't want to re-add an entire set
    // of possible values a variable can take if we have already done so before
    return std::find_if(singleVariableConstraintList.begin(),
                        singleVariableConstraintList.end(), [&](auto svc) {
                          return svc.first == var_name;
                        }) != singleVariableConstraintList.end();
  }

  //! Checks if a particular variable is already inside the SVCL
  bool isVarInSingleConstraintList(const char* var_name) {
    return isVarInSingleConstraintList(std::string(var_name));
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  QueryConstraints const& qc) {
    os << std::string("Printing QueryConstraints:\n");
    for (const SingleVariableConstraints& el :
         qc.singleVariableConstraintList) {
      os << el.first << std::string(": {");
      for (const auto& el2 : el.second) {
        os << el2 << std::string(",");
      }
      os << "}\n";
    }
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