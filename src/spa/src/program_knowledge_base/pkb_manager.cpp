#include "program_knowledge_base/pkb_manager.h"

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

namespace PKB {
PKBManager::PKBManager(const AST ast) {
  pkb_storage->storeAST(ast);
  std::unique_ptr<PKBPreprocessor> pkb_preprocessor =
      std::make_unique<PKBPreprocessor>(ast, pkb_storage);
}

PKBManager::~PKBManager() {}

// helper
std::optional<std::unordered_set<std::string>> PKBManager::getSetFromMap(
    const std::unordered_map<std::string, std::unordered_set<std::string>>
        &umap,
    const std::string key) {
  return (umap.find(key) == umap.end())
             ? std::nullopt
             : std::make_optional<std::unordered_set<std::string>>(
                   umap.at(key));
}

std::optional<std::string> PKBManager::getElemFromMap(
    const std::unordered_map<std::string, std::string> &umap,
    const std::string key) {
  return (umap.find(key) == umap.end())
             ? std::nullopt
             : std::make_optional<std::string>(umap.at(key));
}

// is design entity set empty
bool PKBManager::isVariableSetEmpty() { return pkb_storage->var_set.empty(); }

bool PKBManager::isAssignSetEmpty() { return pkb_storage->assign_set.empty(); }

bool PKBManager::isPrintSetEmpty() { return pkb_storage->print_set.empty(); }

bool PKBManager::isReadSetEmpty() { return pkb_storage->read_set.empty(); }

bool PKBManager::isWhileSetEmpty() { return pkb_storage->while_set.empty(); }

bool PKBManager::isIfSetEmpty() { return pkb_storage->if_set.empty(); }

bool PKBManager::isConstantSetEmpty() {
  return pkb_storage->constant_set.empty();
}

bool PKBManager::isCallSetEmpty() { return pkb_storage->call_set.empty(); }

// is design entity exists
bool PKBManager::isVariableExists(const Variable var) {
  return pkb_storage->var_set.find(var) != pkb_storage->var_set.end();
}

bool PKBManager::isAssignExists(const Line line) {
  return pkb_storage->assign_set.find(line) != pkb_storage->assign_set.end();
}

bool PKBManager::isStatementExists(const Line line) {
  return pkb_storage->statement_set.find(line) !=
         pkb_storage->statement_set.end();
}

bool PKBManager::isPrintExists(const Line line) {
  return pkb_storage->print_set.find(line) != pkb_storage->assign_set.end();
}

bool PKBManager::isReadExists(const Line line) {
  return pkb_storage->read_set.find(line) != pkb_storage->assign_set.end();
}

bool PKBManager::isWhileExists(const Line line) {
  return pkb_storage->while_set.find(line) != pkb_storage->while_set.end();
}

bool PKBManager::isIfExists(const Line line) {
  return pkb_storage->if_set.find(line) != pkb_storage->if_set.end();
}

bool PKBManager::isConstantExists(const Constant c) {
  return pkb_storage->constant_set.find(c) != pkb_storage->constant_set.end();
}

bool PKBManager::isProcedureExists(const Procedure proc) {
  return pkb_storage->procedure_set.find(proc) !=
         pkb_storage->procedure_set.end();
}

bool PKBManager::isCallExists(const Line line) {
  return pkb_storage->call_set.find(line) != pkb_storage->call_set.end();
}

// get design entities

std::unordered_set<Line> PKBManager::getVariableSet() {
  return pkb_storage->var_set;
}

std::unordered_set<Line> PKBManager::getAssignSet() {
  return pkb_storage->assign_set;
}

std::unordered_set<Line> PKBManager::getStatementSet() {
  return pkb_storage->statement_set;
}

std::unordered_set<Line> PKBManager::getPrintSet() {
  return pkb_storage->print_set;
}

std::unordered_set<Line> PKBManager::getReadSet() {
  return pkb_storage->read_set;
}

std::unordered_set<Procedure> PKBManager::getWhileSet() {
  return pkb_storage->while_set;
}

std::unordered_set<Procedure> PKBManager::getIfSet() {
  return pkb_storage->if_set;
}

std::unordered_set<Procedure> PKBManager::getConstantSet() {
  return pkb_storage->constant_set;
}

std::unordered_set<Procedure> PKBManager::getProcedureSet() {
  return pkb_storage->procedure_set;
}

std::unordered_set<Procedure> PKBManager::getCallSet() {
  return pkb_storage->call_set;
}

// is relationship set empty
bool PKBManager::isLineNextLineMapEmpty() {
  return pkb_storage->line_previous_line_next_map.empty();
}

bool PKBManager::isProcedureCallProcedureSetEmpty() {
  return pkb_storage->procedure_calls_procedure_set.empty();
}

bool PKBManager::isProcedureCallProcedureTSetEmpty() {
  return pkb_storage->procedure_calls_procedure_set_t.empty();
}

bool PKBManager::isLineFollowLineSetEmpty() {
  return pkb_storage->follows_set.empty();
}

bool PKBManager::isLineFollowLineTSetEmpty() {
  return pkb_storage->follows_set_t.empty();
}

bool PKBManager::isLineParentLineSetEmpty() {
  return pkb_storage->parent_set.empty();
}

bool PKBManager::isLineParentLineTSetEmpty() {
  return pkb_storage->parent_set_t.empty();
}

bool PKBManager::isLineUsesVarSetEmpty() {
  return pkb_storage->line_uses_var_set.empty();
}

bool PKBManager::isLineModifiesVarSetEmpty() {
  return pkb_storage->line_modifies_var_set.empty();
}

bool PKBManager::isProcedureUsesVarSetEmpty() {
  return pkb_storage->procedure_uses_var_set.empty();
}

bool PKBManager::isProcedureModifiesVarSetEmpty() {
  return pkb_storage->procedure_modifies_var_set.empty();
}

// is relationship valid

bool PKBManager::isProcedureCallsProcedure(
    const ProcedureCaller procedure_caller,
    const ProcedureCallee procedure_callee) {
  return pkb_storage->procedure_calls_procedure_set.find(
             std::pair<ProcedureCaller, ProcedureCallee>(procedure_caller,
                                                         procedure_callee)) !=
         pkb_storage->procedure_calls_procedure_set.end();
}

bool PKBManager::isProcedureCallsProcedureT(
    const ProcedureCaller procedure_caller,
    const ProcedureCallee procedure_callee) {
  return pkb_storage->procedure_calls_procedure_set_t.find(
             std::pair<ProcedureCaller, ProcedureCallee>(procedure_caller,
                                                         procedure_callee)) !=
         pkb_storage->procedure_calls_procedure_set_t.end();
}

bool PKBManager::isLineFollowLine(const LineBefore line_before,
                                  const LineAfter line_after) {
  return pkb_storage->follows_set.find(std::pair<LineBefore, LineAfter>(
             line_before, line_after)) != pkb_storage->follows_set.end();
}

bool PKBManager::isLineFollowLineT(const LineBefore line_before,
                                   const LineAfter line_after) {
  return pkb_storage->follows_set_t.find(std::pair<LineBefore, LineAfter>(
             line_before, line_after)) != pkb_storage->follows_set.end();
}

bool PKBManager::isLineParentLine(const ParentLine parent_line,
                                  const ChildLine child_line) {
  return pkb_storage->parent_set.find(std::pair<ParentLine, ChildLine>(
             parent_line, child_line)) != pkb_storage->follows_set.end();
}

bool PKBManager::isLineParentLineT(const ParentLine parent_line,
                                   const ChildLine child_line) {
  return pkb_storage->parent_set_t.find(std::pair<ParentLine, ChildLine>(
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
std::optional<std::unordered_set<ProcedureCallee>>
PKBManager::getCalleeProcedures(const ProcedureCaller procedure_caller) {
  return getSetFromMap(pkb_storage->procedure_caller_procedure_callee_map,
                       procedure_caller);
}

std::optional<std::unordered_set<ProcedureCaller>>
PKBManager::getCallerProcedures(const ProcedureCaller procedure_callee) {
  return getSetFromMap(pkb_storage->procedure_callee_procedure_caller_map,
                       procedure_callee);
}

std::optional<std::unordered_set<ProcedureCallee>>
PKBManager::getCalleeProceduresT(const ProcedureCaller procedure_caller) {
  return getSetFromMap(pkb_storage->procedure_caller_procedure_callee_map_t,
                       procedure_caller);
}

std::optional<std::unordered_set<ProcedureCaller>>
PKBManager::getCallerProceduresT(const ProcedureCaller procedure_callee) {
  return getSetFromMap(pkb_storage->procedure_callee_procedure_caller_map_t,
                       procedure_callee);
}

std::optional<LineAfter> PKBManager::getFollowingLine(
    const LineBefore line_before) {
  return (pkb_storage->line_before_line_after_map.find(line_before) ==
          pkb_storage->line_before_line_after_map.end())
             ? std::nullopt
             : std::make_optional<LineAfter>(
                   pkb_storage->line_before_line_after_map[line_before]);
}

std::optional<LineBefore> PKBManager::getBeforeLine(
    const LineAfter line_after) {
  return (pkb_storage->line_after_line_before_map.find(line_after) ==
          pkb_storage->line_after_line_before_map.end())
             ? std::nullopt
             : std::make_optional<LineBefore>(
                   pkb_storage->line_after_line_before_map[line_after]);
}

std::optional<std::unordered_set<LineAfter>> PKBManager::getFollowingLineT(
    const LineBefore line_before) {
  return getSetFromMap(pkb_storage->line_before_line_after_map_t, line_before);
}

std::optional<std::unordered_set<LineBefore>> PKBManager::getBeforeLineT(
    const LineAfter line_after) {
  return getSetFromMap(pkb_storage->line_after_line_before_map_t, line_after);
}

std::optional<ParentLine> PKBManager::getParentLine(
    const ChildLine child_line) {
  return (pkb_storage->child_line_parent_line_map.find(child_line) ==
          pkb_storage->child_line_parent_line_map.end())
             ? std::nullopt
             : std::make_optional<ParentLine>(
                   pkb_storage->child_line_parent_line_map[child_line]);
}

std::optional<std::unordered_set<ChildLine>> PKBManager::getChildLine(
    const ParentLine parent_line) {
  return getSetFromMap(pkb_storage->parent_line_child_line_map, parent_line);
}

std::optional<std::unordered_set<ParentLine>> PKBManager::getParentLineT(
    const ChildLine child_line) {
  return getSetFromMap(pkb_storage->child_line_parent_line_map_t, child_line);
}

std::optional<std::unordered_set<ChildLine>> PKBManager::getChildLineT(
    const ParentLine parent_line) {
  return getSetFromMap(pkb_storage->parent_line_child_line_map_t, parent_line);
}

std::optional<std::unordered_set<Procedure>> PKBManager::getProcedureUsesVar(
    const Variable var) {
  return getSetFromMap(pkb_storage->procedure_uses_var_map, var);
}

std::optional<std::unordered_set<Line>> PKBManager::getLineUsesVar(
    const Variable var) {
  return getSetFromMap(pkb_storage->line_uses_var_map, var);
}

std::optional<std::unordered_set<Variable>> PKBManager::getVarUsedByProcedure(
    const Procedure proc) {
  return getSetFromMap(pkb_storage->var_used_by_procedure_map, proc);
}

std::optional<std::unordered_set<Variable>> PKBManager::getVarUsedByLine(
    const Line line) {
  return getSetFromMap(pkb_storage->var_used_by_line_map, line);
}

std::optional<std::unordered_set<Variable>>
PKBManager::getVarModifiedByProcedure(const Procedure proc) {
  return getSetFromMap(pkb_storage->var_modified_by_procedure_map, proc);
}

std::optional<std::unordered_set<Variable>> PKBManager::getVarModifiedByLine(
    const Line line) {
  return getSetFromMap(pkb_storage->var_modified_by_line_map, line);
}

std::optional<std::unordered_set<Procedure>>
PKBManager::getProcedureModifiesVar(const Variable var) {
  return getSetFromMap(pkb_storage->procedure_modifies_var_map, var);
}

std::optional<std::unordered_set<Line>> PKBManager::getLineModifiesVar(
    const Variable var) {
  return getSetFromMap(pkb_storage->line_modifies_var_map, var);
}

// TODO tidy this up
std::optional<std::unordered_set<Line>>
PKBManager::getAssignPatternCompleteMatchLinesWithVar(const Variable var,
                                                      const Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);

  // check if the variable exists
  if (pkb_storage->var_set.find(var) == pkb_storage->var_set.end()) {
    return std::nullopt;
  }

  // TODO check if this is even necessary?
  // check if pattern exists
  if (pkb_storage->assign_pattern_expr_str_set.find(pattern_expr) ==
      pkb_storage->assign_pattern_expr_str_set.end()) {
    return std::nullopt;
  }

  // retrieve list of patterns for the variable
  auto ys = pkb_storage->assign_pattern_var_line_expr_str_map.at(var);
  std::unordered_set<Line> matching_lines;
  for (const auto &elem : ys) {
    auto line = elem.first;
    auto elem_pattern_expr = elem.second;
    if (pattern_expr.compare(elem_pattern_expr) == 0) {
      matching_lines.insert(line);
    }
  }

  if (matching_lines.size() == 0) {
    return std::nullopt;
  } else {
    return std::make_optional<std::unordered_set<Line>>(matching_lines);
  }
}

std::optional<std::unordered_set<Line>>
PKBManager::getAssignPatternPartialMatchLinesWithVar(const Variable var,
                                                     const Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);

  // check if the variable exists
  if (pkb_storage->var_set.find(var) == pkb_storage->var_set.end()) {
    return std::nullopt;
  }

  // retrieve list of patterns for the variable
  auto ys = pkb_storage->assign_pattern_var_line_expr_str_map.at(var);
  std::unordered_set<Line> matching_lines;
  for (const auto &elem : ys) {
    auto line = elem.first;
    auto elem_pattern_expr = elem.second;
    if (elem_pattern_expr.find(pattern_expr) != std::string::npos) {
      matching_lines.insert(line);
    }
  }

  if (matching_lines.size() == 0) {
    return std::nullopt;
  } else {
    return std::make_optional<std::unordered_set<Line>>(matching_lines);
  }
}

std::optional<std::unordered_set<Line>>
PKBManager::getAssignPatternCompleteMatchLines(const Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);

  // TODO check if this is even necessary?
  // check if pattern exists
  if (pkb_storage->assign_pattern_expr_str_set.find(pattern_expr) ==
      pkb_storage->assign_pattern_expr_str_set.end()) {
    return std::nullopt;
  }

  std::unordered_set<Line> matching_lines;
  for (const auto &elem : pkb_storage->assign_pattern_line_expr_str_set) {
    auto line = elem.first;
    auto elem_pattern_expr = elem.second;
    // substring
    if (pattern_expr.compare(elem_pattern_expr) == 0) {
      matching_lines.insert(line);
    }
  }

  if (matching_lines.size() == 0) {
    return std::nullopt;
  } else {
    return std::make_optional<std::unordered_set<Line>>(matching_lines);
  }
}

std::optional<std::unordered_set<Line>>
PKBManager::getAssignPatternPartialMatchLines(const Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);

