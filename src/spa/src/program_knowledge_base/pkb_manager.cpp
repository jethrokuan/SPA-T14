#include "program_knowledge_base/pkb_manager.h"
#include <iostream>
template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

namespace PKB {
PKBManager::PKBManager(const std::shared_ptr<ProcedureNode> ast) {
  pkb_storage->storeAST(ast);
  std::unique_ptr<PKBPreprocessor> pkb_preprocessor =
      std::make_unique<PKBPreprocessor>(ast, pkb_storage);
  // TODO indicate finished
}

PKBManager::~PKBManager() {}

// is design entity exists
bool PKBManager::isVariableExists(Variable var) {
  return pkb_storage->var_set.find(var) != pkb_storage->var_set.end();
}

bool PKBManager::isAssignExists(Line line) {
  return pkb_storage->assign_set.find(line) != pkb_storage->assign_set.end();
}

bool PKBManager::isStatementExists(Line line) {
  return pkb_storage->statement_set.find(line) !=
         pkb_storage->statement_set.end();
}

bool PKBManager::isPrintExists(Line line) {
  return pkb_storage->print_set.find(line) != pkb_storage->assign_set.end();
}

bool PKBManager::isReadExists(Line line) {
  return pkb_storage->read_set.find(line) != pkb_storage->assign_set.end();
}

bool PKBManager::isWhileExists(Line line) {
  return pkb_storage->while_set.find(line) != pkb_storage->while_set.end();
}

bool PKBManager::isIfExists(Line line) {
  return pkb_storage->if_set.find(line) != pkb_storage->if_set.end();
}

bool PKBManager::isConstantExists(Constant c) {
  return pkb_storage->constant_set.find(c) != pkb_storage->constant_set.end();
}

bool PKBManager::isProcedureExists(Procedure proc) {
  return pkb_storage->procedure_set.find(proc) !=
         pkb_storage->procedure_set.end();
}

// get design entities

std::vector<Line> PKBManager::getVariableList() {
  return pkb_storage->var_list;
}

std::vector<Line> PKBManager::getAssignList() {
  return pkb_storage->assign_list;
}

std::vector<Line> PKBManager::getStatementList() {
  return pkb_storage->statement_list;
}

std::vector<Line> PKBManager::getPrintList() { return pkb_storage->print_list; }

std::vector<Line> PKBManager::getReadList() { return pkb_storage->read_list; }

std::vector<Procedure> PKBManager::getWhileList() {
  return pkb_storage->while_list;
}

std::vector<Procedure> PKBManager::getIfList() { return pkb_storage->if_list; }

std::vector<Procedure> PKBManager::getConstantList() {
  return pkb_storage->constant_list;
}

std::vector<Procedure> PKBManager::getProcedureList() {
  return pkb_storage->procedure_list;
}

// is relationship valid

bool PKBManager::isLineFollowLine(const LineBefore line_before,
                                  const LineAfter line_after) {
  return pkb_storage->follows_set.find(std::pair<LineBefore, LineAfter>(
             line_before, line_after)) != pkb_storage->follows_set.end();
}

bool PKBManager::isLineParentLine(const ParentLine parent_line,
                                  const ChildLine child_line) {
  return pkb_storage->parent_set.find(std::pair<ParentLine, ChildLine>(
             parent_line, child_line)) != pkb_storage->follows_set.end();
}

bool PKBManager::isProcedureUsesVar(const Procedure proc, const Variable var) {
  return pkb_storage->procedure_uses_var_set.find(
             std::pair<Procedure, Variable>(proc, var)) !=
         pkb_storage->procedure_uses_var_set.end();
}

bool PKBManager::isLineUsesVar(const Line line, const Variable var) {
  return pkb_storage->line_uses_var_set.find(std::pair<Line, Variable>(
             line, var)) != pkb_storage->line_uses_var_set.end();
}

bool PKBManager::isProcedureModifiesVar(const Procedure proc,
                                        const Variable var) {
  return pkb_storage->procedure_modifies_var_set.find(
             std::pair<Procedure, Variable>(proc, var)) !=
         pkb_storage->procedure_modifies_var_set.end();
}

bool PKBManager::isLineModifiesVar(const Line line, const Variable var) {
  return pkb_storage->line_modifies_var_set.find(std::pair<Line, Variable>(
             line, var)) != pkb_storage->line_modifies_var_set.end();
}

// get relationship mapping

std::vector<Procedure> PKBManager::getProcedureUsesVar(const Variable var) {
  return pkb_storage->procedure_uses_var_map.at(var);
}

std::vector<Line> PKBManager::getLineUsesVar(const Variable var) {
  return pkb_storage->line_uses_var_map.at(var);
}

std::vector<Variable> PKBManager::getVarUsedByProcedure(const Procedure proc) {
  return pkb_storage->var_used_by_procedure_map.at(proc);
}

std::vector<Variable> PKBManager::getVarUsedByLine(const Line line) {
  return pkb_storage->var_used_by_line_map.at(line);
}

std::vector<Variable> PKBManager::getVarModifiedByProcedure(
    const Procedure proc) {
  return pkb_storage->var_modified_by_procedure_map.at(proc);
}

std::vector<Variable> PKBManager::getVarModifiedByLine(const Line line) {
  return pkb_storage->var_modified_by_line_map.at(line);
}

std::vector<Procedure> PKBManager::getProcedureModifiesVar(const Variable var) {
  return pkb_storage->procedure_modifies_var_map.at(var);
}

std::vector<Line> PKBManager::getLineModifiesVar(const Variable var) {
  return pkb_storage->line_modifies_var_map.at(var);
}

}  // namespace PKB
