#pragma once
#include <optional>

#include <string>
#include <unordered_set>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_builder/pql/pql.h"
#include "query_executor/constraint_solver/constraint_database.h"

using namespace PKB;
using namespace QE;

//! \brief Clauses can either return constraints or indicate no constraints
//! bool == true ==> no constraints.
//! bool == false or empty allowed pair ==> no valid results
//! Otherwise, need to constrain other relevant values
// using AllowedValuesPairOrBool = std::variant<TupledConstraint, bool>;
//! Any type of clause - for use during sorting and synonym mapping
class QueryExecutor {
 private:
  PKBManager* pkb;

  //! Evaluates any clause by passing it to the correct executor
  bool executeClause(std::vector<QE::Declaration>* decls, Clause clause,
                     ConstraintDatabase& db);
  //! Evaluates any SuchThat clause
  bool executeClause(std::vector<QE::Declaration>* decls, QE::RelCond* relCond,
                     ConstraintDatabase& db);
  //! Evaluates any Pattern clause
  bool executeClause(std::vector<QE::Declaration>* decls,
                     QE::PatternCond* pattern, ConstraintDatabase& db);

  //! Evaluates any With clauses
  bool executeClause(std::vector<QE::Declaration>* decls,
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

  //! Asks the DB for the result type corresponding to the Select clause
  std::vector<std::string> selectFromDB(std::vector<QE::Declaration>* decls,
                                        Result* result, ConstraintDatabase& db);

  //! For each synonym, look at decls and map them to their design entity
  std::unordered_map<std::string, DesignEntity> getSynoynmToDesignEntityTypeMap(
      std::vector<Declaration>* decls,
      std::vector<QE::ResultItem>* selected_declarations);

  //! E.g. If c.procName is a columns, do .procName on that col's values
  void applyAttributesToResults(
      std::vector<QE::ResultItem>* selected_declarations,
      std::vector<std::vector<std::string>>& results,
      unordered_map<std::string, DesignEntity>& synonym_de_map);

  //! Format results by adding a space between them
  std::vector<std::string> joinResults(
      const std::vector<std::vector<std::string>>& results);

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

  //! /brief Convenience method to get an attribute value from a design entity
  //! value.
  //! E.g. for type print and value 3, we check 3.varName
  static std::string applyAttrToDesignEntityValue(PKBManager* pkb,
                                                  const DesignEntity& de_type,
                                                  const std::string& val);
  //! Convenience when we don't know it's a NAME type
  static std::string applyAttrToDesignEntityValue(PKBManager* pkb,
                                                  const DesignEntity& de_type,
                                                  const std::string& val,
                                                  const AttrName& attrName);
};
