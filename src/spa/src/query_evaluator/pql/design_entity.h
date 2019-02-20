#pragma once
#include <string>
#include <vector>

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

// Utility definitions for different sets of design entities
// TODO: Add Call/Procedure as required
const auto ALL_STMT_DESIGN_ENTITIES = std::vector<DesignEntity>{
    DesignEntity::ASSIGN, DesignEntity::READ,  DesignEntity::STMT,
    DesignEntity::IF,     DesignEntity::WHILE, DesignEntity::PROCEDURE,
    DesignEntity::PRINT};
const auto ALL_STMT_DESIGN_ENTITIES_NO_PRINT = std::vector<DesignEntity>{
    DesignEntity::ASSIGN, DesignEntity::READ,  DesignEntity::STMT,
    DesignEntity::IF,     DesignEntity::WHILE, DesignEntity::PROCEDURE};
const auto ALL_STMT_DESIGN_ENTITIES_NO_READ = std::vector<DesignEntity>{
    DesignEntity::ASSIGN, DesignEntity::STMT,      DesignEntity::IF,
    DesignEntity::WHILE,  DesignEntity::PROCEDURE, DesignEntity::PRINT};

// wordMap is declared in the .cpp file
// Basic accessor for the DesignEntity word map and a reverse mapper
// Note: can throw exception..really wish i had std::optional
DesignEntity getDesignEntity(std::string&);
std::string getDesignEntityString(DesignEntity);
std::string getDesignEntityVectorString(std::vector<DesignEntity>);
}  // namespace QE