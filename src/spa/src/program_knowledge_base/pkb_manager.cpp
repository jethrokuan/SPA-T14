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

std::vector<Line> PKBManager::getAssignList() {
  return pkb_storage->assign_list;
}

std::vector<Line> PKBManager::getPrintList() { return pkb_storage->print_list; }

std::vector<Line> PKBManager::getReadList() { return pkb_storage->read_list; }

std::vector<Procedure> PKBManager::getProcedureList() {
  return pkb_storage->procedure_list;
}

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

// bool PKBManager::isProcedureUsesVar(const Procedure proc, const Variable var)
// {
//   return pkb_storage->procedure_uses_var_set.find(
//              std::pair<Procedure, Variable>(proc, var)) !=
//          pkb_storage->procedure_uses_var_set.end();
// }

bool PKBManager::isLineUsesVar(const Line line, const Variable var) {
  return pkb_storage->line_uses_var_set.find(std::pair<Line, Variable>(
             line, var)) != pkb_storage->line_uses_var_set.end();
}

// bool PKBManager::isProcedureModifiesVar(const Procedure proc,
//                                         const Variable var) {
//   return pkb_storage->procedure_modifies_var_set.find(
//              std::pair<Procedure, Variable>(proc, var)) !=
//          pkb_storage->procedure_modifies_var_set.end();
// }

bool PKBManager::isLineModifiesVar(const Line line, const Variable var) {
  return pkb_storage->line_modifies_var_set.find(std::pair<Line, Variable>(
             line, var)) != pkb_storage->line_modifies_var_set.end();
}

}  // namespace PKB
