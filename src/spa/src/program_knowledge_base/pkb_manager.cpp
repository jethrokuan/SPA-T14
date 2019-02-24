#include "program_knowledge_base/pkb_manager.h"

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

// helper
std::optional<std::vector<std::string>> PKBManager::getUniqueVectorFromMap(
    const std::unordered_map<std::string, std::vector<std::string>> &umap,
    const std::string key) {
  return (umap.find(key) == umap.end())
             ? std::nullopt
             : std::make_optional<std::vector<std::string>>(
                   getUniqueVector(umap.at(key)));
}

std::vector<std::string> PKBManager::getUniqueVector(
    std::vector<std::string> dupe_vec) {
  std::unordered_set<std::string> unique_set(dupe_vec.begin(), dupe_vec.end());
  std::vector<std::string> unique_vec(unique_set.begin(), unique_set.end());

  return unique_vec;
}

// is design entity set empty
bool PKBManager::isVariableSetEmpty() { return pkb_storage->var_set.empty(); }

bool PKBManager::isAssignSetEmpty() { return pkb_storage->assign_set.empty(); }

bool PKBManager::isStatementSetEmpty() {
  return pkb_storage->statement_set.empty();
}

bool PKBManager::isPrintSetEmpty() { return pkb_storage->print_set.empty(); }

bool PKBManager::isReadSetEmpty() { return pkb_storage->read_set.empty(); }

bool PKBManager::isWhileSetEmpty() { return pkb_storage->while_set.empty(); }

bool PKBManager::isIfSetEmpty() { return pkb_storage->if_set.empty(); }

bool PKBManager::isConstantSetEmpty() {
  return pkb_storage->constant_set.empty();
}

bool PKBManager::isProcedureSetEmpty() {
  return pkb_storage->procedure_set.empty();
}

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

// get design entities

std::vector<Line> PKBManager::getVariableList() {
  return getUniqueVector(pkb_storage->var_list);
}

std::vector<Line> PKBManager::getAssignList() {
  return getUniqueVector(pkb_storage->assign_list);
}

std::vector<Line> PKBManager::getStatementList() {
  return getUniqueVector(pkb_storage->statement_list);
}

std::vector<Line> PKBManager::getPrintList() {
  return getUniqueVector(pkb_storage->print_list);
}

std::vector<Line> PKBManager::getReadList() {
  return getUniqueVector(pkb_storage->read_list);
}

std::vector<Procedure> PKBManager::getWhileList() {
  return getUniqueVector(pkb_storage->while_list);
}

std::vector<Procedure> PKBManager::getIfList() {
  return getUniqueVector(pkb_storage->if_list);
}

std::vector<Procedure> PKBManager::getConstantList() {
  return getUniqueVector(pkb_storage->constant_list);
}

std::vector<Procedure> PKBManager::getProcedureList() {
  return getUniqueVector(pkb_storage->procedure_list);
}

// is relationship set empty
bool PKBManager::isLineFollowLineSetEmpty() {
  return pkb_storage->follows_set.empty();
}

bool PKBManager::isLineFollowLineSSetEmpty() {
  return pkb_storage->follows_set_s.empty();
}

bool PKBManager::isLineParentLineSetEmpty() {
  return pkb_storage->parent_set.empty();
}

