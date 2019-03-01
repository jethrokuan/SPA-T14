#pragma once
#include <optional>
#include <set>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"

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
using SingleVariableConstraints = std::map<std::string, std::set<std::string>>;
//! Map between variable name and constrained values it can take
using TupledConstraints = std::map<SynonymPair, AllowedValuePairSet>;

class ConstraintSolver {
 private:
  static std::set<std::string> getFirstsFromSet(AllowedValuePairSet&);
  static std::set<std::string> getSecondsFromSet(AllowedValuePairSet&);
  //! Find the intersection between all stated constraints by synonyms
  static SingleVariableConstraints intersectConstraints(
      std::vector<TupledConstraint>);

  static void intersectTwoConstraints(SingleVariableConstraints&, std::string&,
                                      std::set<std::string>);

  static std::vector<TupledConstraint> filterAllowedValues(
      SingleVariableConstraints& synonym_constraints,
      TupledConstraints& tupled_constraints,
      std::vector<TupledConstraint>& allowedValues);

  static AllowedValuePairSet filterAllowedValuePair(
      SingleVariableConstraints& synonym_constraints,
      TupledConstraints& tupled_constraints,
      TupledConstraint& allowedValuePair);

  static void printConstraints(SingleVariableConstraints);

 public:
  //! Placeholder for cases where a second synonym or value is not needed
  static const std::string DUMMY_SYNONYM;

  //! Get an empty constraint set
  static TupledConstraint makeEmptyAllowedValuesPair() {
    return std::make_pair(std::make_pair(DUMMY_SYNONYM, DUMMY_SYNONYM),
                          std::set<AllowedValuePair>());
  }

  //! Get an empty constraint set that's tied to a particular synonym
  static TupledConstraint makeEmptyAllowedValuesPairForSynonym(Synonym& s) {
    return std::make_pair(std::make_pair(s.synonym, DUMMY_SYNONYM),
                          std::set<AllowedValuePair>());
  }

  //! Get an empty constraint set that's tied to a particular synonym
  static TupledConstraint makeEmptyAllowedValuesPairForSynonyms(Synonym& s1,
                                                                Synonym& s2) {
    return std::make_pair(std::make_pair(s1.synonym, s2.synonym),
                          std::set<AllowedValuePair>());
  }

  //! Version that does not require a Synonym object, just the string
  static TupledConstraint makeAllowedValues(std::string& syn,
                                            std::vector<std::string>& vals);

  //! Creates a list of allowed values for a single synonym
  static TupledConstraint makeAllowedValues(Synonym&,
                                            std::vector<std::string>&);

  static TupledConstraint makeAllowedValues(Synonym&, Synonym&,
                                            AllowedValuePairSet&);

  static TupledConstraints intersectTupledConstraints(
      std::vector<TupledConstraint>);

  static std::string getStringPairAsString(std::pair<std::string, std::string>);
  static void printAllowedValuesPair(TupledConstraint&);

  //! Actually constrain the set of values and select the synonym indicated
  static std::vector<std::string> constrainAndSelect(
      std::vector<TupledConstraint> allowedValues, std::string toSelect);
};
