#pragma once
#include <optional>

#include <string>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/constraint_solver/constraint_database.h"
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

  //! Internal call to make the PQL query that returns unsorted results
  std::vector<std::string> makeQueryUnsorted(QE::Query* query);

  //! Evaluates any SuchThat clause
  bool handleSuchThat(std::vector<QE::Declaration>* decls, QE::RelCond* relCond,
                      ConstraintDatabase& db);
  //! Evaluates any Pattern clause
  bool handlePattern(std::vector<QE::Declaration>* decls, QE::PatternB* pattern,
                     ConstraintDatabase& db);

  //! Evaluates any With clauses
  bool handleWithCond(std::vector<QE::Declaration>* decls,
                      QE::WithCond* withcond, ConstraintDatabase& db);

  //! Utility function to get all synonym strings from a list of selected
  //! vars
  std::vector<std::string> getSynonymsFromSelect(
      std::vector<QE::ResultItem>* resultItems);

  //! Add all selected variables to the set of constraints
  void addAllSelectedVarsToConstraints(Query* query, ConstraintDatabase& db);

  //! Convert Synonym/SynAttr to string based on synonym
  std::string resultItemToString(const ResultItem& resultItem);

  //! Gets the appropriate result to return when the query is evaluted to false
  std::vector<std::string> getNegativeResult(const ResultType& resultType);

  //! Runs the correct ConstraintSolver methods for non/BOOLEAN selects
  std::vector<std::string> selectFromDB(Result* result, ConstraintDatabase& db);

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
      ConstraintDatabase&);

  static std::unordered_set<std::string> getAllDesignEntityValuesByVarName(
      std::vector<QE::Declaration>*, PKBManager*, const std::string&);

  //! Convert a Ref to simpler forms for each evaluator
  static std::optional<QE::Synonym> getRefAsSynonym(QE::Ref);
  static bool isRefUnderscore(QE::Ref);
  static std::optional<std::string> getRefAsBasic(QE::Ref);
};