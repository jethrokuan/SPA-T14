#pragma once
#include <optional>
#include <set>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/pql/pql.h"

using namespace QE;

//! Describes two synoynms (or one synonym and a dummy) that are constrained
using SynonymPair = std::pair<std::string, std::string>;
//! One allowed pair of values
using AllowedValue = std::pair<std::string, std::string>;
//! Set of all allowed values for a synonym pair
using AllowedValueSet = std::set<AllowedValue>;
//! Describes a pair of synonyms and all of their allowed values
using AllowedValuesPair = std::pair<SynonymPair, std::set<AllowedValue>>;
//! Describes all found pairs of synoynms and their allowed values
using AllowedValuesList = std::vector<AllowedValuesPair>;

class ConstraintSolver {
 private:
  static std::set<std::string> getFirstsFromSet(AllowedValueSet&);
  static std::set<std::string> getSecondsFromSet(AllowedValueSet&);
  //! Find the intersection between all stated constraints by synonyms
  static std::map<std::string, std::set<std::string>> intersectConstraints(
      std::vector<AllowedValuesPair>);

  static void intersectTwoConstraints(
      std::map<std::string, std::set<std::string>>&, std::string&,
      std::set<std::string>);
  static void printConstraints(std::map<std::string, std::set<std::string>>);

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

  //! Version that does not require a Synonym object, just the string
  static AllowedValuesPair makeAllowedValues(std::string& syn,
                                             std::vector<std::string>& vals);

  //! Creates a list of allowed values for a single synonym
  static AllowedValuesPair makeAllowedValues(Synonym&,
                                             std::vector<std::string>&);

  static AllowedValuesPair makeAllowedValues(Synonym&, Synonym&,
                                             AllowedValueSet&);

  static std::string getStringPairAsString(std::pair<std::string, std::string>);
  static void printAllowedValuesPair(AllowedValuesPair&);

  //! Actually constrain the set of values and select the synonym indicated
  static std::vector<std::string> constrainAndSelect(
      std::vector<AllowedValuesPair> allowedValues, std::string toSelect);
};
