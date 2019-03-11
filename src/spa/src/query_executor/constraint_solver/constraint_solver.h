#pragma once
#include <optional>

#include <string>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/constraint_solver/query_constraints.h"

using namespace QE;
class ConstraintSolver {
 private:
  template <class T, class H>
  static std::unordered_set<T> getFirstsFromSet(
      std::unordered_set<std::pair<T, T>, H>& s) {
    std::unordered_set<T> result;
    for (auto el : s) {
      result.insert(el.first);
    }
    return result;
  }

  template <class T, class H>
  static std::unordered_set<T> getSecondsFromSet(
      std::unordered_set<std::pair<T, T>, H>& s) {
    std::unordered_set<T> result;
    for (auto el : s) {
      result.insert(el.second);
    }
    return result;
  };

  //! Get a map of constraints from single variables to allowed values
  static std::unordered_map<std::string, std::unordered_set<std::string>>
  intersectSingleVarConstraints(SingleVariableConstraintMap& svcm,
                                PairedVariableConstraintList& pvcl);

  //! Intersects the current list of single constraints with a new constraint
  static void intersectTwoConstraints(
      std::unordered_map<std::string, std::unordered_set<std::string>>&
          new_synonym_constraints,
      SingleVariableConstraints& incoming_constraint);

  //! Get a map of all constraints from paired variables to their allowed values
  static std::unordered_map<
      std::pair<std::string, std::string>,
      std::unordered_set<std::pair<std::string, std::string>, Utils::pair_hash>,
      Utils::pair_hash>
  intersectPairedVarConstraints(PairedVariableConstraintList& pvcl);

  //! Filters a QueryConstraints object based on calculated constraints
  static void filterQueryConstraints(
      std::unordered_map<std::string, std::unordered_set<std::string>>
          one_synonym_constraints,
      std::unordered_map<std::pair<std::string, std::string>,
                         std::unordered_set<std::pair<std::string, std::string>,
                                            Utils::pair_hash>,
                         Utils::pair_hash>,
      QueryConstraints& qc);

  static void printConstraints(
      const std::unordered_map<std::string, std::unordered_set<std::string>>);

 public:
  //! Special case of constrainAndSelect - select only one variable
  static std::vector<std::string> constrainAndSelect(
      QueryConstraints& qc, const std::string toSelect);
  //! \brief Intersect and resolve constraints and select allowed set of values
  //! for multiple variables from the results: returns one vector for each var
  //! in order - if vars_to_select = {x, y, z}, result will be in that order
  static std::vector<std::vector<std::string>> constrainAndSelect(
      QueryConstraints& qc, const std::vector<std::string> vars_to_select);
};