bool PKBManager::isLineParentLineSSetEmpty() {
  return pkb_storage->parent_set_s.empty();
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

bool PKBManager::isLineFollowLine(const LineBefore line_before,
                                  const LineAfter line_after) {
  return pkb_storage->follows_set.find(std::pair<LineBefore, LineAfter>(
             line_before, line_after)) != pkb_storage->follows_set.end();
}

bool PKBManager::isLineFollowLineS(const LineBefore line_before,
                                   const LineAfter line_after) {
  return pkb_storage->follows_set_s.find(std::pair<LineBefore, LineAfter>(
             line_before, line_after)) != pkb_storage->follows_set.end();
}

bool PKBManager::isLineParentLine(const ParentLine parent_line,
                                  const ChildLine child_line) {
  return pkb_storage->parent_set.find(std::pair<ParentLine, ChildLine>(
             parent_line, child_line)) != pkb_storage->follows_set.end();
}

bool PKBManager::isLineParentLineS(const ParentLine parent_line,
                                   const ChildLine child_line) {
  return pkb_storage->parent_set_s.find(std::pair<ParentLine, ChildLine>(
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

std::optional<std::vector<LineAfter>> PKBManager::getFollowingLineS(
    const LineBefore line_before) {
  return getUniqueVectorFromMap(pkb_storage->line_before_line_after_map_s,
                                line_before);
}

std::optional<std::vector<LineBefore>> PKBManager::getBeforeLineS(
    const LineAfter line_after) {
  return getUniqueVectorFromMap(pkb_storage->line_after_line_before_map_s,
                                line_after);
}

std::optional<ParentLine> PKBManager::getParentLine(
    const ChildLine child_line) {
  return (pkb_storage->child_line_parent_line_map.find(child_line) ==
          pkb_storage->child_line_parent_line_map.end())
             ? std::nullopt
             : std::make_optional<ParentLine>(
                   pkb_storage->child_line_parent_line_map[child_line]);
}

std::optional<std::vector<ChildLine>> PKBManager::getChildLine(
    const ParentLine parent_line) {
  return getUniqueVectorFromMap(pkb_storage->parent_line_child_line_map,
                                parent_line);
}

std::optional<std::vector<ParentLine>> PKBManager::getParentLineS(
    const ChildLine child_line) {
  return getUniqueVectorFromMap(pkb_storage->child_line_parent_line_map_s,
                                child_line);
}

std::optional<std::vector<ChildLine>> PKBManager::getChildLineS(
    const ParentLine parent_line) {
  return getUniqueVectorFromMap(pkb_storage->parent_line_child_line_map_s,
                                parent_line);
}

std::optional<std::vector<Procedure>> PKBManager::getProcedureUsesVar(
    const Variable var) {
  return getUniqueVectorFromMap(pkb_storage->procedure_uses_var_map, var);
}

std::optional<std::vector<Line>> PKBManager::getLineUsesVar(
    const Variable var) {
  return getUniqueVectorFromMap(pkb_storage->line_uses_var_map, var);
}

std::optional<std::vector<Variable>> PKBManager::getVarUsedByProcedure(
    const Procedure proc) {
  return getUniqueVectorFromMap(pkb_storage->var_used_by_procedure_map, proc);
}

std::optional<std::vector<Variable>> PKBManager::getVarUsedByLine(
    const Line line) {
  return getUniqueVectorFromMap(pkb_storage->var_used_by_line_map, line);
}

std::optional<std::vector<Variable>> PKBManager::getVarModifiedByProcedure(
    const Procedure proc) {
  return getUniqueVectorFromMap(pkb_storage->var_modified_by_procedure_map,
                                proc);
}

std::optional<std::vector<Variable>> PKBManager::getVarModifiedByLine(
    const Line line) {
  return getUniqueVectorFromMap(pkb_storage->var_modified_by_line_map, line);
}

std::optional<std::vector<Procedure>> PKBManager::getProcedureModifiesVar(
    const Variable var) {
  return getUniqueVectorFromMap(pkb_storage->procedure_modifies_var_map, var);
}

std::optional<std::vector<Line>> PKBManager::getLineModifiesVar(
    const Variable var) {
  return getUniqueVectorFromMap(pkb_storage->line_modifies_var_map, var);
}

// TODO tidy this up
std::optional<std::vector<Line>> PKBManager::getCompleteMatchLinesWithVar(
    const Variable var, const Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);

  // check if the variable exists
  if (pkb_storage->var_set.find(var) == pkb_storage->var_set.end()) {
    return std::nullopt;
  }

  // TODO check if this is even necessary?
  // check if pattern exists
  if (pkb_storage->expr_str_set.find(pattern_expr) ==
      pkb_storage->expr_str_set.end()) {
    return std::nullopt;
  }

  // retrieve list of patterns for the variable
  auto ys = pkb_storage->var_expr_str_map.at(var);
  std::vector<Line> matching_lines;
  for (const auto &elem : ys) {
    auto line = elem.first;
    auto elem_pattern_expr = elem.second;
    if (pattern_expr.compare(elem_pattern_expr) == 0) {
      matching_lines.push_back(line);
    }
  }

  if (matching_lines.size() == 0) {
    return std::nullopt;
  } else {
    return std::make_optional<std::vector<Line>>(matching_lines);
  }
}

std::optional<std::vector<Line>> PKBManager::getPartialMatchLinesWithVar(
    const Variable var, const Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);

  // check if the variable exists
  if (pkb_storage->var_set.find(var) == pkb_storage->var_set.end()) {
    return std::nullopt;
  }

  // retrieve list of patterns for the variable
  auto ys = pkb_storage->var_expr_str_map.at(var);
  std::vector<Line> matching_lines;
  for (const auto &elem : ys) {
    auto line = elem.first;
    auto elem_pattern_expr = elem.second;
    if (elem_pattern_expr.find(pattern_expr) != std::string::npos) {
      matching_lines.push_back(line);
    }
  }

  if (matching_lines.size() == 0) {
    return std::nullopt;
  } else {
    return std::make_optional<std::vector<Line>>(matching_lines);
  }
}

std::optional<std::vector<Line>> PKBManager::getCompleteMatchLines(
    const Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);

  // TODO check if this is even necessary?
  // check if pattern exists
  if (pkb_storage->expr_str_set.find(pattern_expr) ==
      pkb_storage->expr_str_set.end()) {
    return std::nullopt;
  }

  std::vector<Line> matching_lines;
  for (const auto &elem : pkb_storage->line_expr_str_set) {
    auto line = elem.first;
    auto elem_pattern_expr = elem.second;
    // substring
    if (pattern_expr.compare(elem_pattern_expr) == 0) {
      matching_lines.push_back(line);
    }
  }

  if (matching_lines.size() == 0) {
    return std::nullopt;
  } else {
    return std::make_optional<std::vector<Line>>(matching_lines);
  }
}

std::optional<std::vector<Line>> PKBManager::getPartialMatchLines(
    const Pattern pattern) {
  Expr expr = SimpleInterface::parseExpression(pattern);
  ExprStr pattern_expr =
      std::visit([](const auto &s) { return s->to_str(); }, expr);

  std::vector<Line> matching_lines;
  for (const auto &elem : pkb_storage->line_expr_str_set) {
    auto line = elem.first;
    auto elem_pattern_expr = elem.second;
    // substring
    if (elem_pattern_expr.find(pattern_expr) != std::string::npos) {
      matching_lines.push_back(line);
    }
  }

  if (matching_lines.size() == 0) {
    return std::nullopt;
  } else {
    return std::make_optional<std::vector<Line>>(matching_lines);
  }
}

}  // namespace PKB
