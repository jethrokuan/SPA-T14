#pragma once
#include <optional>

#include <string>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/constraint_solver/constraint_solver.h"
#include "query_executor/constraint_solver/query_constraints.h"

using namespace PKB;

//! \brief Clauses can either return constraints or indicate no constraints
//! bool == true ==> no constraints.
//! bool == false or empty allowed pair ==> no valid results
//! Otherwise, need to constrain other relevant values
// using AllowedValuesPairOrBool = std::variant<TupledConstraint, bool>;

class QueryExecutor {
 private:
  PKBManager* pkb;

  //! Evaluates any SuchThat clause
  bool handleSuchThat(QE::Query*, QueryConstraints&);
  //! Evaluates any Pattern clause
  bool handlePattern(QE::Query*, QueryConstraints&);

  //! Utility function to get all synonym strings from a list of selected vars
  std::vector<std::string> getSynonymsFromSelect(
      std::vector<QE::Declaration*>* decls);

 public:
  QueryExecutor(PKBManager* pkb) : pkb(pkb){};

  //! External interface called to make the PQL query
  std::vector<std::string> makeQuery(QE::Query* query);

  //! Gets the result of an unconditional select query (Select <designentity>)
  static std::unordered_set<std::string> getSelect(PKBManager* pkb,
                                                   QE::DesignEntity);

  //! Utility method to call getSelect for a single variable
  static void addAllValuesForVariableToConstraints(
      std::vector<QE::Declaration>*, PKBManager* pkb, const std::string&,
      QueryConstraints&);

  static std::unordered_set<std::string> getAllDesignEntityValuesByVarName(
      std::vector<QE::Declaration>*, PKBManager*, const std::string&);

  //! Convert a StmtOrEntRef to a string to pass to PKB
  static std::string suchThatArgToString(QE::StmtOrEntRef);
  static std::string stmtRefToString(QE::StmtRef);
  static std::string entRefToString(QE::EntRef);
  static std::optional<QE::Synonym> getSuchThatArgAsSynonym(QE::StmtOrEntRef);
  static bool isSuchThatArgUnderscore(QE::StmtOrEntRef);
  static std::optional<std::string> getSuchThatArgAsBasic(QE::StmtOrEntRef);
};