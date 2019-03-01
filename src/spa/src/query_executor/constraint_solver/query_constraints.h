#pragma once
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

//! Defines the allowed set of values for a given Query
class QueryConstraints {
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

 private:
  SingleVariableConstraintList singleVariableConstraintList;
  PairedVariableConstraintList pairedVariableConstraintList;

  template <class T>
  void swapElementsInSet(std::set<std::pair<T, T>> set) {
    for (auto& el : set) {
      swapElements(el);
    }
  }

  template <class T>
  void swapElements(std::pair<T, T>& pair) {
    T temp;
    temp = pair.first;
    pair.first = pair.second;
    pair.second = temp;
  }

 public:
  //! Add the constraints for a single variable, e.g. a = {2, 3, 4}
  void addToSingleVariableConstraints(std::string var_name,
                                      std::set<std::string> constraint_values);
  //! Add paired constraints for 2 vars, e.g. (a, v) = {(2, 3), (3, 4), (1, 2)}
  void addToPairedVariableConstraints(
      std::string var1_name, std::string var2_name,
      std::set<std::pair<std::string, std::string>> constraint_values);
  // TODO: This must be internally sorted - otherwise can get (a,v)/(v,a)
};