  std::unordered_set<Line> matching_lines;
  for (const auto &elem : pkb_storage->assign_pattern_line_expr_str_set) {
    auto line = elem.first;
    auto elem_pattern_expr = elem.second;
    // substring
    if (elem_pattern_expr.find(pattern_expr) != std::string::npos) {
      matching_lines.insert(line);
    }
  }

  if (matching_lines.size() == 0) {
    return std::nullopt;
  } else {
    return std::make_optional<std::unordered_set<Line>>(matching_lines);
  }
}

std::optional<std::unordered_set<std::pair<Line, Variable>, pair_hash>>
PKBManager::getAssignPatternCompleteMatchLinesAndVars(const Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);

  return (pkb_storage->assign_pattern_expr_str_line_var_map.find(
              pattern_expr) ==
          pkb_storage->assign_pattern_expr_str_line_var_map.end())
             ? std::nullopt
             : std::make_optional<
                   std::unordered_set<std::pair<Line, Variable>, pair_hash>>(
                   pkb_storage->assign_pattern_expr_str_line_var_map.at(
                       pattern_expr));
}

// TODO check for possible errors
// check if it always returns properly
std::optional<std::unordered_set<std::pair<Line, Variable>, pair_hash>>
PKBManager::getAssignPatternPartialMatchLinesAndVars(const Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);

  std::unordered_set<ExprStr> matching_expr_str;
  for (const auto &elem : pkb_storage->assign_pattern_expr_str_set) {
    // find the expr_str that matches partially
    if (elem.find(pattern_expr) != std::string::npos) {
      matching_expr_str.insert(elem);
    }
  }

  std::unordered_set<std::pair<Line, Variable>, pair_hash> matching_line_var;
  for (const auto &elem : matching_expr_str) {
    // pattern must be in the map already at this point
    auto xs = pkb_storage->assign_pattern_expr_str_line_var_map.at(elem);
    for (const auto &x : (xs)) {
      matching_line_var.insert(x);
    }
  }

  if (matching_expr_str.size() == 0) {
    return std::nullopt;
  } else {
    return std::make_optional<
        std::unordered_set<std::pair<Line, Variable>, pair_hash>>(
        matching_line_var);
  }
}

