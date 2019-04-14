#include "program_knowledge_base/pkb_preprocessor.h"

namespace PKB {

PKBPreprocessor::PKBPreprocessor(const AST ast,
                                 std::shared_ptr<PKBStorage> pkb_storage) {
  storage = pkb_storage;
  setLineNumbers(ast);
  setCFG(ast);
  setDesignEntities(ast);
  setCallsRelations(ast);
  setFollowsRelations(ast);
  setParentRelations(ast);
  setUsesRelations(ast);
  setModifiesRelations(ast);
  setPattern(ast);
  setAssign(ast);
  setProcedureStartAndEnd(ast);
  setCFGBip();
}

PKBPreprocessor::~PKBPreprocessor() {}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<RootNode> node) {
  for (const auto &proc : node->ProcList) {
    setLineNumbers(proc);
  }
}

void PKBPreprocessor::setLineNumbers(
    const std::shared_ptr<ProcedureNode> node) {
  setLineNumbersIterator(node->StmtList, node->Name);
}

void PKBPreprocessor::setLineNumbers(const StmtNode node,
                                     const Procedure proc) {
  const Line line_number = storage->storeLine(node);
  storage->storeLineProcedureRelation(line_number, proc);
  std::visit(
      [this, proc](const auto &s) {
        using T = std::decay_t<decltype(s)>;
        if constexpr (std::is_same_v<T, std::shared_ptr<IfNode>>) {
          setLineNumbersIterator(s->StmtListThen, proc);
          setLineNumbersIterator(s->StmtListElse, proc);
        } else if constexpr (std::is_same_v<T, std::shared_ptr<WhileNode>>) {
          setLineNumbersIterator(s->StmtList, proc);
        }
      },
      node);
}

void PKBPreprocessor::setLineNumbersIterator(
    const std::vector<StmtNode> stmt_lst, const Procedure proc) {
  // iterate through AST via DFS
  for (const auto &stmt : stmt_lst) {
    setLineNumbers(stmt, proc);
  }
}

void PKBPreprocessor::setCFG(const std::shared_ptr<RootNode> node) {
  for (const auto &proc : node->ProcList) {
    setCFG(proc);
  }
}

void PKBPreprocessor::setCFG(const std::shared_ptr<ProcedureNode> node) {
  std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache =
      std::make_shared<std::unordered_map<Line, std::unordered_set<Line>>>();
  setCFGIterator(node->StmtList, cache);
}

void PKBPreprocessor::setCFG(
    const std::shared_ptr<IfNode> node,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache) {
  const ParentLine parent_line = storage->getLineFromNode(node);
  const ChildLine then_child_line =
      storage->getLineFromNode(node->StmtListThen.front());
  const ChildLine else_child_line =
      storage->getLineFromNode(node->StmtListElse.front());
  storage->storeCFGEdge(parent_line, then_child_line);
  storage->storeCFGEdge(parent_line, else_child_line);

  setCFGIterator(node->StmtListThen, cache);
  setCFGIterator(node->StmtListElse, cache);
}

void PKBPreprocessor::setCFG(
    const std::shared_ptr<WhileNode> node,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache) {
  const ParentLine parent_line = storage->getLineFromNode(node);
  const ChildLine child_line = storage->getLineFromNode(node->StmtList.front());
  auto terminating = getTerminatingLines(node->StmtList.back(), cache);
  for (const auto &line : terminating) {
    storage->storeCFGEdge(line, parent_line);
  }
  storage->storeCFGEdge(parent_line, child_line);
  setCFGIterator(node->StmtList, cache);
}

void PKBPreprocessor::setCFG(
    const std::shared_ptr<ReadNode>,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>) {}

void PKBPreprocessor::setCFG(
    const std::shared_ptr<PrintNode>,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>) {}

void PKBPreprocessor::setCFG(
    const std::shared_ptr<AssignNode>,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>) {}

void PKBPreprocessor::setCFG(
    const std::shared_ptr<CallNode>,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>>) {}

void PKBPreprocessor::setCFGIterator(
    const std::vector<StmtNode> stmt_lst,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this, cache](const auto &s) { setCFG(s, cache); }, stmt);
  }

  // iterate through statement nodes
  for (std::size_t i = 0; i < stmt_lst.size() - 1; i++) {
    // compare 2 nodes at once i and i + 1
    auto terminating = getTerminatingLines(stmt_lst[i], cache);
    const Line cur_line = storage->getLineFromNode(stmt_lst[i + 1]);

    for (const auto &line : terminating) {
      storage->storeCFGEdge(line, cur_line);
    }
  }
}

