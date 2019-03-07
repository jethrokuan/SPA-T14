#include "query_executor/constraint_solver/constraint_solver.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <string>
#include <vector>
#include "query_executor/constraint_solver/query_constraints.h"

//! Actually constrain the set of values and select the synonym indicated
std::vector<std::string> ConstraintSolver::constrainAndSelect(
    QueryConstraints& qc, const std::string toSelect) {
  std::map<std::string, std::set<std::string>> start_one_synonym_constraints,
      end_one_synonym_constraints;
  do {
    // Get individually allowed values from each of the tupled constraints
    start_one_synonym_constraints =
        intersectConstraints(qc.getSingleVariableConstraintMapRef(),
                             qc.getPairedVariableConstraintListRef());
    // Get all the pairs of values that are allowed
    std::map<std::pair<std::string, std::string>,
             std::set<std::pair<std::string, std::string>>>
        tupled_constraints =
            intersectTupledConstraints(qc.getPairedVariableConstraintListRef());

    filterQueryConstraints(start_one_synonym_constraints, tupled_constraints,
                           qc);

    // Re-constrain this set
    end_one_synonym_constraints =
        intersectConstraints(qc.getSingleVariableConstraintMapRef(),
                             qc.getPairedVariableConstraintListRef());
  } while (start_one_synonym_constraints != end_one_synonym_constraints);

  // Return intended variable
  auto set_to_return = end_one_synonym_constraints[toSelect];
  std::vector<std::string> result(set_to_return.begin(), set_to_return.end());
  return result;
}

std::map<std::string, std::set<std::string>>
ConstraintSolver::intersectConstraints(SingleVariableConstraintMap& svcm,
                                       PairedVariableConstraintList& pvcl) {
  std::map<std::string, std::set<std::string>> new_synonym_constraints;

  // Iterate through all variables that are constrained
  // Use these to construct an intersected set of constraints
  for (std::pair<std::string, SingleConstraintSet> single_var_constraints :
       svcm) {
    intersectTwoConstraints(new_synonym_constraints, single_var_constraints);
  }
  // Convert paired constraints into single constraints for single-intersection
  for (auto paired_var_constraints : pvcl) {
    auto [var1, var2] = paired_var_constraints.first;
    auto var1_set = getFirstsFromSet(paired_var_constraints.second);
    auto var2_set = getSecondsFromSet(paired_var_constraints.second);
    SingleVariableConstraints var1_svcm = {var1, var1_set};
    SingleVariableConstraints var2_svcm = {var2, var2_set};
    intersectTwoConstraints(new_synonym_constraints, var1_svcm);
    intersectTwoConstraints(new_synonym_constraints, var2_svcm);
  }

  return new_synonym_constraints;
}

void ConstraintSolver::intersectTwoConstraints(
    std::map<std::string, std::set<std::string>>& new_synonym_constraints,
    SingleVariableConstraints& incoming_constraint) {
  std::string syn = incoming_constraint.first;
  if (new_synonym_constraints.find(syn) == new_synonym_constraints.end()) {
    // This synonym has no existing constraints - add the set we found
    new_synonym_constraints[syn] = incoming_constraint.second;
  } else {
    // This synonym has an existing constraint - intersect with the set we found
    auto existing_constraints = new_synonym_constraints[syn];
    new_synonym_constraints[syn].clear();
    std::set_intersection(
        existing_constraints.begin(), existing_constraints.end(),
        incoming_constraint.second.begin(), incoming_constraint.second.end(),
        std::inserter(new_synonym_constraints[syn],
                      new_synonym_constraints[syn].begin()));
  }
}

std::map<std::pair<std::string, std::string>,
         std::set<std::pair<std::string, std::string>>>
ConstraintSolver::intersectTupledConstraints(
    PairedVariableConstraintList& pvcl) {
  std::map<std::pair<std::string, std::string>,
           std::set<std::pair<std::string, std::string>>>
      tupled_constraints;

  for (auto paired_var_constraints : pvcl) {
    auto [var1, var2] = paired_var_constraints.first;

    if (tupled_constraints.find({var1, var2}) == tupled_constraints.end()) {
      tupled_constraints[{var1, var2}] = paired_var_constraints.second;
    } else {
      auto existing_constraints = tupled_constraints[{var1, var2}];
      tupled_constraints[{var1, var2}].clear();
      std::set_intersection(
          existing_constraints.begin(), existing_constraints.end(),
          paired_var_constraints.second.begin(),
          paired_var_constraints.second.end(),
          std::inserter(tupled_constraints[{var1, var2}],
                        tupled_constraints[{var1, var2}].begin()));
    }
  }

  return tupled_constraints;
}

void ConstraintSolver::filterQueryConstraints(
    std::map<std::string, std::set<std::string>> one_synonym_constraints,
    std::map<std::pair<std::string, std::string>,
             std::set<std::pair<std::string, std::string>>>
        tupled_constraints,
    QueryConstraints& qc) {
  // Go through each QueryConstraint and remove values that aren't allowed
  // Filter single values first
  SingleVariableConstraintMap svcm;
  for (auto single_var_constraints : qc.getSingleVariableConstraintMapRef()) {
    SingleConstraintSet constraint_set;
    std::string var_name = single_var_constraints.first;
    for (auto single_constraint : single_var_constraints.second) {
      bool isPresent =
          one_synonym_constraints[var_name].find(single_constraint) !=
          one_synonym_constraints[var_name].end();
      if (isPresent) {
        constraint_set.insert(single_constraint);
      }
    }
    svcm[var_name] = constraint_set;
  }

  // Make sure every tupled constraint is present in individual and tupled sets
  PairedVariableConstraintList pvcl;
  for (auto paired_var_constraints : qc.getPairedVariableConstraintListRef()) {
    PairedConstraintSet constraint_set;
    auto [var1, var2] = paired_var_constraints.first;
    for (auto paired_constraint : paired_var_constraints.second) {
      bool var1_present =
          one_synonym_constraints[var1].find(paired_constraint.first) !=
          one_synonym_constraints[var1].end();
      bool var2_present =
          one_synonym_constraints[var2].find(paired_constraint.second) !=
          one_synonym_constraints[var2].end();
      bool pair_present =
          tupled_constraints[paired_var_constraints.first].find(
              paired_constraint) !=
          tupled_constraints[paired_var_constraints.first].end();

      if (var1_present && var2_present && pair_present) {
        constraint_set.insert(paired_constraint);
      }
    }
    pvcl.push_back({paired_var_constraints.first, constraint_set});
  }
  // Set these values back to the query constraints container
  qc.setSingleVariableConstraintMapRef(svcm);
  qc.setPairedVariableConstraintListRef(pvcl);
}

void ConstraintSolver::printConstraints(
    const std::map<std::string, std::set<std::string>> constraints) {
  for (auto m : constraints) {
    std::cout << "Intersected constraints for: " << m.first << std::endl;
    for (auto s : m.second) {
      std::cout << s << " ";
    }
    std::cout << "\n";
  }
}