std::unordered_set<std::pair<Line, Variable>, pair_hash>
PKBManager::getAllAssignPatternLinesAndVars() {
  return pkb_storage->assign_pattern_line_var_set;
}

// TODO use this in the other pattern methods
bool PKBManager::isAssignPatternExists(Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);
  return pkb_storage->assign_pattern_expr_str_set.find(pattern_expr) !=
         pkb_storage->assign_pattern_expr_str_set.end();
}

std::optional<std::unordered_set<Line>> PKBManager::getIfPatternLine(
    const Variable var) {
  return getSetFromMap(pkb_storage->if_pattern_control_variable_line_map, var);
}

std::optional<std::unordered_set<Variable>> PKBManager::getIfPatternVariable(
    const Line line) {
  return getSetFromMap(pkb_storage->if_pattern_line_control_variable_map, line);
}

std::unordered_set<std::pair<Line, Variable>, pair_hash>
PKBManager::getAllIfPatternLinesAndVars() {
  return pkb_storage->if_pattern_line_var_set;
}

bool PKBManager::isIfPatternExists(const Line line, const Variable var) {
  const std::pair<Line, Variable> pattern =
      std::pair<Line, Variable>(line, var);
  return pkb_storage->if_pattern_line_var_set.find(pattern) !=
         pkb_storage->if_pattern_line_var_set.end();
}

std::optional<std::unordered_set<Line>> PKBManager::getWhilePatternLine(
    const Variable var) {
  return getSetFromMap(pkb_storage->while_pattern_control_variable_line_map,
                       var);
}

std::optional<std::unordered_set<Variable>> PKBManager::getWhilePatternVariable(
    const Line line) {
  return getSetFromMap(pkb_storage->while_pattern_line_control_variable_map,
                       line);
}

std::unordered_set<std::pair<Line, Variable>, pair_hash>
PKBManager::getAllWhilePatternLinesAndVars() {
  return pkb_storage->while_pattern_line_var_set;
}

bool PKBManager::isWhilePatternExists(const Line line, const Variable var) {
  const std::pair<Line, Variable> pattern =
      std::pair<Line, Variable>(line, var);
  return pkb_storage->while_pattern_line_var_set.find(pattern) !=
         pkb_storage->while_pattern_line_var_set.end();
}

std::optional<std::unordered_set<Line>> PKBManager::getLineForAssignVar(
    const Variable var) {
  return getSetFromMap(pkb_storage->assign_pattern_var_line_map, var);
}

std::optional<Procedure> PKBManager::getProcedureFromLine(const Line line) {
  if (isStatementExists(line)) {
    return getElemFromMap(pkb_storage->line_procedure_map, line);
  } else {
    return std::nullopt;
  }
}