std::unordered_set<Line> PKBPreprocessor::getTerminatingLines(
    const std::shared_ptr<IfNode> node,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache) {
  const Line cur_line = storage->getLineFromNode(node);
  if (cache->find(cur_line) != cache->end()) {
    // cache hit
    return cache->at(cur_line);
  }
  // get the last node in its statement list
  auto last_item_then = node->StmtListThen.back();
  auto last_item_else = node->StmtListElse.back();
  auto terminating_then = getTerminatingLines(last_item_then, cache);
  auto terminating_else = getTerminatingLines(last_item_else, cache);
  std::unordered_set<Line> terminating;
  for (const auto &line : terminating_then) {
    terminating.insert(line);
  }
  for (const auto &line : terminating_else) {
    terminating.insert(line);
  }
  // store in cache
  cache->emplace(cur_line, terminating);

  return terminating;
}

std::unordered_set<Line> PKBPreprocessor::getTerminatingLines(
    const std::shared_ptr<WhileNode> node,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache) {
  const Line cur_line = storage->getLineFromNode(node);
  if (cache->find(cur_line) != cache->end()) {
    // cache hit
    return cache->at(cur_line);
  }

  std::unordered_set<Line> terminating_lines;
  terminating_lines.emplace(cur_line);
  cache->emplace(cur_line, terminating_lines);

  return terminating_lines;
}

std::unordered_set<Line> PKBPreprocessor::getTerminatingLines(
    const std::shared_ptr<ReadNode> node,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache) {
  const Line cur_line = storage->getLineFromNode(node);
  if (cache->find(cur_line) != cache->end()) {
    // cache hit
    return cache->at(cur_line);
  }
  // store in cache
  std::unordered_set<Line> terminating_lines;
  terminating_lines.emplace(cur_line);
  cache->emplace(cur_line, terminating_lines);
  return terminating_lines;
}

std::unordered_set<Line> PKBPreprocessor::getTerminatingLines(
    const std::shared_ptr<PrintNode> node,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache) {
  const Line cur_line = storage->getLineFromNode(node);
  if (cache->find(cur_line) != cache->end()) {
    // cache hit
    return cache->at(cur_line);
  }
  // store in cache
  std::unordered_set<Line> terminating_lines;
  terminating_lines.emplace(cur_line);
  cache->emplace(cur_line, terminating_lines);
  return terminating_lines;
}

std::unordered_set<Line> PKBPreprocessor::getTerminatingLines(
    const std::shared_ptr<AssignNode> node,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache) {
  const Line cur_line = storage->getLineFromNode(node);
  if (cache->find(cur_line) != cache->end()) {
    // cache hit
    return cache->at(cur_line);
  }
  // store in cache
  std::unordered_set<Line> terminating_lines;
  terminating_lines.emplace(cur_line);
  cache->emplace(cur_line, terminating_lines);
  return terminating_lines;
}

std::unordered_set<Line> PKBPreprocessor::getTerminatingLines(
    const std::shared_ptr<CallNode> node,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache) {
  const Line cur_line = storage->getLineFromNode(node);
  if (cache->find(cur_line) != cache->end()) {
    // cache hit
    return cache->at(cur_line);
  }
  // store in cache
  std::unordered_set<Line> terminating_lines;
  terminating_lines.emplace(cur_line);
  cache->emplace(cur_line, terminating_lines);
  return terminating_lines;
}

std::unordered_set<Line> PKBPreprocessor::getTerminatingLines(
    const StmtNode node,
    std::shared_ptr<std::unordered_map<Line, std::unordered_set<Line>>> cache) {
  return std::visit(
      [this, cache](const auto &n) { return getTerminatingLines(n, cache); },
      node);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<RootNode> node) {
  setDesignEntitiesProcListIterator(node->ProcList);
  for (const auto &proc : node->ProcList) {
    setDesignEntities(proc);
  }
}

