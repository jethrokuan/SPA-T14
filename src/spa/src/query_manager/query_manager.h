#pragma once
#include <string>
#include "program_knowledge_base/pkb_manager.h"
#include "query_evaluator/pql/pql.h"

using namespace PKB;

class QueryManager {
 private:
  PKBManager* pkb;
  //! Gets the result of a select query (everything from a design entity)
  std::vector<std::string> getSelect(QE::DesignEntity);

  //! Returns true if this SuchThat has no variables to fill
  bool isBooleanSuchThat(QE::SuchThat*);
  //! Evaluates the SuchThat clause as a boolean
  bool isBooleanSuchThatTrue(QE::SuchThat*);

  //! Convert a StmtOrEntRef to a string to pass to PKB
  std::string suchThatArgToString(QE::StmtOrEntRef);
  std::string stmtRefToString(QE::StmtRef);
  std::string entRefToString(QE::EntRef);

 public:
  QueryManager(PKBManager* pkb) : pkb(pkb){};
  std::vector<std::string> makeQuery(QE::Query* query);
};