std::optional<Procedure> PKBManager::getProcedureCalleeFromLine(
    const Line line) {
  if (isCallExists(line)) {
    return getElemFromMap(pkb_storage->line_calls_procedure_map, line);
  } else {
    return std::nullopt;
  }
}

std::optional<Variable> PKBManager::getReadVariableFromLine(const Line line) {
  if (isReadExists(line)) {
    return getElemFromMap(pkb_storage->line_read_var_map, line);
  } else {
    return std::nullopt;
  }
}

std::optional<Variable> PKBManager::getPrintVariableFromLine(const Line line) {
  if (isPrintExists(line)) {
    return getElemFromMap(pkb_storage->line_print_var_map, line);
  } else {
    return std::nullopt;
  }
}

std::optional<Variable> PKBManager::getModifyVariableFromAssignLine(
    const ModifyLine modify_line) {
  if (isAssignExists(modify_line)) {
    return getElemFromMap(pkb_storage->assign_line_modifies_variable_map,
                          modify_line);
  } else {
    return std::nullopt;
  }
}

std::optional<std::unordered_set<Variable>>
PKBManager::getUsesVariableFromAssignLine(const UsesLine uses_line) {
  if (isAssignExists(uses_line)) {
    return getSetFromMap(pkb_storage->assign_line_uses_variable_map, uses_line);
  } else {
    return std::nullopt;
  }
}

bool PKBManager::isFromSameProcedure(const Line l1, const Line l2) {
  auto p1 = getProcedureFromLine(l1);
  auto p2 = getProcedureFromLine(l2);
  return (p1 && p2) && (*p1 == *p2);
}

bool PKBManager::isLineAffectsVariable(const Line line, const Variable var) {
  // modify in affects can only occur for
  // assign, read and call
  if (isAssignExists(line) || isReadExists(line) || isCallExists(line)) {
    auto var_modified = getVarModifiedByLine(line);
    if (var_modified) {
      return (*var_modified).find(var) != (*var_modified).end();
    }
  }

  return false;
}

bool PKBManager::isLineNextLine(const PreviousLine previous_line,
                                const NextLine next_line) {
  if (pkb_storage->line_previous_line_next_map.find(previous_line) !=
      pkb_storage->line_previous_line_next_map.end()) {
    return pkb_storage->line_previous_line_next_map.at(previous_line)
               .find(next_line) !=
           pkb_storage->line_previous_line_next_map.at(previous_line).end();
  } else {
    return false;
  }
}

bool PKBManager::isLineNextLineT(const PreviousLine previous_line,
                                 const NextLine next_line) {
  // check that lines are in the same procedure
  // able to eliminate unnecessary DFS
  if (!isFromSameProcedure(previous_line, next_line)) {
    return false;
  }
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  auto to_visit = getNextLine(previous_line);
  if (to_visit) {
    for (const auto &neighbour : *to_visit) {
      const bool res = isLineNextLineTH(neighbour, next_line, visited);
      if (res) {
        return true;
      }
    }
  }

  return false;
}

bool PKBManager::isLineNextLineTH(
    const Line cur_line, const Line target,
    std::shared_ptr<std::unordered_set<Line>> visited) {
  // if node has been visited before
  if (visited->find(cur_line) != visited->end()) {
    return false;
  }
  // if target node has been reached
  if (cur_line == target) {
    return true;
  }
  visited->insert(cur_line);
  auto to_visit = getNextLine(cur_line);
  if (to_visit) {
    for (const auto &neighbour : *to_visit) {
      const bool res = isLineNextLineTH(neighbour, target, visited);
      if (res) {
        return true;
      }
    }
  }
  return false;
}

std::optional<std::unordered_set<PreviousLine>> PKBManager::getPreviousLine(
    const NextLine next_line) {
  return getSetFromMap(pkb_storage->line_next_line_previous_map, next_line);
}

std::optional<std::unordered_set<PreviousLine>> PKBManager::getNextLine(
    const PreviousLine previous_line) {
  return getSetFromMap(pkb_storage->line_previous_line_next_map, previous_line);
}

std::optional<std::unordered_set<PreviousLine>> PKBManager::getPreviousLineT(
    const NextLine next_line) {
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  auto to_visit = getPreviousLine(next_line);
  if (to_visit) {
    for (const auto &neighbour : *to_visit) {
      getPreviousLineTH(neighbour, visited);
    }
    if (visited->size() > 0) {
      return std::make_optional<std::unordered_set<Line>>(*visited.get());
    }
  }
  return std::nullopt;
}

void PKBManager::getPreviousLineTH(
    const Line cur_line, std::shared_ptr<std::unordered_set<Line>> visited) {
  if (visited->find(cur_line) != visited->end()) {
    return;
  }
  visited->insert(cur_line);
  auto to_visit = getPreviousLine(cur_line);
  if (to_visit) {
    for (const auto &neighbour : *to_visit) {
      getPreviousLineTH(neighbour, visited);
    }
  }
}

std::optional<std::unordered_set<PreviousLine>> PKBManager::getNextLineT(
    const PreviousLine previous_line) {
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  auto to_visit = getNextLine(previous_line);
  if (to_visit) {
    for (const auto &neighbour : *to_visit) {
      getNextLineTH(neighbour, visited);
    }
    if (visited->size() > 0) {
      return std::make_optional<std::unordered_set<Line>>(*visited.get());
    }
  }
  return std::nullopt;
}

void PKBManager::getNextLineTH(
    const Line cur_line, std::shared_ptr<std::unordered_set<Line>> visited) {
  if (visited->find(cur_line) != visited->end()) {
    return;
  }
  visited->insert(cur_line);
  auto to_visit = getNextLine(cur_line);
  if (to_visit) {
    for (const auto &neighbour : *to_visit) {
      getNextLineTH(neighbour, visited);
    }
  }
}

bool PKBManager::isLineAffectsLine(const ModifyLine modify_line,
                                   const UsesLine target_line) {
  // check if cache can be utilised
  if (modify_uses_affects_cache.find(modify_line) !=
      modify_uses_affects_cache.end()) {
    // retrieve from cache
    auto uses_lines = modify_uses_affects_cache.at(modify_line);
    // check if target has been reached
    if (uses_lines.find(target_line) != uses_lines.end()) {
      return true;
    }
  } else {
    auto uses_lines = getAffectUsesLine(modify_line);
    if (uses_lines) {
      // check if target has been reached
      if ((*uses_lines).find(target_line) != (*uses_lines).end()) {
        return true;
      }
    }
  }
  // end DFS without finding
  return false;
}