void PKBPreprocessor::setDesignEntitiesProcListIterator(
    const std::vector<std::shared_ptr<ProcedureNode>> proc_lst) {
  for (const auto &proc : proc_lst) {
    storage->storeProcedure(proc->Name);
  }
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<ProcedureNode> node) {
  setDesignEntitiesIterator(node->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<IfNode> node) {
  const Line cur_line = storage->getLineFromNode(node);
  storage->storeStatement(cur_line);
  storage->storeIf(cur_line);
  setDesignEntities(node->CondExpr);
  setDesignEntitiesIterator(node->StmtListThen);
  setDesignEntitiesIterator(node->StmtListElse);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<WhileNode> node) {
  const Line cur_line = storage->getLineFromNode(node);
  storage->storeStatement(cur_line);
  storage->storeWhile(cur_line);
  setDesignEntities(node->CondExpr);
  setDesignEntitiesIterator(node->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<ReadNode> node) {
  const Line cur_line = storage->getLineFromNode(node);
  storage->storeStatement(cur_line);
  storage->storeRead(cur_line, node->Var->Name);
  setDesignEntities(node->Var);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<PrintNode> node) {
  const Line cur_line = storage->getLineFromNode(node);
  storage->storeStatement(cur_line);
  storage->storePrint(cur_line, node->Var->Name);
  setDesignEntities(node->Var);
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<AssignNode> node) {
  const Line cur_line = storage->getLineFromNode(node);
  storage->storeStatement(cur_line);
  storage->storeAssign(cur_line);
  setDesignEntities(node->Var);
  setDesignEntities(node->Exp);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<CallNode> node) {
  const Line cur_line = storage->getLineFromNode(node);
  storage->storeStatement(cur_line);
  storage->storeCall(cur_line, node->ProcName);
}

void PKBPreprocessor::setDesignEntities(const Expr node) {
  std::visit([this](const auto &n) { setDesignEntities(n); }, node);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<BinOpNode> node) {
  setDesignEntities(node->Left);
  setDesignEntities(node->Right);
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<CondExprNode> node) {
  if (node == nullptr) {
    return;
  }
  setDesignEntities(node->RelExpr);
  setDesignEntities(node->CondLHS);
  setDesignEntities(node->CondRHS);
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<RelExprNode> node) {
  if (node == nullptr) {
    return;
  }
  setDesignEntities(node->LHS);
  setDesignEntities(node->RHS);
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<NumberNode> node) {
  storage->storeConstant(node->Val);
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<VariableNode> node) {
  storage->storeVariable(node->Name);
}

void PKBPreprocessor::setDesignEntitiesIterator(
    const std::vector<StmtNode> stmt_lst) {
  // iterate through AST via DFS
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setDesignEntities(s); }, stmt);
  }
}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<RootNode> node) {
  // set direct relations
  for (const auto &proc : node->ProcList) {
    setCallsRelations(proc);
  }
  // set indirect relations after direct relations have been set
  setCallsIndirectRelations();
}

void PKBPreprocessor::setCallsIndirectRelations() {
  // make a copy of procedure_calls_procedure_set which contains direct
  // relations
  auto direct_procedure_calls_procedure_sets =
      std::unordered_set<std::pair<ProcedureCaller, ProcedureCallee>,
                         pair_hash>(
          storage->procedure_calls_procedure_set.begin(),
          storage->procedure_calls_procedure_set.end());
  for (const auto &direct_call : direct_procedure_calls_procedure_sets) {
    const ProcedureCaller proc_caller = direct_call.first;
    const ProcedureCallee proc_callee = direct_call.second;
    setCallsIndirectRelationsH(proc_caller, proc_callee);
  }
}

void PKBPreprocessor::setCallsIndirectRelationsH(
    const ProcedureCaller proc_caller, const ProcedureCallee proc_callee) {
  bool has_indirect =
      storage->procedure_caller_procedure_callee_map.find(proc_callee) !=
      storage->procedure_caller_procedure_callee_map.end();
  if (has_indirect) {
    auto proc_callee_callee_set =
        storage->procedure_caller_procedure_callee_map.at(proc_callee);
    for (const auto &proc_callee_callee : proc_callee_callee_set) {
      storage->storeCallsRelationS(proc_caller, proc_callee_callee);
      setCallsIndirectRelationsH(proc_caller, proc_callee_callee);
    }
  }
}

void PKBPreprocessor::setCallsRelations(
    const std::shared_ptr<ProcedureNode> node) {
  setCallsRelationsIterator(node->StmtList);
}
void PKBPreprocessor::setCallsRelations(const std::shared_ptr<IfNode> node) {
  setCallsRelationsIterator(node->StmtListThen);
  setCallsRelationsIterator(node->StmtListElse);
}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<WhileNode> node) {
  setCallsRelationsIterator(node->StmtList);
}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<ReadNode>) {}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<PrintNode>) {}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<AssignNode>) {}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<CallNode> node) {
  const Line line_number = storage->getLineFromNode(node);
  const Procedure proc_caller = storage->getProcedureFromLine(line_number);
  storage->storeCallsRelation(proc_caller, node->ProcName);
  storage->storeCallsRelationS(proc_caller, node->ProcName);
}

void PKBPreprocessor::setCallsRelationsIterator(
    const std::vector<StmtNode> stmt_lst) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setCallsRelations(s); }, stmt);
  }
}

void PKBPreprocessor::setFollowsRelations(
    const std::shared_ptr<RootNode> node) {
  for (const auto &proc : node->ProcList) {
    setFollowsRelations(proc);
  }
}

void PKBPreprocessor::setFollowsRelations(
    const std::shared_ptr<ProcedureNode> node) {
  setFollowsRelationsIterator(node->StmtList);
}

void PKBPreprocessor::setFollowsRelations(const std::shared_ptr<IfNode> node) {
  setFollowsRelationsIterator(node->StmtListThen);
  setFollowsRelationsIterator(node->StmtListElse);
}

void PKBPreprocessor::setFollowsRelations(
    const std::shared_ptr<WhileNode> node) {
  setFollowsRelationsIterator(node->StmtList);
}

