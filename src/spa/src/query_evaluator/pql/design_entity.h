#pragma once
#include <string>

namespace QE {
// WARNING: WHEN UPDATING THIS CLASS --> THE wordMap in the CPP FILE MUST BE
// UPDATED
enum class DesignEntity {
  STMT = 1,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE = 10
};
// wordMap is declared in the .cpp file
// Basic accessor for the DesignEntity word map and a reverse mapper
// Note: can throw exception..really wish i had std::optional
DesignEntity getDesignEntity(std::string&);
std::string getDesignEntityString(DesignEntity);
}  // namespace QE