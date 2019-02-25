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
 public:
  //! Placeholder for cases where a second synonym or value is not needed
  static const std::string DUMMY_SYNONYM;

  //! Get an empty constraint set
  static AllowedValuesPair makeEmptyAllowedValuesPair() {
    return std::make_pair(std::make_pair(DUMMY_SYNONYM, DUMMY_SYNONYM),
                          std::set<AllowedValue>());
  }
  //! Creates a list of allowed values for a single synonym
  static AllowedValuesPair makeAllowedValues(Synonym&,
                                             std::vector<std::string>&);
  static std::string getStringPairAsString(std::pair<std::string, std::string>);
  static void printAllowedValuesPair(AllowedValuesPair&);
};