void PKBPreprocessor::setFollowsRelationsIterator(
    const std::vector<StmtNode> stmt_lst) {
  // add direct relations
  for (std::size_t i = 0; i < stmt_lst.size() - 1; i++) {
    const Line cur_line_number = storage->getLineFromNode(stmt_lst[i]);
    const Line next_line_number = storage->getLineFromNode(stmt_lst[i + 1]);
    storage->storeFollowsRelation(cur_line_number, next_line_number);
  }
  // add indirect relations
  for (std::size_t i = 0; i < stmt_lst.size() - 1; i++) {
    for (std::size_t j = i + 1; j < stmt_lst.size(); j++) {
      const Line cur_line_number = storage->getLineFromNode(stmt_lst[i]);
      const Line next_line_number = storage->getLineFromNode(stmt_lst[j]);
      storage->storeFollowsRelationS(cur_line_number, next_line_number);
    }
  }

  // iterate through AST via DFS
  for (const auto &stmt : stmt_lst) {
    std::visit(
        [this](const auto &s) {
          using T = std::decay_t<decltype(s)>;
          if constexpr (std::is_same_v<T, std::shared_ptr<ProcedureNode>> ||
                        std::is_same_v<T, std::shared_ptr<IfNode>> ||
                        std::is_same_v<T, std::shared_ptr<WhileNode>>) {
            setFollowsRelations(s);
          }
        },
        stmt);
  }
}

void PKBPreprocessor::setParentRelations(const std::shared_ptr<RootNode> node) {
  for (const auto &proc : node->ProcList) {
    setParentRelations(proc);
  }
}

void PKBPreprocessor::setParentRelations(
    const std::shared_ptr<ProcedureNode> node) {
  std::vector<ParentLine> parent_lines;
  setParentRelationsH(node, parent_lines);
}

void PKBPreprocessor::setParentRelationsH(
    const std::shared_ptr<ProcedureNode> node,
    const std::vector<ParentLine> parent_lines) {
  setParentRelationsIterator(node->StmtList, parent_lines);
}

void PKBPreprocessor::setParentRelationsH(
    const std::shared_ptr<IfNode> node, std::vector<ParentLine> parent_lines) {
  const Line cur_line_number = storage->getLineFromNode(node);
  parent_lines.push_back(cur_line_number);
  setParentRelationsIterator(node->StmtListThen, parent_lines);
  setParentRelationsIterator(node->StmtListElse, parent_lines);
}

void PKBPreprocessor::setParentRelationsH(
    const std::shared_ptr<WhileNode> node,
    std::vector<ParentLine> parent_lines) {
  const Line cur_line_number = storage->getLineFromNode(node);
  parent_lines.push_back(cur_line_number);
  setParentRelationsIterator(node->StmtList, parent_lines);
}

void PKBPreprocessor::setParentRelationsIterator(
    const std::vector<StmtNode> stmt_lst,
    const std::vector<ParentLine> parent_lines) {
  if (parent_lines.size() > 0) {
    const Line direct_parent_line_number = parent_lines.back();
    for (const auto &stmt : stmt_lst) {
      const Line cur_line_number = storage->getLineFromNode(stmt);
      // add direct relationship
      storage->storeParentRelation(direct_parent_line_number, cur_line_number);
      // add indirect relationship
      for (const auto &cur_parent_line_number : parent_lines) {
        storage->storeParentRelationS(cur_parent_line_number, cur_line_number);
      }
    }
  }

  for (const auto &stmt : stmt_lst) {
    std::visit(
        [this, parent_lines](const auto &s) {
          using T = std::decay_t<decltype(s)>;
          if constexpr (std::is_same_v<T, std::shared_ptr<ProcedureNode>> ||
                        std::is_same_v<T, std::shared_ptr<IfNode>> ||
                        std::is_same_v<T, std::shared_ptr<WhileNode>>) {
            setParentRelationsH(s, parent_lines);
          }
        },
        stmt);
  }
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<RootNode> node) {
  // set relations within each procedure
  for (const auto &proc : node->ProcList) {
    setUsesRelations(proc);
  }
  // set indirect relations across procedures
  setUsesIndirectRelations();
}

