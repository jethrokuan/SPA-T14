#include "query_manager/query_manager.h"
#include <cassert>
#include <string>
#include <vector>

using namespace QE;

std::vector<std::string> QueryManager::makeQuery(Query* query) {
  // If no such-that and pattern clauses - run just the select
  if (query->such_that == nullptr && query->pattern == nullptr) {
    std::cout << "Such that and pattern does not exist - returning "
                 "unconditional Select\n";
    return getSelect(query->selected_declaration->getDesignEntity());
  }

  std::cout << "Such that or pattern exists\n";
  // If such-that + pattern is true/false answer, can immediately return the
  // select statement
  if (query->such_that && isBooleanSuchThat(query->such_that)) {
    std::cout << "IS A BOOLEAN SUCH THAT!";
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
  std::cout << "GetSelect: ";
  switch (de) {
    case DesignEntity::ASSIGN:
      std::cout << "assign";
      // return pkb->getAssignList();
      break;
    case DesignEntity::CALL:
      // Next iteration
      // return pkb->getCallList();
      std::cout << "call";
      break;
    case DesignEntity::CONSTANT:
      std::cout << "constant";
      // return pkb->getConstantList();
      break;
    case DesignEntity::IF:
      // return pkb->getIfList();
      std::cout << "if";
      break;
    case DesignEntity::PRINT:
      // return pkb->getPrintList();
      std::cout << "print";
      break;
    case DesignEntity::PROCEDURE:
      // return pkb->getProcedureList();
      std::cout << "procedure";
      break;
    case DesignEntity::READ:
      // return pkb->getReadList();
      std::cout << "read";
      break;
    case DesignEntity::STMT:
      // return pkb->getStatementList();
      std::cout << "stmt";
      break;
    case DesignEntity::VARIABLE:
      // return pkb->getVariableList();
      std::cout << "variable";
      break;
    case DesignEntity::WHILE:
      // return pkb->getWhileList();
      std::cout << "while";
      break;
    default:
      break;
      // This should never happen - we should have handled all cases
      // assert(false);
  }

  std::cout << std::endl;
  return std::vector<std::string>();
}