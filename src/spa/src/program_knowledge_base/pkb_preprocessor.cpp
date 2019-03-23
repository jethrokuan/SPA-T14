#include "program_knowledge_base/pkb_preprocessor.h"
#include <iostream>
#include "structs/node.h"

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
  std::visit(
      [this, proc](const auto &s) {
        const Line line_number = storage->storeLine(s);
        storage->storeLineProcedureRelation(line_number, proc);
      },
      node);
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
    std::visit([this, proc](const auto &s) { setLineNumbers(s, proc); }, stmt);
  }
}

void PKBPreprocessor::setCFG(const std::shared_ptr<RootNode> node) {
  // std::cout << "RootNode" << std::endl;
  for (const auto &proc : node->ProcList) {
    setCFG(proc);
  }
}

void PKBPreprocessor::setCFG(const std::shared_ptr<ProcedureNode> node) {
  // std::cout << "ProcedureNode" << std::endl;
  std::shared_ptr<std::vector<Line>> terminating_lines =
      std::make_shared<std::vector<Line>>();
  setCFGIterator(node->StmtList, terminating_lines);
}

void PKBPreprocessor::setCFG(
    const std::shared_ptr<IfNode> node,
    std::shared_ptr<std::vector<Line>> terminating_lines) {
  // std::cout << "IfNode" << std::endl;
  const ParentLine parent_line = storage->getLineFromNode(node);
  ChildLine then_child_line =
      std::visit([this](const auto &s) { return storage->getLineFromNode(s); },
                 node->StmtListThen.front());
  ChildLine else_child_line =
      std::visit([this](const auto &s) { return storage->getLineFromNode(s); },
                 node->StmtListElse.front());
  storage->storeCFGEdge(parent_line, then_child_line);
  storage->storeCFGEdge(parent_line, else_child_line);

  setCFGIterator(node->StmtListThen, terminating_lines);
  setCFGIterator(node->StmtListElse, terminating_lines);
}

void PKBPreprocessor::setCFG(
    const std::shared_ptr<WhileNode> node,
    std::shared_ptr<std::vector<Line>> terminating_lines) {
  // std::cout << "WhileNode" << std::endl;
  const ParentLine parent_line = storage->getLineFromNode(node);
  ChildLine child_line =
      std::visit([this](const auto &s) { return storage->getLineFromNode(s); },
                 node->StmtList.front());
  storage->storeCFGEdge(parent_line, child_line);
  setCFGIterator(node->StmtList, terminating_lines);
}

void PKBPreprocessor::setCFG(const std::shared_ptr<ReadNode>,
                             std::shared_ptr<std::vector<Line>>) {
  // std::cout << "ReadNode" << std::endl;
}

void PKBPreprocessor::setCFG(const std::shared_ptr<PrintNode>,
                             std::shared_ptr<std::vector<Line>>) {
  // std::cout << "PrintNode" << std::endl;
}

void PKBPreprocessor::setCFG(const std::shared_ptr<AssignNode>,
                             std::shared_ptr<std::vector<Line>>) {
  // std::cout << "AssignNode" << std::endl;
}

void PKBPreprocessor::setCFG(const std::shared_ptr<CallNode>,
                             std::shared_ptr<std::vector<Line>>) {
  // std::cout << "CallNode" << std::endl;
}

void PKBPreprocessor::setCFGIterator(
    const std::vector<StmtNode> stmt_lst,
    std::shared_ptr<std::vector<Line>> terminating_lines) {
  // std::cout << "iterator" << std::endl;
  // add edge for consecutive lines
  Line previous_line = INVALID_LINE_NUMBER;  // used to keep reference
  bool previous_node_is_if_node = false;
  for (std::size_t i = 0; i < stmt_lst.size(); i++) {
    const bool is_last_node = (i == (stmt_lst.size() - 1));
    std::visit(
        [this, &previous_line, &previous_node_is_if_node, &terminating_lines,
         is_last_node](const auto &s) mutable {
          const Line cur_line = storage->getLineFromNode(s);
          if (previous_node_is_if_node) {
            // set next relations for everything in the list
            for (const auto &line : *terminating_lines.get()) {
              storage->storeCFGEdge(line, cur_line);
            }
            // delete terminating nodes from the list
            terminating_lines->clear();
            previous_node_is_if_node = false;  // reset flag
          } else if (previous_line != INVALID_LINE_NUMBER) {
            storage->storeCFGEdge(previous_line, cur_line);
          }
          using T = std::decay_t<decltype(s)>;
          if constexpr (std::is_same_v<T, std::shared_ptr<IfNode>>) {
            previous_node_is_if_node = true;  // set flag
            setCFG(s, terminating_lines);
          } else if constexpr (std::is_same_v<T, std::shared_ptr<WhileNode>>) {
            std::shared_ptr<std::vector<Line>> temp_list =
                std::make_shared<std::vector<Line>>();
            setCFG(s, temp_list);
            // set edges in temp_list
            for (const auto &line : *temp_list.get()) {
              storage->storeCFGEdge(line, cur_line);
            }
          } else {
            setCFG(s, terminating_lines);
          }
          // if the last node is not an if node
          if constexpr (std::is_same_v<T, std::shared_ptr<IfNode>>) {
            // ignore if node
          } else if (is_last_node) {
            // std::cout << "storing " + cur_line + " as last node" <<
            // std::endl;
            terminating_lines->push_back(cur_line);
          }
          previous_line = cur_line;
        },
        stmt_lst[i]);
  }
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
  storage->storeStatement(storage->getLineFromNode(node));
  storage->storeIf(storage->getLineFromNode(node));
  setDesignEntities(node->CondExpr);
  setDesignEntitiesIterator(node->StmtListThen);
  setDesignEntitiesIterator(node->StmtListElse);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<WhileNode> node) {
  storage->storeStatement(storage->getLineFromNode(node));
  storage->storeWhile(storage->getLineFromNode(node));
  setDesignEntities(node->CondExpr);
  setDesignEntitiesIterator(node->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<ReadNode> node) {
  storage->storeStatement(storage->getLineFromNode(node));
  storage->storeRead(storage->getLineFromNode(node), node->Var->Name);
  setDesignEntities(node->Var);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<PrintNode> node) {
  storage->storeStatement(storage->getLineFromNode(node));
  storage->storePrint(storage->getLineFromNode(node), node->Var->Name);
  setDesignEntities(node->Var);
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<AssignNode> node) {
  storage->storeStatement(storage->getLineFromNode(node));
  storage->storeAssign(storage->getLineFromNode(node));
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
  // std::cout << "RootNode" << std::endl;
  // set direct relations
  for (const auto &proc : node->ProcList) {
    setCallsRelations(proc);
  }
  // set indirect relations after direct relations have been set
  // std::cout << "SETTING INDIRECT RELATIONS" << std::endl;
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
  // std::cout << "setCallsIndirectRelationsH" << std::endl;
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
  // std::cout << "ProcedureNode" << std::endl;
  setCallsRelationsIterator(node->StmtList);
}
void PKBPreprocessor::setCallsRelations(const std::shared_ptr<IfNode> node) {
  // std::cout << "IfNode" << std::endl;
  setCallsRelationsIterator(node->StmtListThen);
  setCallsRelationsIterator(node->StmtListElse);
}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<WhileNode> node) {
  // std::cout << "WhileNode" << std::endl;
  setCallsRelationsIterator(node->StmtList);
}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<ReadNode>) {
  // std::cout << "ReadNode" << std::endl;
}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<PrintNode>) {
  // std::cout << "PrintNode" << std::endl;
}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<AssignNode>) {
  // std::cout << "AssignNode" << std::endl;
}