void PKBPreprocessor::setUsesIndirectRelations() {
  // make a copy of procedure_calls_procedure_set which contains direct
  // relations
  auto indirect_procedure_calls_sets =
      std::unordered_set<std::pair<ProcedureCaller, ProcedureCallee>,
                         pair_hash>(
          storage->procedure_calls_procedure_set_t.begin(),
          storage->procedure_calls_procedure_set_t.end());
  for (const auto &indirect_call : indirect_procedure_calls_sets) {
    const ProcedureCaller proc_caller = indirect_call.first;
    const ProcedureCallee proc_callee = indirect_call.second;
    // if callee uses variables
    if (storage->var_used_by_procedure_map.find(proc_callee) !=
        storage->var_used_by_procedure_map.end()) {
      auto vars_used = storage->var_used_by_procedure_map.at(proc_callee);
      for (const auto &var : vars_used) {
        storage->storeProcedureUsesVarRelation(proc_caller, var);
      }
    }
  }

  // loop through call_set
  // gets the lines where there is a call statement
  auto line_proc_set =
      std::unordered_set<std::pair<Line, Procedure>, pair_hash>(
          storage->line_calls_procedure_set.begin(),
          storage->line_calls_procedure_set.end());
  for (const auto &line_proc : line_proc_set) {
    const Line line = line_proc.first;
    const Procedure proc_called = line_proc.second;
    // find out what vars the proc_called uses
    if (storage->var_used_by_procedure_map.find(proc_called) !=
        storage->var_used_by_procedure_map.end()) {
      auto vars_used = storage->var_used_by_procedure_map.at(proc_called);
      for (const auto &var : vars_used) {
        storage->storeLineUsesVarRelation(line, var);
      }

      if (storage->child_line_parent_line_map.find(line) !=
          storage->child_line_parent_line_map.end()) {
        auto parents = storage->child_line_parent_line_map_t.at(line);
        for (const auto &parent : parents) {
          for (auto &var : vars_used) {
            storage->storeLineUsesVarRelation(parent, var);
          }
        }
      }
    }
  }
}

void PKBPreprocessor::setUsesRelations(
    const std::shared_ptr<ProcedureNode> node) {
  setUsesRelationsIterator(node->StmtList);
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<IfNode> node) {
  setUsesRelationsH(node->CondExpr, node);
  setUsesRelationsIterator(node->StmtListThen);
  setUsesRelationsIterator(node->StmtListElse);
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<WhileNode> node) {
  setUsesRelationsH(node->CondExpr, node);
  setUsesRelationsIterator(node->StmtList);
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<PrintNode> node) {
  setUsesRelationsH(node->Var, node);
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<AssignNode> node) {
  setUsesRelationsH(node->Exp, node);
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<CallNode>) {}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<ReadNode>) {}

void PKBPreprocessor::setUsesRelationsH(const Expr node,
                                        const StmtNode parent_node) {
  std::visit(
      [this, parent_node](const auto &n) { setUsesRelationsH(n, parent_node); },
      node);
}

void PKBPreprocessor::setUsesRelationsH(const std::shared_ptr<BinOpNode> node,
                                        const StmtNode parent_node) {
  setUsesRelationsH(node->Left, parent_node);
  setUsesRelationsH(node->Right, parent_node);
}

void PKBPreprocessor::setUsesRelationsH(
    const std::shared_ptr<CondExprNode> node, const StmtNode parent_node) {
  if (node == nullptr) {
    return;
  }
  setUsesRelationsH(node->RelExpr, parent_node);
  setUsesRelationsH(node->CondLHS, parent_node);
  setUsesRelationsH(node->CondRHS, parent_node);
}

void PKBPreprocessor::setUsesRelationsH(const std::shared_ptr<RelExprNode> node,
                                        const StmtNode parent_node) {
  if (node == nullptr) {
    return;
  }
  setUsesRelationsH(node->LHS, parent_node);
  setUsesRelationsH(node->RHS, parent_node);
}

void PKBPreprocessor::setUsesRelationsH(const std::shared_ptr<NumberNode>,
                                        const StmtNode){};

void PKBPreprocessor::setUsesRelationsH(
    const std::shared_ptr<VariableNode> node, const StmtNode parent_node) {
  const Line cur_line_number = storage->getLineFromNode(parent_node);
  const Procedure proc = storage->getProcedureFromLine(cur_line_number);
  storage->storeProcedureUsesVarRelation(proc, node->Name);
  storage->storeLineUsesVarRelation(cur_line_number, node->Name);

  if (storage->child_line_parent_line_map_t.find(cur_line_number) !=
      storage->child_line_parent_line_map_t.end()) {
    auto parents = storage->child_line_parent_line_map_t.at(cur_line_number);
    for (const auto &elem : parents) {
      storage->storeLineUsesVarRelation(elem, node->Name);
    }
  }
}

void PKBPreprocessor::setUsesRelationsIterator(
    const std::vector<StmtNode> stmt_lst) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setUsesRelations(s); }, stmt);
  }
}

void PKBPreprocessor::setModifiesRelations(
    const std::shared_ptr<RootNode> node) {
  for (const auto &proc : node->ProcList) {
    setModifiesRelations(proc);
  }
  setModifiesIndirectRelations();
}