bool PKBManager::isLineAffectsLineT(const ModifyLine modify_line,
                                    const UsesLine uses_line) {
  // check that a1 a2 belong to the same procedure
  if (!isFromSameProcedure(modify_line, uses_line)) {
    return false;
  }
  // check that a1 a2 are both assignment statements
  if (!isAssignExists(modify_line) || !isAssignExists(modify_line)) {
    return false;
  }
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  return isLineAffectsLineTH(modify_line, uses_line, visited);
}

// helper class for isLineAffectsLine DFS
bool PKBManager::isLineAffectsLineTH(
    const ModifyLine modify_line, const UsesLine target_line,
    std::shared_ptr<std::unordered_set<Line>> visited) {
  // check if visited
  if (visited->find(modify_line) != visited->end()) {
    return false;
  } else {
    visited->insert(modify_line);
  }

  // check if cache can be utilised
  if (modify_uses_affects_cache.find(modify_line) !=
      modify_uses_affects_cache.end()) {
    // retrieve from cache
    auto uses_lines = modify_uses_affects_cache.at(modify_line);
    // check if target has been reached
    if (uses_lines.find(target_line) != uses_lines.end()) {
      return true;
    }
    // check what each of those lines are modified by
    for (const auto &line : uses_lines) {
      const bool res = isLineAffectsLineTH(line, target_line, visited);
      if (res) {
        return true;
      }
    }
  } else {
    auto uses_lines = getAffectUsesLine(modify_line);
    if (uses_lines) {
      // check if target has been reached
      if ((*uses_lines).find(target_line) != (*uses_lines).end()) {
        return true;
      }
      // check what each of those lines are modified by
      for (const auto &line : *uses_lines) {
        const bool res = isLineAffectsLineTH(line, target_line, visited);
        if (res) {
          return true;
        }
      }
    }
  }
  // end DFS without finding
  return false;
}

std::optional<std::unordered_set<ModifyLine>> PKBManager::getAffectModifiesLine(
    const UsesLine uses_line) {
  // check what variables the uses_line uses
  auto var_set = getUsesVariableFromAssignLine(uses_line);
  if (var_set) {
    std::shared_ptr<std::unordered_set<Line>> modifies_set =
        std::make_shared<std::unordered_set<Line>>();
    for (const auto &var : *var_set) {
      // do dfs starting from line
      getAffectModifiesLineH(uses_line, var, uses_line, modifies_set);
    }

    if (modifies_set->empty()) {
      return std::nullopt;
    } else {
      return std::make_optional<std::unordered_set<UsesLine>>(
          *modifies_set.get());
    }
  } else {
    return std::nullopt;
  }
}

void PKBManager::getAffectModifiesLineH(
    const Line cur_line, const Variable target_var,
    const ModifyLine source_line,
    std::shared_ptr<std::unordered_set<Line>> modifies_set) {
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  getAffectModifiesLineH(cur_line, target_var, true, source_line, visited,
                         modifies_set);
}

void PKBManager::getAffectModifiesLineH(
    const Line cur_line, const Variable target_var, const bool first_iteration,
    const ModifyLine source_line,
    std::shared_ptr<std::unordered_set<Line>> visited,
    std::shared_ptr<std::unordered_set<Line>> modifies_set) {
  if (visited->find(cur_line) != visited->end()) {
    // node has ben visited before
    // stop traversing down this path
    return;
  } else if (!first_iteration) {
    // add node to visited
    visited->insert(cur_line);
  }

  // ignore on first iteration since line can possibly be like
  // x = x + 1
  if (!first_iteration) {
    // check if line modifies the variable
    if (isLineAffectsVariable(cur_line, target_var)) {
      if (isAssignExists(cur_line)) {
        pkb_storage->addToSetMap(uses_modify_affects_cache, source_line,
                                 cur_line);
        modifies_set->insert(cur_line);
      }
      // stop traversing down this path
      return;
    }
  }

  // traverse down neighbours
  auto neighbours = getPreviousLine(cur_line);
  if (neighbours) {
    for (const auto &neighbour : *neighbours) {
      getAffectModifiesLineH(neighbour, target_var, false, source_line, visited,
                             modifies_set);
    }
  }
}

std::optional<std::unordered_set<ModifyLine>>
PKBManager::getAffectModifiesLineT(const UsesLine uses_line) {
  std::shared_ptr<std::unordered_set<Line>> modifies_set =
      std::make_shared<std::unordered_set<Line>>();
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  getAffectModifiesLineTH(uses_line, modifies_set, visited);
  if (modifies_set->empty()) {
    return std::nullopt;
  } else {
    return std::make_optional<std::unordered_set<ModifyLine>>(
        *modifies_set.get());
  }
}

void PKBManager::getAffectModifiesLineTH(
    const UsesLine uses_line,
    std::shared_ptr<std::unordered_set<ModifyLine>> modifies_set,
    std::shared_ptr<std::unordered_set<Line>> visited) {
  if (visited->find(uses_line) != visited->end()) {
    return;
  } else {
    visited->insert(uses_line);
  }
  if (uses_modify_affects_cache.find(uses_line) !=
      uses_modify_affects_cache.end()) {
    // take from cache
    auto modify_lines = uses_modify_affects_cache.at(uses_line);
    // push each modify line onto the set
    for (const auto &line : modify_lines) {
      modifies_set->insert(line);
    }
    // check what each of those lines are modified by
    for (const auto &line : modify_lines) {
      getAffectModifiesLineTH(line, modifies_set, visited);
    }
  } else {
    auto modify_lines = getAffectModifiesLine(uses_line);
    if (modify_lines) {
      // push each modify line onto the set
      for (const auto &line : *modify_lines) {
        modifies_set->insert(line);
      }
      // check what each of those lines are modified by
      for (const auto &line : *modify_lines) {
        getAffectModifiesLineTH(line, modifies_set, visited);
      }
    }
  }
}

std::optional<std::unordered_set<UsesLine>> PKBManager::getAffectUsesLine(
    const ModifyLine modify_line) {
  // check what variable the modify_line modifies
  auto var = getModifyVariableFromAssignLine(modify_line);
  if (var) {
    std::shared_ptr<std::unordered_set<Line>> uses_set =
        std::make_shared<std::unordered_set<Line>>();
    // do dfs starting from line
    getAffectUsesLineH(modify_line, (*var), modify_line, uses_set);
    if (uses_set->empty()) {
      return std::nullopt;
    } else {
      return std::make_optional<std::unordered_set<UsesLine>>(*uses_set.get());
    }
  } else {
    return std::nullopt;
  }
}

void PKBManager::getAffectUsesLineH(
    const Line cur_line, const Variable target_var,
    const ModifyLine source_line,
    std::shared_ptr<std::unordered_set<Line>> uses_set) {
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  getAffectUsesLineH(cur_line, target_var, true, source_line, visited,
                     uses_set);
}

