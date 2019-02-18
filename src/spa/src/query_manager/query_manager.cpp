#include "query_manager/query_manager.h"
#include <cassert>
#include <string>
#include <vector>

using namespace QE;

std::vector<std::string> QueryManager::makeQuery(Query* query) {
  // If no such-that and pattern clauses - run just the select
  if (query->such_that == nullptr && query->pattern == nullptr) {
    // std::cout << "Such that and pattern does not exist - returning "
    //             "unconditional Select\n";

    // std::cout << "Running getSelect\n";
    return getSelect(query->selected_declaration->getDesignEntity());
  }

  // std::cout << "Such that or pattern exists\n";
  // If such-that + pattern is true/false answer, can immediately return the
  // select statement
  if (query->such_that && isBooleanSuchThat(query->such_that)) {
    // std::cout << "IS A BOOLEAN SUCH THAT!";
    if (isBooleanSuchThatTrue(query->such_that)) {
      // TODO: This shouldn't be a return - pattern still matters
      return getSelect(query->selected_declaration->getDesignEntity());
    } else {
      // Empty result if our such_that is false - at least one clause is false!
      return std::vector<std::string>();
    }
  }
  // TODO: HANDLE PATTERN

  return std::vector<std::string>();
}

std::vector<std::string> QueryManager::getSelect(DesignEntity de) {
  // All possible return types from select all PKB calls are vector<string>
  // std::cout << "GetSelect: ";
  switch (de) {
    case DesignEntity::ASSIGN:
      // std::cout << "assign";
      return pkb->getAssignList();
      break;
    case DesignEntity::CALL:
      // Next iteration
      // return pkb->getCallList();
      // std::cout << "call";
      break;
    case DesignEntity::CONSTANT:
      // std::cout << "constant";
      return pkb->getConstantList();
      break;
    case DesignEntity::IF:
      // std::cout << "if";
      return pkb->getIfList();
      break;
    case DesignEntity::PRINT:
      // std::cout << "print";
      return pkb->getPrintList();
      break;
    case DesignEntity::PROCEDURE:
      // std::cout << "procedure: ";
      return pkb->getProcedureList();
      break;
    case DesignEntity::READ:
      // std::cout << "read";
      return pkb->getReadList();
      break;
    case DesignEntity::STMT:
      // std::cout << "stmt";
      return pkb->getStatementList();
      break;
    case DesignEntity::VARIABLE:
      // std::cout << "variable";
      return pkb->getVariableList();
      break;
    case DesignEntity::WHILE:
      // std::cout << "while";
      return pkb->getWhileList();
      break;
    default:
      break;
      // This should never happen - we should have handled all cases
      assert(false);
  }

  std::cout << std::endl;
  return std::vector<std::string>();
}