#pragma once
#include <optional>
#include <string>
#include <vector>
#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/pql/pql.h"

using namespace PKB;

class QueryManager {
 private:
  PKBManager* pkb;

  //! Makes the actual query - but returns the result unsorted
  std::vector<std::string> makeQueryUnsorted(QE::Query* query);
  //! Gets the result of a select query (everything from a design entity)
  std::vector<std::string> getSelect(QE::DesignEntity);

  //! Returns true if this SuchThat has no variables to fill
  bool isBooleanSuchThat(QE::SuchThat*);
  //! Evaluates the SuchThat clause as a boolean
  bool isBooleanSuchThatTrue(QE::SuchThat*);
  //! Evaluates SuchThat clauses that don't return a simple boolean
  std::variant<bool, std::vector<std::string>> handleNonBooleanSuchThat(
      QE::Query*);

  //! Can return clause truth/falsity OR return vector to filter with
  std::variant<bool, std::vector<std::string>> handleFollowsTSuchThat(
      QE::Query* query, std::optional<QE::Synonym> arg1AsSynonym,
      std::optional<QE::Synonym> arg2AsSynonym, bool arg1InSelect,
      bool arg2InSelect, bool arg1IsUnderscore, bool arg2IsUnderscore,
      std::optional<std::string> arg1AsBasic,
      std::optional<std::string> arg2AsBasic);

  //! Convert a StmtOrEntRef to a string to pass to PKB
  std::string suchThatArgToString(QE::StmtOrEntRef);
  std::string stmtRefToString(QE::StmtRef);
  std::string entRefToString(QE::EntRef);
  std::optional<QE::Synonym> getSuchThatArgAsSynonym(QE::StmtOrEntRef);
  bool isSuchThatArgUnderscore(QE::StmtOrEntRef);
  std::optional<std::string> getSuchThatArgAsBasic(QE::StmtOrEntRef);

 public:
  QueryManager(PKBManager* pkb) : pkb(pkb){};
  std::vector<std::string> makeQuery(QE::Query* query);
};