void PKBManager::getAffectUsesLineH(
    const Line cur_line, const Variable target_var, const bool first_iteration,
    const ModifyLine source_line,
    std::shared_ptr<std::unordered_set<Line>> visited,
    std::shared_ptr<std::unordered_set<Line>> uses_set) {
  if (visited->find(cur_line) != visited->end()) {
    // node has ben visited before
    // stop traversing down this path
    return;
  } else if (!first_iteration) {
    // add node to visited
    visited->insert(cur_line);
  }

  // ignore on first iteration since line can possibly be like
  // x = x + 1
  if (!first_iteration) {
    // check if line uses the variable
    auto var_used = getUsesVariableFromAssignLine(cur_line);
    if (var_used) {
      if ((*var_used).find(target_var) != (*var_used).end()) {
        pkb_storage->addToSetMap(modify_uses_affects_cache, source_line,
                                 cur_line);
        uses_set->insert(cur_line);
      }
    }

    // check if line modifies the variable
    if (isLineAffectsVariable(cur_line, target_var)) {
      // stop traversing down this path
      return;
    }
  }

  // traverse down neighbours
  auto neighbours = getNextLine(cur_line);
  if (neighbours) {
    for (const auto &neighbour : *neighbours) {
      getAffectUsesLineH(neighbour, target_var, false, source_line, visited,
                         uses_set);
    }
  }
}

std::optional<std::unordered_set<UsesLine>> PKBManager::getAffectUsesLineT(
    const ModifyLine modify_line) {
  std::shared_ptr<std::unordered_set<Line>> uses_set =
      std::make_shared<std::unordered_set<Line>>();
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  getAffectUsesLineTH(modify_line, uses_set, visited);
  if (uses_set->empty()) {
    return std::nullopt;
  } else {
    return std::make_optional<std::unordered_set<UsesLine>>(*uses_set.get());
  }
}

void PKBManager::getAffectUsesLineTH(
    const ModifyLine modify_line,
    std::shared_ptr<std::unordered_set<UsesLine>> uses_set,
    std::shared_ptr<std::unordered_set<Line>> visited) {
  if (visited->find(modify_line) != visited->end()) {
    return;
  } else {
    visited->insert(modify_line);
  }
  if (modify_uses_affects_cache.find(modify_line) !=
      modify_uses_affects_cache.end()) {
    // retrieve from cache
    auto uses_lines = modify_uses_affects_cache.at(modify_line);
    // push each modify line onto the set
    for (const auto &line : uses_lines) {
      uses_set->insert(line);
    }
    // check what each of those lines are modified by
    for (const auto &line : uses_lines) {
      getAffectUsesLineTH(line, uses_set, visited);
    }
  } else {
    auto uses_lines = getAffectUsesLine(modify_line);
    if (uses_lines) {
      // push each modify line onto the set
      for (const auto &line : *uses_lines) {
        uses_set->insert(line);
      }
      // check what each of those lines are modified by
      for (const auto &line : *uses_lines) {
        getAffectUsesLineTH(line, uses_set, visited);
      }
    }
  }
}

std::unordered_map<ProcedureCaller, std::unordered_set<ProcedureCallee>>
PKBManager::getCFG() {
  return pkb_storage->line_previous_line_next_map;
};

void PKBManager::clearCache() {
  modify_uses_affects_cache.clear();
  uses_modify_affects_cache.clear();
}

void PKBManager::clearBipCache() {
  modify_uses_affects_bip_cache.clear();
  uses_modify_affects_bip_cache.clear();
}

std::optional<std::unordered_set<NextLine>> PKBManager::getNextLineBip(
    const PreviousLine previous_line) {
  std::unordered_set<NextLine> next_line;
  // if current line is a call statement
  if (isCallExists(previous_line)) {
    // get the procedure that is being called
    auto proc = getProcedureCalleeFromLine(previous_line);
    if (proc) {
      // get first line to that procedure
      const Line first_line = pkb_storage->proc_first_line_map.at(*proc);
      next_line.insert(first_line);
    } else {
      // call statement has to call a procedure
      assert(false);
    }
  } else {
    // non call line
    // get next line as per normal
    auto next_line_normal = getNextLine(previous_line);
    if (next_line_normal) {
      next_line = *next_line_normal;
    } else {
      // reached the end of procedure
      // find out which procedure this line belongs to
      const Procedure proc = pkb_storage->getProcedureFromLine(previous_line);
      // check what procedure's lines were calling it
      if (pkb_storage->procedure_line_calls_map.find(proc) !=
          pkb_storage->procedure_line_calls_map.end()) {
        std::unordered_set<Line> call_lines =
            pkb_storage->procedure_line_calls_map.at(proc);
        for (const auto &line : call_lines) {
          // for each line that was calling it
          // get their next line as per normal
          auto next_line_after_calls = getNextLine(line);
          if (next_line_after_calls) {
            for (const auto &line2 : *next_line_after_calls) {
              next_line.insert(line2);
            }
          }
        }
      }
      // else no procedures were calling it
    }
  }

  if (next_line.empty()) {
    return std::nullopt;
  } else {
    return std::make_optional<std::unordered_set<NextLine>>(next_line);
  }
}

std::optional<std::unordered_set<PreviousLine>> PKBManager::getPreviousLineBip(
    const NextLine next_line) {
  std::unordered_set<PreviousLine> previous_line;
  // get previous line as per normal
  auto previous_lines = getPreviousLine(next_line);
  if (previous_lines) {
    // non starting line
    // check if the previous line is a call
    for (const auto &previous : *previous_lines) {
      if (isCallExists(previous)) {
        // check what procedure it calls to
        auto proc = getProcedureCalleeFromLine(previous);
        if (proc) {
          // previous lines will be the last lines of that procedure
          std::unordered_set<PreviousLine> lines =
              pkb_storage->proc_last_line_map.at(*proc);
          for (const auto &line : lines) {
            previous_line.insert(line);
          }
        } else {
          // call line must call a procedure
          assert(false);
        }
      } else {
        previous_line.insert(previous);
      }
    }
  } else {
    // current line is starting line of a procedure
    // check what lines were calling it
    const Procedure proc = pkb_storage->getProcedureFromLine(next_line);
    if (pkb_storage->procedure_line_calls_map.find(proc) !=
        pkb_storage->procedure_line_calls_map.end()) {
      // if it was being called
      previous_line = pkb_storage->procedure_line_calls_map.at(proc);
    }
  }

  if (previous_line.empty()) {
    return std::nullopt;
  } else {
    return std::make_optional<std::unordered_set<PreviousLine>>(previous_line);
  }
}

std::optional<std::unordered_set<PreviousLine>> PKBManager::getPreviousLineTBip(
    const NextLine next_line) {
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  auto to_visit = getPreviousLineBip(next_line);
  if (to_visit) {
    for (const auto &neighbour : *to_visit) {
      getPreviousLineTBipH(neighbour, visited);
    }
    if (visited->size() > 0) {
      return std::make_optional<std::unordered_set<Line>>(*visited.get());
    }
  }
  return std::nullopt;
}

