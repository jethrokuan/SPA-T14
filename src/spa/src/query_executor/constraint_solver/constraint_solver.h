#pragma once
#include <optional>
#include <set>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/constraint_solver/query_constraints.h"

using namespace QE;

//! Describes two synoynms (or one synonym and a dummy) that are constrained
using SynonymPair = std::pair<std::string, std::string>;
//! One allowed pair of values
using AllowedValuePair = std::pair<std::string, std::string>;
//! Set of all allowed values for a synonym pair
using AllowedValuePairSet = std::set<AllowedValuePair>;
//! Describes a pair of synonyms and all of their allowed values
using TupledConstraint = std::pair<SynonymPair, AllowedValuePairSet>;
//! Describes all found pairs of synoynms and their allowed values
using AllowedValuesList = std::vector<TupledConstraint>;
//! Map between variable name and constrained values it can take
// using SingleVariableConstraints = std::map<std::string,
// std::set<std::string>>;
//! Map between variable name and constrained values it can take
using TupledConstraints = std::map<SynonymPair, AllowedValuePairSet>;

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

  //! Find the intersection between all stated constraints by synonyms
  static std::map<std::string, std::set<std::string>> intersectConstraints(
      SingleVariableConstraintList& svcl, PairedVariableConstraintList& pvcl);

  static void intersectTwoConstraints(
      std::map<std::string, std::set<std::string>>& new_synonym_constraints,
      SingleVariableConstraints& incoming_constraint);

  static TupledConstraints intersectTupledConstraints(
      PairedVariableConstraintList& pvcl);

  static void filterQueryConstraints(
      std::map<std::string, std::set<std::string>> one_synonym_constraints,
      std::map<std::pair<std::string, std::string>,
               std::set<std::pair<std::string, std::string>>>,
      QueryConstraints& qc);

 public:
  //! Actually constrain the set of values and select the synonym indicated
  static std::vector<std::string> constrainAndSelect(QueryConstraints& qc,
                                                     std::string toSelect);
};
