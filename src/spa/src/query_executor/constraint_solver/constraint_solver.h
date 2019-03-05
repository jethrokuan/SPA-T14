#pragma once
#include <optional>
#include <set>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/constraint_solver/query_constraints.h"

using namespace QE;
class ConstraintSolver {
 private:
  template <class T>
  static std::set<T> getFirstsFromSet(std::set<std::pair<T, T>>& s) {
    std::set<std::string> result;
    for (auto el : s) {
      result.insert(el.first);
    }
    return result;
  }

  template <class T>
  static std::set<T> getSecondsFromSet(std::set<std::pair<T, T>>& s) {
    std::set<std::string> result;
    for (auto el : s) {
      result.insert(el.second);
    }
    return result;
  };

  //! Get a map of constraints from single variables to allowed values
  static std::map<std::string, std::set<std::string>> intersectConstraints(
      SingleVariableConstraintList& svcl, PairedVariableConstraintList& pvcl);

  //! Intersects the current list of single constraints with a new constraint
  static void intersectTwoConstraints(
      std::map<std::string, std::set<std::string>>& new_synonym_constraints,
      SingleVariableConstraints& incoming_constraint);

  //! Get a map of all constraints from paired variables to their allowed values
  static std::map<std::pair<std::string, std::string>,
                  std::set<std::pair<std::string, std::string>>>
  intersectTupledConstraints(PairedVariableConstraintList& pvcl);

  //! Filters a QueryConstraints object based on calculated constraints
  static void filterQueryConstraints(
      std::map<std::string, std::set<std::string>> one_synonym_constraints,
      std::map<std::pair<std::string, std::string>,
               std::set<std::pair<std::string, std::string>>>,
      QueryConstraints& qc);

  static void printConstraints(std::map<std::string, std::set<std::string>>);

 public:
  //! Actually constrain the set of values and select the synonym indicated
  static std::vector<std::string> constrainAndSelect(QueryConstraints& qc,
                                                     std::string toSelect);
};