void PKBPreprocessor::setModifiesIndirectRelations() {
  // make a copy of procedure_calls_procedure_set which contains direct
  // relations
  auto indirect_procedure_calls_sets =
      std::unordered_set<std::pair<ProcedureCaller, ProcedureCallee>,
                         pair_hash>(
          storage->procedure_calls_procedure_set_t.begin(),
          storage->procedure_calls_procedure_set_t.end());
  for (const auto &indirect_call : indirect_procedure_calls_sets) {
    const ProcedureCaller proc_caller = indirect_call.first;
    const ProcedureCallee proc_callee = indirect_call.second;
    // if callee modifies variables
    if (storage->var_modified_by_procedure_map.find(proc_callee) !=
        storage->var_modified_by_procedure_map.end()) {
      auto vars_modified =
          storage->var_modified_by_procedure_map.at(proc_callee);
      for (const auto &var : vars_modified) {
        storage->storeProcedureModifiesVarRelation(proc_caller, var);
      }
    }
  }

  // loop through call_set
  // gets the lines where there is a call statement
  auto line_proc_set =
      std::unordered_set<std::pair<Line, Procedure>, pair_hash>(
          storage->line_calls_procedure_set.begin(),
          storage->line_calls_procedure_set.end());
  for (const auto &line_proc : line_proc_set) {
    const Line line = line_proc.first;
    const Procedure proc_called = line_proc.second;
    // find out what vars the proc_called modifies
    if (storage->var_modified_by_procedure_map.find(proc_called) !=
        storage->var_modified_by_procedure_map.end()) {
      auto vars_modified =
          storage->var_modified_by_procedure_map.at(proc_called);

      for (const auto &var : vars_modified) {
        storage->storeLineModifiesVarRelation(line, var);
      }

      if (storage->child_line_parent_line_map.find(line) !=
          storage->child_line_parent_line_map.end()) {
        auto parents = storage->child_line_parent_line_map_t.at(line);
        for (const auto &parent : parents) {
          for (const auto &var : vars_modified) {
            storage->storeLineModifiesVarRelation(parent, var);
          }
        }
      }
    }
  }
}

void PKBPreprocessor::setModifiesRelations(
    const std::shared_ptr<ProcedureNode> node) {
  setModifiesRelationsIterator(node->StmtList);
}

void PKBPreprocessor::setModifiesRelations(const std::shared_ptr<PrintNode>) {}

void PKBPreprocessor::setModifiesRelations(const std::shared_ptr<IfNode> node) {
  setModifiesRelationsIterator(node->StmtListThen);
  setModifiesRelationsIterator(node->StmtListElse);
}

void PKBPreprocessor::setModifiesRelations(
    const std::shared_ptr<WhileNode> node) {
  setModifiesRelationsIterator(node->StmtList);
}

void PKBPreprocessor::setModifiesRelations(
    const std::shared_ptr<ReadNode> node) {
  setModifiesRelationsH(node->Var, node);
}

void PKBPreprocessor::setModifiesRelations(const std::shared_ptr<CallNode>) {}

void PKBPreprocessor::setModifiesRelations(
    const std::shared_ptr<AssignNode> node) {
  setModifiesRelationsH(node->Var, node);
}

void PKBPreprocessor::setModifiesRelationsH(
    const std::shared_ptr<VariableNode> node, const StmtNode parent_node) {
  const Line cur_line_number = storage->getLineFromNode(parent_node);
  Procedure proc = storage->getProcedureFromLine(cur_line_number);
  storage->storeProcedureModifiesVarRelation(proc, node->Name);
  storage->storeLineModifiesVarRelation(cur_line_number, node->Name);

  // TODO this bit of code is repeated in pkb_manager
  // perhaps shift it under storage helper function
  if (storage->child_line_parent_line_map_t.find(cur_line_number) !=
      storage->child_line_parent_line_map_t.end()) {
    auto parents = storage->child_line_parent_line_map_t.at(cur_line_number);
    for (const auto &elem : parents) {
      storage->storeLineModifiesVarRelation(elem, node->Name);
    }
  }
}

void PKBPreprocessor::setModifiesRelationsIterator(
    const std::vector<StmtNode> stmt_lst) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setModifiesRelations(s); }, stmt);
  }
}

void PKBPreprocessor::setPattern(const std::shared_ptr<RootNode> node) {
  for (const auto &proc : node->ProcList) {
    setPattern(proc);
  }
}

void PKBPreprocessor::setPattern(const std::shared_ptr<ProcedureNode> node) {
  setPatternIterator(node->StmtList);
}

void PKBPreprocessor::setPattern(const std::shared_ptr<AssignNode> node) {
  // get var name
  const Variable var_name = node->Var->Name;
  const Line line_num = storage->getLineFromNode(node);
  const ExprStr expr_str =
      std::visit([](const auto &s) { return s->to_str(); }, node->Exp);
  storage->storeAssignPattern(var_name, expr_str, line_num);
}

void PKBPreprocessor::setPattern(const std::shared_ptr<IfNode> node) {
  setPatternH(node->CondExpr, node);
  setPatternIterator(node->StmtListThen);
  setPatternIterator(node->StmtListElse);
}

