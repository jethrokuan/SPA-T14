#pragma once
#include <string>
#include <vector>

namespace QE {
// WARNING: WHEN UPDATING THIS CLASS --> THE wordMap in the CPP FILE MUST BE
// UPDATED, along with all other stuff in this .h file if needed
enum class DesignEntity {
  ASSIGN,
  READ,
  STMT,
  IF,
  WHILE,
  PRINT,
  CALL,
  VARIABLE,
  CONSTANT,
  PROCEDURE,
  PROG_LINE
};

// Utility definitions for different sets of design entities
// Used in relation.cpp for relation synonym argument matching
const auto ALL_STMT_DESIGN_ENTITIES = std::vector<DesignEntity>{
    DesignEntity::ASSIGN, DesignEntity::READ,  DesignEntity::STMT,
    DesignEntity::IF,     DesignEntity::WHILE, DesignEntity::PRINT,
    DesignEntity::CALL};
const auto ALL_STMT_DESIGN_ENTITIES_NO_PRINT = std::vector<DesignEntity>{
    DesignEntity::ASSIGN, DesignEntity::READ,  DesignEntity::STMT,
    DesignEntity::IF,     DesignEntity::WHILE, DesignEntity::CALL};
const auto ALL_STMT_DESIGN_ENTITIES_NO_READ = std::vector<DesignEntity>{
    DesignEntity::ASSIGN, DesignEntity::STMT,  DesignEntity::IF,
    DesignEntity::WHILE,  DesignEntity::PRINT, DesignEntity::CALL};
const auto PROCEDURE_DESIGN_ENTITY =
    std::vector<DesignEntity>{DesignEntity::PROCEDURE};
const auto VARIABLE_DESIGN_ENTITY =
    std::vector<DesignEntity>{DesignEntity::VARIABLE};
const auto PROG_LINE_DESIGN_ENTITY =
    std::vector<DesignEntity>{DesignEntity::PROG_LINE};
const auto ASSIGN_DESIGN_ENTITY =
    std::vector<DesignEntity>{DesignEntity::ASSIGN};
// wordMap is declared in the .cpp file
// Basic accessor for the DesignEntity word map and a reverse mapper
// Note: can throw exception..really wish i had std::optional
DesignEntity getDesignEntity(std::string&);
std::string getDesignEntityString(DesignEntity);
std::string getDesignEntityVectorString(std::vector<DesignEntity>);
}  // namespace QE