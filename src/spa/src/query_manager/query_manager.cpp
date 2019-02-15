#include "query_manager/query_manager.h"
#include <cassert>
#include <string>
#include <vector>

using namespace QE;

void QueryManager::makeQuery(Query* query) {
  // If no such-that and pattern clauses - run just the select
  if (query->such_that == nullptr && query->pattern == nullptr) {
    std::cout << "Such that and pattern does not exist\n";
    getSelect(query->selected_declaration->getDesignEntity());
  } else {
    std::cout << "Such that or pattern exists\n";
  }
  return;
}

std::vector<std::string> QueryManager::getSelect(DesignEntity de) {
  // All possible return types from select all PKB calls are vector<string>
  std::cout << "GetSelect: ";
  switch (de) {
    case DesignEntity::ASSIGN:
      std::cout << "assign";
      break;
    case DesignEntity::CALL:
      std::cout << "call";
      break;
    case DesignEntity::CONSTANT:
      std::cout << "constant";
      break;
    case DesignEntity::IF:
      std::cout << "if";
      break;
    case DesignEntity::PRINT:
      std::cout << "print";
      break;
    case DesignEntity::PROCEDURE:
      std::cout << "procedure";
      break;
    case DesignEntity::READ:
      std::cout << "read";
      break;
    case DesignEntity::STMT:
      std::cout << "stmt";
      break;
    case DesignEntity::VARIABLE:
      std::cout << "variable";
      break;
    case DesignEntity::WHILE:
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