void PKBPreprocessor::setPattern(const std::shared_ptr<WhileNode> node) {
  setPatternH(node->CondExpr, node);
  setPatternIterator(node->StmtList);
}

void PKBPreprocessor::setPattern(const std::shared_ptr<ReadNode>) {}

void PKBPreprocessor::setPattern(const std::shared_ptr<CallNode>) {}

void PKBPreprocessor::setPattern(const std::shared_ptr<PrintNode>) {}

void PKBPreprocessor::setPatternH(const Expr node, const StmtNode parent_node) {
  std::visit(
      [this, parent_node](const auto &n) { setPatternH(n, parent_node); },
      node);
}

void PKBPreprocessor::setPatternH(const std::shared_ptr<BinOpNode> node,
                                  const StmtNode parent_node) {
  setPatternH(node->Left, parent_node);
  setPatternH(node->Right, parent_node);
}

void PKBPreprocessor::setPatternH(const std::shared_ptr<CondExprNode> node,
                                  const StmtNode parent_node) {
  if (node == nullptr) {
    return;
  }
  setPatternH(node->RelExpr, parent_node);
  setPatternH(node->CondLHS, parent_node);
  setPatternH(node->CondRHS, parent_node);
}

void PKBPreprocessor::setPatternH(const std::shared_ptr<RelExprNode> node,
                                  const StmtNode parent_node) {
  if (node == nullptr) {
    return;
  }
  setPatternH(node->LHS, parent_node);
  setPatternH(node->RHS, parent_node);
}

void PKBPreprocessor::setPatternH(const std::shared_ptr<NumberNode>,
                                  const StmtNode) {}

void PKBPreprocessor::setPatternH(const std::shared_ptr<VariableNode> node,
                                  const StmtNode parent_node) {
  const Line line = storage->getLineFromNode(parent_node);
  const Variable var = node->Name;

  std::visit(
      [this, line, var](const auto &s) mutable {
        using T = std::decay_t<decltype(s)>;
        if constexpr (std::is_same_v<T, std::shared_ptr<IfNode>>) {
          storage->storeIfPattern(var, line);
        } else if constexpr (std::is_same_v<T, std::shared_ptr<WhileNode>>) {
          storage->storeWhilePattern(var, line);
        } else {
          assert(false);
        }
      },
      parent_node);
}

void PKBPreprocessor::setPatternIterator(const std::vector<StmtNode> stmt_lst) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setPattern(s); }, stmt);
  }
}

void PKBPreprocessor::setAssign(const std::shared_ptr<RootNode> node) {
  for (const auto &proc : node->ProcList) {
    setAssign(proc);
  }
}

void PKBPreprocessor::setAssign(const std::shared_ptr<ProcedureNode> node) {
  setAssignIterator(node->StmtList);
}

void PKBPreprocessor::setAssign(const std::shared_ptr<IfNode> node) {
  setAssignIterator(node->StmtListThen);
  setAssignIterator(node->StmtListElse);
}

void PKBPreprocessor::setAssign(const std::shared_ptr<WhileNode> node) {
  setAssignIterator(node->StmtList);
}

void PKBPreprocessor::setAssign(const std::shared_ptr<ReadNode>) {}

void PKBPreprocessor::setAssign(const std::shared_ptr<PrintNode>) {}

void PKBPreprocessor::setAssign(const std::shared_ptr<AssignNode> node) {
  const Line cur_line = storage->getLineFromNode(node);
  storage->storeAssignLineModifiesVariable(cur_line, node->Var->Name);
  setAssign(node->Exp, cur_line);
}

void PKBPreprocessor::setAssign(const std::shared_ptr<CallNode>) {}

void PKBPreprocessor::setAssign(const Expr node, const Line line_number) {
  std::visit([this, line_number](const auto &n) { setAssign(n, line_number); },
             node);
}

void PKBPreprocessor::setAssign(const std::shared_ptr<BinOpNode> node,
                                const Line line_number) {
  setAssign(node->Left, line_number);
  setAssign(node->Right, line_number);
}

void PKBPreprocessor::setAssign(const std::shared_ptr<CondExprNode> node,
                                const Line line_number) {
  if (node == nullptr) {
    return;
  }
  setAssign(node->RelExpr, line_number);
  setAssign(node->CondLHS, line_number);
  setAssign(node->CondRHS, line_number);
}

void PKBPreprocessor::setAssign(const std::shared_ptr<RelExprNode> node,
                                const Line line_number) {
  if (node == nullptr) {
    return;
  }
  setAssign(node->LHS, line_number);
  setAssign(node->RHS, line_number);
}

void PKBPreprocessor::setAssign(const std::shared_ptr<NumberNode>, const Line) {
}

void PKBPreprocessor::setAssign(const std::shared_ptr<VariableNode> node,
                                const Line line_number) {
  storage->storeAssignLineUsesVariable(line_number, node->Name);
}

