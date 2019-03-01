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
using AllowedValue = std::pair<std::string, std::string>;
//! Set of all allowed values for a synonym pair
using AllowedValueSet = std::set<AllowedValue>;
//! Describes a pair of synonyms and all of their allowed values
using AllowedValuesPair = std::pair<SynonymPair, AllowedValueSet>;
//! Describes all found pairs of synoynms and their allowed values
using AllowedValuesList = std::vector<AllowedValuesPair>;
//! Map between variable name and constrained values it can take
using SingleVariableConstraints = std::map<std::string, std::set<std::string>>;
//! Map between variable name and constrained values it can take
using TupledConstraints = std::map<SynonymPair, AllowedValueSet>;

class ConstraintSolver {
 private:
  static std::set<std::string> getFirstsFromSet(AllowedValueSet&);
  static std::set<std::string> getSecondsFromSet(AllowedValueSet&);
  //! Find the intersection between all stated constraints by synonyms
  static SingleVariableConstraints intersectConstraints(
      std::vector<AllowedValuesPair>);

  static void intersectTwoConstraints(SingleVariableConstraints&, std::string&,
                                      std::set<std::string>);

  static std::vector<AllowedValuesPair> filterAllowedValues(
      SingleVariableConstraints& synonym_constraints,
      TupledConstraints& tupled_constraints,
      std::vector<AllowedValuesPair>& allowedValues);

  static AllowedValueSet filterAllowedValuePair(
      SingleVariableConstraints& synonym_constraints,
      TupledConstraints& tupled_constraints,
      AllowedValuesPair& allowedValuePair);

  static void printConstraints(SingleVariableConstraints);

 public:
  //! Placeholder for cases where a second synonym or value is not needed
  static const std::string DUMMY_SYNONYM;

  //! Get an empty constraint set
  static AllowedValuesPair makeEmptyAllowedValuesPair() {
    return std::make_pair(std::make_pair(DUMMY_SYNONYM, DUMMY_SYNONYM),
                          std::set<AllowedValue>());
  }

  //! Get an empty constraint set that's tied to a particular synonym
  static AllowedValuesPair makeEmptyAllowedValuesPairForSynonym(Synonym& s) {
    return std::make_pair(std::make_pair(s.synonym, DUMMY_SYNONYM),
                          std::set<AllowedValue>());
  }

  //! Get an empty constraint set that's tied to a particular synonym
  static AllowedValuesPair makeEmptyAllowedValuesPairForSynonyms(Synonym& s1,
                                                                 Synonym& s2) {
    return std::make_pair(std::make_pair(s1.synonym, s2.synonym),
                          std::set<AllowedValue>());
  }

  //! Version that does not require a Synonym object, just the string
  static AllowedValuesPair makeAllowedValues(std::string& syn,
                                             std::vector<std::string>& vals);

  //! Creates a list of allowed values for a single synonym
  static AllowedValuesPair makeAllowedValues(Synonym&,
                                             std::vector<std::string>&);

  static AllowedValuesPair makeAllowedValues(Synonym&, Synonym&,
                                             AllowedValueSet&);

  static TupledConstraints intersectTupledConstraints(
      std::vector<AllowedValuesPair>);

  static std::string getStringPairAsString(std::pair<std::string, std::string>);
  static void printAllowedValuesPair(AllowedValuesPair&);

  //! Actually constrain the set of values and select the synonym indicated
  static std::vector<std::string> constrainAndSelect(
      std::vector<AllowedValuesPair> allowedValues, std::string toSelect);
};