void PKBManager::getPreviousLineTBipH(
    const Line cur_line, std::shared_ptr<std::unordered_set<Line>> visited) {
  if (visited->find(cur_line) != visited->end()) {
    return;
  }
  visited->insert(cur_line);
  auto to_visit = getPreviousLineBip(cur_line);
  if (to_visit) {
    for (const auto &neighbour : *to_visit) {
      getPreviousLineTBipH(neighbour, visited);
    }
  }
}

std::optional<std::unordered_set<PreviousLine>> PKBManager::getNextLineTBip(
    const PreviousLine previous_line) {
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  auto to_visit = getNextLineBip(previous_line);
  if (to_visit) {
    for (const auto &neighbour : *to_visit) {
      getNextLineTBipH(neighbour, visited);
    }
    if (visited->size() > 0) {
      return std::make_optional<std::unordered_set<Line>>(*visited.get());
    }
  }
  return std::nullopt;
}

void PKBManager::getNextLineTBipH(
    const Line cur_line, std::shared_ptr<std::unordered_set<Line>> visited) {
  if (visited->find(cur_line) != visited->end()) {
    return;
  }
  visited->insert(cur_line);
  auto to_visit = getNextLineBip(cur_line);
  if (to_visit) {
    for (const auto &neighbour : *to_visit) {
      getNextLineTBipH(neighbour, visited);
    }
  }
}

bool PKBManager::isLineAffectsVariableBip(const Line line, const Variable var) {
  // modify in affects can only occur for
  // assign, read and call
  if (isAssignExists(line) || isReadExists(line)) {
    auto var_modified = getVarModifiedByLine(line);
    if (var_modified) {
      return (*var_modified).find(var) != (*var_modified).end();
    }
  }

  return false;
}

std::optional<std::unordered_set<ModifyLine>>
PKBManager::getAffectModifiesLineBip(const UsesLine uses_line) {
  // check what variables the uses_line uses
  auto var_set = getUsesVariableFromAssignLine(uses_line);
  if (var_set) {
    std::shared_ptr<std::unordered_set<Line>> modifies_set =
        std::make_shared<std::unordered_set<Line>>();
    for (const auto &var : *var_set) {
      // do dfs starting from line
      getAffectModifiesLineBipH(uses_line, var, uses_line, modifies_set);
    }

    if (modifies_set->empty()) {
      return std::nullopt;
    } else {
      return std::make_optional<std::unordered_set<UsesLine>>(
          *modifies_set.get());
    }
  } else {
    return std::nullopt;
  }
}

void PKBManager::getAffectModifiesLineBipH(
    const Line cur_line, const Variable target_var,
    const ModifyLine source_line,
    std::shared_ptr<std::unordered_set<Line>> modifies_set) {
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  getAffectModifiesLineBipH(cur_line, target_var, true, source_line, visited,
                            modifies_set);
}

void PKBManager::getAffectModifiesLineBipH(
    const Line cur_line, const Variable target_var, const bool first_iteration,
    const ModifyLine source_line,
    std::shared_ptr<std::unordered_set<Line>> visited,
    std::shared_ptr<std::unordered_set<Line>> modifies_set) {
  if (visited->find(cur_line) != visited->end()) {
    // node has ben visited before
    // stop traversing down this path
    return;
  } else if (!first_iteration) {
    // add node to visited
    visited->insert(cur_line);
  }

  // ignore on first iteration since line can possibly be like
  // x = x + 1
  if (!first_iteration) {
    // check if line modifies the variable
    if (isLineAffectsVariableBip(cur_line, target_var)) {
      if (isAssignExists(cur_line)) {
        pkb_storage->addToSetMap(uses_modify_affects_bip_cache, source_line,
                                 cur_line);
        modifies_set->insert(cur_line);
      }
      // stop traversing down this path
      return;
    }
  }

  // traverse down neighbours
  auto neighbours = getPreviousLineBip(cur_line);
  if (neighbours) {
    for (const auto &neighbour : *neighbours) {
      getAffectModifiesLineBipH(neighbour, target_var, false, source_line,
                                visited, modifies_set);
    }
  }
}

std::optional<std::unordered_set<UsesLine>> PKBManager::getAffectUsesLineBip(
    const ModifyLine modify_line) {
  // check what variable the modify_line modifies
  auto var = getModifyVariableFromAssignLine(modify_line);
  if (var) {
    std::shared_ptr<std::unordered_set<Line>> uses_set =
        std::make_shared<std::unordered_set<Line>>();
    // do dfs starting from line
    getAffectUsesLineBipH(modify_line, (*var), modify_line, uses_set);
    if (uses_set->empty()) {
      return std::nullopt;
    } else {
      return std::make_optional<std::unordered_set<UsesLine>>(*uses_set.get());
    }
  } else {
    return std::nullopt;
  }
}

void PKBManager::getAffectUsesLineBipH(
    const Line cur_line, const Variable target_var,
    const ModifyLine source_line,
    std::shared_ptr<std::unordered_set<Line>> uses_set) {
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  getAffectUsesLineBipH(cur_line, target_var, true, source_line, visited,
                        uses_set);
}

void PKBManager::getAffectUsesLineBipH(
    const Line cur_line, const Variable target_var, const bool first_iteration,
    const ModifyLine source_line,
    std::shared_ptr<std::unordered_set<Line>> visited,
    std::shared_ptr<std::unordered_set<Line>> uses_set) {
  if (visited->find(cur_line) != visited->end()) {
    // node has ben visited before
    // stop traversing down this path
    return;
  } else if (!first_iteration) {
    // add node to visited
    visited->insert(cur_line);
  }

  // ignore on first iteration since line can possibly be like
  // x = x + 1
  if (!first_iteration) {
    // check if line uses the variable
    auto var_used = getUsesVariableFromAssignLine(cur_line);
    if (var_used) {
      if ((*var_used).find(target_var) != (*var_used).end()) {
        pkb_storage->addToSetMap(modify_uses_affects_bip_cache, source_line,
                                 cur_line);
        uses_set->insert(cur_line);
      }
    }

    // check if line modifies the variable
    if (isLineAffectsVariableBip(cur_line, target_var)) {
      // stop traversing down this path
      return;
    }
  }

  // traverse down neighbours
  auto neighbours = getNextLineBip(cur_line);
  if (neighbours) {
    for (const auto &neighbour : *neighbours) {
      getAffectUsesLineBipH(neighbour, target_var, false, source_line, visited,
                            uses_set);
    }
  }
}