void PKBPreprocessor::setAssignIterator(const std::vector<StmtNode> stmt_lst) {
  // iterate through AST via DFS
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setAssign(s); }, stmt);
  }
}

void PKBPreprocessor::setProcedureStartAndEnd(
    const std::shared_ptr<RootNode> node) {
  for (const auto &proc : node->ProcList) {
    // set procedure start
    const Procedure proc_name = proc->Name;
    const Line first_proc_line = storage->getLineFromNode(proc->StmtList[0]);
    storage->storeProcFirstLine(proc_name, first_proc_line);

    // set procedure end
    std::shared_ptr<std::unordered_set<Line>> visited =
        std::make_shared<std::unordered_set<Line>>();
    setProcedureEnd(first_proc_line, visited);
    setProcedureEndHIterator(proc->StmtList);
  }
}

void PKBPreprocessor::setProcedureEnd(
    const Line cur_line, std::shared_ptr<std::unordered_set<Line>> visited) {
  if (visited->find(cur_line) != visited->end()) {
    return;
  }
  visited->insert(cur_line);
  if (storage->line_previous_line_next_map.find(cur_line) !=
      storage->line_previous_line_next_map.end()) {
    std::unordered_set<NextLine> next_lines =
        storage->line_previous_line_next_map.at(cur_line);
    for (const auto &line : next_lines) {
      setProcedureEnd(line, visited);
    }
  } else {
    // reached the end
    const Procedure proc = storage->getProcedureFromLine(cur_line);
    storage->storeProcLastLine(proc, cur_line);
  }
}

// to catch edge case where program ends with while
void PKBPreprocessor::setProcedureEndHIterator(
    const std::vector<StmtNode> stmt_lst) {
  std::visit([this](const auto &s) { setProcedureEndH(s); }, stmt_lst.back());
}

void PKBPreprocessor::setProcedureEndH(const std::shared_ptr<IfNode> node) {
  setProcedureEndHIterator(node->StmtListThen);
  setProcedureEndHIterator(node->StmtListElse);
}

void PKBPreprocessor::setProcedureEndH(const std::shared_ptr<WhileNode> node) {
  const Line line = storage->getLineFromNode(node);
  const Procedure proc = storage->getProcedureFromLine(line);
  storage->storeProcLastLine(proc, line);
}

void PKBPreprocessor::setProcedureEndH(const std::shared_ptr<ReadNode>) {}

void PKBPreprocessor::setProcedureEndH(const std::shared_ptr<PrintNode>) {}

void PKBPreprocessor::setProcedureEndH(const std::shared_ptr<AssignNode>) {}

void PKBPreprocessor::setProcedureEndH(const std::shared_ptr<CallNode>) {}

void PKBPreprocessor::setProcedureEndH(const StmtNode node) {
  std::visit([this](const auto &n) { setProcedureEndH(n); }, node);
}

void PKBPreprocessor::setCFGBip() {
  // loop through all the statement lines
  for (const auto &line : storage->statement_set) {
    setCFGBipH(line);
  }
}

void PKBPreprocessor::setCFGBipH(const PreviousLine previous_line) {
  // if current line is a call statement
  if (storage->call_set.find(previous_line) != storage->call_set.end()) {
    // get the procedure that is being called
    const ProcedureCallee procedure_callee =
        storage->line_calls_procedure_map.at(previous_line);
    // get first line to that procedure
    const Line first_line = storage->proc_first_line_map.at(procedure_callee);
    storage->storeCFGBipEdge(previous_line, first_line);
  } else {
    const Procedure cur_proc = storage->getProcedureFromLine(previous_line);
    if (storage->proc_last_line_map.at(cur_proc).find(previous_line) !=
        storage->proc_last_line_map.at(cur_proc).end()) {
      // reached the end of procedure
      // check what procedure's lines were calling it
      if (storage->procedure_line_calls_map.find(cur_proc) !=
          storage->procedure_line_calls_map.end()) {
        std::unordered_set<Line> call_lines =
            storage->procedure_line_calls_map.at(cur_proc);
        for (const auto &line : call_lines) {
          // for each line that was calling it
          // get their next line as per normal
          if (storage->line_previous_line_next_map.find(line) !=
              storage->line_previous_line_next_map.end()) {
            std::unordered_set<Line> next_lines =
                storage->line_previous_line_next_map.at(line);
            for (const auto &next_line : next_lines) {
              storage->storeCFGBipEdge(previous_line, next_line);
            }
          }
        }
      }
      // else no procedures were calling it
    } else {
      // get next line as per normal
      std::unordered_set<Line> next_lines =
          storage->line_previous_line_next_map.at(previous_line);
      for (const auto &next_line : next_lines) {
        storage->storeCFGBipEdge(previous_line, next_line);
      }
    }
  }
}

}  // namespace PKB