void PKBPreprocessor::setCallsRelations(const std::shared_ptr<CallNode> node) {
  // std::cout << "CallNode" << std::endl;
  const Line line_number = storage->getLineFromNode(node);
  const Procedure proc_caller = storage->getProcedureFromLine(line_number);
  storage->storeCallsRelation(proc_caller, node->ProcName);
  storage->storeCallsRelationS(proc_caller, node->ProcName);
}

void PKBPreprocessor::setCallsRelationsIterator(
    const std::vector<StmtNode> stmt_lst) {
  // std::cout << "Iterator" << std::endl;
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
    const Line cur_line_number = std::visit(
        [this](const auto &s) { return storage->getLineFromNode(s); },
        stmt_lst[i]);

    const Line next_line_number = std::visit(
        [this](const auto &s) { return storage->getLineFromNode(s); },
        stmt_lst[i + 1]);
    storage->storeFollowsRelation(cur_line_number, next_line_number);
  }
  // add indirect relations
  for (std::size_t i = 0; i < stmt_lst.size() - 1; i++) {
    for (std::size_t j = i + 1; j < stmt_lst.size(); j++) {
      const Line cur_line_number = std::visit(
          [this](const auto &s) { return storage->getLineFromNode(s); },
          stmt_lst[i]);

      const Line next_line_number = std::visit(
          [this](const auto &s) { return storage->getLineFromNode(s); },
          stmt_lst[j]);
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
      const Line cur_line_number = std::visit(
          [this](const auto &s) { return storage->getLineFromNode(s); }, stmt);
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
                                        std::shared_ptr<Node> parent_node) {
  std::visit(
      [this, parent_node](const auto &n) { setUsesRelationsH(n, parent_node); },
      node);
}

void PKBPreprocessor::setUsesRelationsH(
    const std::shared_ptr<BinOpNode> node,
    const std::shared_ptr<Node> parent_node) {
  setUsesRelationsH(node->Left, parent_node);
  setUsesRelationsH(node->Right, parent_node);
}

void PKBPreprocessor::setUsesRelationsH(
    const std::shared_ptr<CondExprNode> node,
    const std::shared_ptr<Node> parent_node) {
  if (node == nullptr) {
    return;
  }
  setUsesRelationsH(node->RelExpr, parent_node);
  setUsesRelationsH(node->CondLHS, parent_node);
  setUsesRelationsH(node->CondRHS, parent_node);
}

void PKBPreprocessor::setUsesRelationsH(
    const std::shared_ptr<RelExprNode> node,
    const std::shared_ptr<Node> parent_node) {
  if (node == nullptr) {
    return;
  }
  setUsesRelationsH(node->LHS, parent_node);
  setUsesRelationsH(node->RHS, parent_node);
}

void PKBPreprocessor::setUsesRelationsH(std::shared_ptr<NumberNode>,
                                        const std::shared_ptr<Node>){};

void PKBPreprocessor::setUsesRelationsH(
    const std::shared_ptr<VariableNode> node,
    const std::shared_ptr<Node> parent_node) {
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
    const std::shared_ptr<VariableNode> node,
    const std::shared_ptr<Node> parent_node) {
  Line cur_line_number = storage->getLineFromNode(parent_node);
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
  // TODO
  setPatternH(node->CondExpr, node);
  setPatternIterator(node->StmtListThen);
  setPatternIterator(node->StmtListElse);
}

void PKBPreprocessor::setPattern(const std::shared_ptr<WhileNode> node) {
  // TODO
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
  const Line line =
      std::visit([this](const auto &s) { return storage->getLineFromNode(s); },
                 parent_node);
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

}  // namespace PKB