std::optional<std::unordered_set<UsesLine>> PKBManager::getAffectUsesLineTBip(
    const ModifyLine modify_line) {
  std::shared_ptr<std::unordered_set<Line>> uses_set =
      std::make_shared<std::unordered_set<Line>>();
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  getAffectUsesLineTBipH(modify_line, uses_set, visited);
  if (uses_set->empty()) {
    return std::nullopt;
  } else {
    return std::make_optional<std::unordered_set<UsesLine>>(*uses_set.get());
  }
}

void PKBManager::getAffectUsesLineTBipH(
    const ModifyLine modify_line,
    std::shared_ptr<std::unordered_set<UsesLine>> uses_set,
    std::shared_ptr<std::unordered_set<Line>> visited) {
  if (visited->find(modify_line) != visited->end()) {
    return;
  } else {
    visited->insert(modify_line);
  }
  if (modify_uses_affects_bip_cache.find(modify_line) !=
      modify_uses_affects_bip_cache.end()) {
    // retrieve from cache
    auto uses_lines = modify_uses_affects_bip_cache.at(modify_line);
    // push each modify line onto the set
    for (const auto &line : uses_lines) {
      uses_set->insert(line);
    }
    // check what each of those lines are modified by
    for (const auto &line : uses_lines) {
      getAffectUsesLineTBipH(line, uses_set, visited);
    }
  } else {
    auto uses_lines = getAffectUsesLineBip(modify_line);
    if (uses_lines) {
      // push each modify line onto the set
      for (const auto &line : *uses_lines) {
        uses_set->insert(line);
      }
      // check what each of those lines are modified by
      for (const auto &line : *uses_lines) {
        getAffectUsesLineTBipH(line, uses_set, visited);
      }
    }
  }
}

std::optional<std::unordered_set<ModifyLine>>
PKBManager::getAffectModifiesLineTBip(const UsesLine uses_line) {
  std::shared_ptr<std::unordered_set<Line>> modifies_set =
      std::make_shared<std::unordered_set<Line>>();
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  getAffectModifiesLineTBipH(uses_line, modifies_set, visited);
  if (modifies_set->empty()) {
    return std::nullopt;
  } else {
    return std::make_optional<std::unordered_set<ModifyLine>>(
        *modifies_set.get());
  }
}

void PKBManager::getAffectModifiesLineTBipH(
    const UsesLine uses_line,
    std::shared_ptr<std::unordered_set<ModifyLine>> modifies_set,
    std::shared_ptr<std::unordered_set<Line>> visited) {
  if (visited->find(uses_line) != visited->end()) {
    return;
  } else {
    visited->insert(uses_line);
  }
  if (uses_modify_affects_bip_cache.find(uses_line) !=
      uses_modify_affects_bip_cache.end()) {
    // take from cache
    auto modify_lines = uses_modify_affects_bip_cache.at(uses_line);
    // push each modify line onto the set
    for (const auto &line : modify_lines) {
      modifies_set->insert(line);
    }
    // check what each of those lines are modified by
    for (const auto &line : modify_lines) {
      getAffectModifiesLineTBipH(line, modifies_set, visited);
    }
  } else {
    auto modify_lines = getAffectModifiesLineBip(uses_line);
    if (modify_lines) {
      // push each modify line onto the set
      for (const auto &line : *modify_lines) {
        modifies_set->insert(line);
      }
      // check what each of those lines are modified by
      for (const auto &line : *modify_lines) {
        getAffectModifiesLineTBipH(line, modifies_set, visited);
      }
    }
  }
}

bool PKBManager::isLineAffectsLineBip(const ModifyLine modify_line,
                                      const UsesLine target_line) {
  // check if cache can be utilised
  if (modify_uses_affects_cache.find(modify_line) !=
      modify_uses_affects_cache.end()) {
    // retrieve from cache
    auto uses_lines = modify_uses_affects_cache.at(modify_line);
    // check if target has been reached
    if (uses_lines.find(target_line) != uses_lines.end()) {
      return true;
    }
  } else {
    auto uses_lines = getAffectUsesLineBip(modify_line);
    if (uses_lines) {
      // check if target has been reached
      if ((*uses_lines).find(target_line) != (*uses_lines).end()) {
        return true;
      }
    }
  }
  // end DFS without finding
  return false;
}

bool PKBManager::isLineAffectsLineTBip(const ModifyLine modify_line,
                                       const UsesLine uses_line) {
  // check that a1 a2 are both assignment statements
  if (!isAssignExists(modify_line) || !isAssignExists(modify_line)) {
    return false;
  }
  std::shared_ptr<std::unordered_set<Line>> visited =
      std::make_shared<std::unordered_set<Line>>();
  return isLineAffectsLineTBipH(modify_line, uses_line, visited);
}

// helper class for isLineAffectsLine DFS
bool PKBManager::isLineAffectsLineTBipH(
    const ModifyLine modify_line, const UsesLine target_line,
    std::shared_ptr<std::unordered_set<Line>> visited) {
  // check if visited
  if (visited->find(modify_line) != visited->end()) {
    return false;
  } else {
    visited->insert(modify_line);
  }

  // check if cache can be utilised
  if (modify_uses_affects_bip_cache.find(modify_line) !=
      modify_uses_affects_bip_cache.end()) {
    // retrieve from cache
    auto uses_lines = modify_uses_affects_bip_cache.at(modify_line);
    // check if target has been reached
    if (uses_lines.find(target_line) != uses_lines.end()) {
      return true;
    }
    // check what each of those lines are modified by
    for (const auto &line : uses_lines) {
      const bool res = isLineAffectsLineTBipH(line, target_line, visited);
      if (res) {
        return true;
      }
    }
  } else {
    auto uses_lines = getAffectUsesLineBip(modify_line);
    if (uses_lines) {
      // check if target has been reached
      if ((*uses_lines).find(target_line) != (*uses_lines).end()) {
        return true;
      }
      // check what each of those lines are modified by
      for (const auto &line : *uses_lines) {
        const bool res = isLineAffectsLineTBipH(line, target_line, visited);
        if (res) {
          return true;
        }
      }
    }
  }
  // end DFS without finding
  return false;
}

bool PKBManager::isLineNextLineBip(const PreviousLine previous_line,
    const NextLine next_line) {
  auto next_lines = getNextLineBip(previous_line);
  if (next_lines) {
    if ((*next_lines).find(next_line) != (*next_lines).end()) {
      return true;
    }
  }

  return false;
}

bool PKBManager::isLineNextLineTBip(const PreviousLine previous_line,
    const NextLine next_line) {
  auto next_lines_t = getNextLineTBip(previous_line);
  if (next_lines_t) {
    if ((*next_lines_t).find(next_line) != (*next_lines_t).end()) {
      return true;
    }
  }

  return false;
}

}  // namespace PKB
