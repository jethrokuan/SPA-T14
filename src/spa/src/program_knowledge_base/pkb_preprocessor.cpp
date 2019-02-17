#include "program_knowledge_base/pkb_preprocessor.h"
#include <iostream>
#include "structs/node.h"

namespace PKB {

PKBPreprocessor::PKBPreprocessor(const std::shared_ptr<ProcedureNode> ast,
                                 std::shared_ptr<PKBStorage> pkb_storage) {
  storage = pkb_storage;
  setLineNumbers(ast);
  setDesignEntities(ast);
  setFollowsRelations(ast);
  setParentRelations(ast);
  setUsesRelations(ast);
  setModifiesRelations(ast);
}

PKBPreprocessor::~PKBPreprocessor() {}

void PKBPreprocessor::setLineNumbers(
    const std::shared_ptr<ProcedureNode> node) {
  setLineNumbersIterator(node->StmtList->StmtList, node->Name);
}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<IfNode> node,
                                     Procedure proc) {
  auto line_number = storage->storeLine(node);
  storage->storeLineProcedureRelation(line_number, proc);
  setLineNumbersIterator(node->StmtListThen->StmtList, proc);
  setLineNumbersIterator(node->StmtListElse->StmtList, proc);
}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<WhileNode> node,
                                     Procedure proc) {
  auto line_number = storage->storeLine(node);
  storage->storeLineProcedureRelation(line_number, proc);
  setLineNumbersIterator(node->StmtList->StmtList, proc);
}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<ReadNode> node,
                                     Procedure proc) {
  auto line_number = storage->storeLine(node);
  storage->storeLineProcedureRelation(line_number, proc);
}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<PrintNode> node,
                                     Procedure proc) {
  auto line_number = storage->storeLine(node);
  storage->storeLineProcedureRelation(line_number, proc);
}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<AssignNode> node,
                                     Procedure proc) {
  auto line_number = storage->storeLine(node);
  storage->storeLineProcedureRelation(line_number, proc);
}

void PKBPreprocessor::setLineNumbersIterator(
    const std::vector<StmtNode> stmt_lst, Procedure proc) {
  // iterate through AST via DFS
  for (const auto &stmt : stmt_lst) {
    std::visit([this, proc](const auto &s) { setLineNumbers(s, proc); }, stmt);
  }
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<ProcedureNode> node) {
  storage->storeProcedure(node->Name);
  setDesignEntitiesIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<IfNode> node) {
  storage->storeStatement(storage->getLineFromNode(node));
  storage->storeIf(storage->getLineFromNode(node));
  setDesignEntities(node->CondExpr);
  setDesignEntitiesIterator(node->StmtListThen->StmtList);
  setDesignEntitiesIterator(node->StmtListElse->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<WhileNode> node) {
  storage->storeStatement(storage->getLineFromNode(node));
  storage->storeWhile(storage->getLineFromNode(node));
  setDesignEntities(node->CondExpr);
  setDesignEntitiesIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<ReadNode> node) {
  storage->storeStatement(storage->getLineFromNode(node));
  storage->storeRead(storage->getLineFromNode(node));
  setDesignEntities(node->Var);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<PrintNode> node) {
  storage->storeStatement(storage->getLineFromNode(node));
  storage->storePrint(storage->getLineFromNode(node));
  setDesignEntities(node->Var);
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<AssignNode> node) {
  storage->storeStatement(storage->getLineFromNode(node));
  storage->storeAssign(storage->getLineFromNode(node));
  setDesignEntities(node->Exp);
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
  if (node == nullptr) {
    return;
  }
  storage->storeConstant(node->Val);
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<VariableNode> node) {
  if (node == nullptr) {
    return;
  }
  storage->storeVariable(node->Name);
}

void PKBPreprocessor::setDesignEntitiesIterator(
    const std::vector<StmtNode> stmt_lst) {
  // iterate through AST via DFS
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setDesignEntities(s); }, stmt);
  }
}

void PKBPreprocessor::setFollowsRelations(
    const std::shared_ptr<ProcedureNode> node) {
  setFollowsRelationsIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setFollowsRelations(const std::shared_ptr<IfNode> node) {
  setFollowsRelationsIterator(node->StmtListThen->StmtList);
  setFollowsRelationsIterator(node->StmtListElse->StmtList);
}

void PKBPreprocessor::setFollowsRelations(
    const std::shared_ptr<WhileNode> node) {
  setFollowsRelationsIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setFollowsRelationsIterator(
    const std::vector<StmtNode> stmt_lst) {
  // add direct relations
  for (std::size_t i = 0; i < stmt_lst.size() - 1; i++) {
    Line cur_line_number = std::visit(
        [this](const auto &s) { return storage->getLineFromNode(s); },
        stmt_lst[i]);

    Line next_line_number = std::visit(
        [this](const auto &s) { return storage->getLineFromNode(s); },
        stmt_lst[i + 1]);
    storage->storeFollowsRelation(cur_line_number, next_line_number);
  }
  // add indirect relations
  for (std::size_t i = 0; i < stmt_lst.size() - 1; i++) {
    for (std::size_t j = i + 1; j < stmt_lst.size(); j++) {
      Line cur_line_number = std::visit(
          [this](const auto &s) { return storage->getLineFromNode(s); },
          stmt_lst[i]);

      Line next_line_number = std::visit(
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

void PKBPreprocessor::setParentRelations(
    const std::shared_ptr<ProcedureNode> node) {
  std::vector<ParentLine> parent_lines;
  setParentRelationsH(node, parent_lines);
}

void PKBPreprocessor::setParentRelationsH(
    const std::shared_ptr<ProcedureNode> node,
    std::vector<ParentLine> parent_lines) {
  Line cur_line_number = storage->getLineFromNode(node);
  parent_lines.push_back(cur_line_number);
  setParentRelationsIterator(node->StmtList->StmtList, parent_lines);
}

void PKBPreprocessor::setParentRelationsH(
    const std::shared_ptr<IfNode> node, std::vector<ParentLine> parent_lines) {
  Line cur_line_number = storage->getLineFromNode(node);
  parent_lines.push_back(cur_line_number);
  setParentRelationsIterator(node->StmtListThen->StmtList, parent_lines);
  setParentRelationsIterator(node->StmtListElse->StmtList, parent_lines);
}

void PKBPreprocessor::setParentRelationsH(
    const std::shared_ptr<WhileNode> node,
    std::vector<ParentLine> parent_lines) {
  Line cur_line_number = storage->getLineFromNode(node);
  parent_lines.push_back(cur_line_number);
  setParentRelationsIterator(node->StmtList->StmtList, parent_lines);
}

void PKBPreprocessor::setParentRelationsIterator(
    const std::vector<StmtNode> stmt_lst,
    const std::vector<ParentLine> parent_lines) {
  Line direct_parent_line_number = parent_lines.back();
  for (const auto &stmt : stmt_lst) {
    Line cur_line_number = std::visit(
        [this](const auto &s) { return storage->getLineFromNode(s); }, stmt);
    // add direct relationship
    storage->storeParentRelation(direct_parent_line_number, cur_line_number);
    // add indirect relationship
    for (const auto &cur_parent_line_number : parent_lines) {
      storage->storeParentRelationS(cur_parent_line_number, cur_line_number);
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

void PKBPreprocessor::setUsesRelations(
    const std::shared_ptr<ProcedureNode> node) {
  setUsesRelationsIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<IfNode> node) {
  setUsesRelationsH(node->CondExpr, node);
  setUsesRelationsIterator(node->StmtListThen->StmtList);
  setUsesRelationsIterator(node->StmtListElse->StmtList);
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<WhileNode> node) {
  setUsesRelationsH(node->CondExpr, node);
  setUsesRelationsIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<PrintNode> node) {
  setUsesRelationsH(node->Var, node);
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<AssignNode> node) {
  setUsesRelationsH(node->Exp, node);
}

void PKBPreprocessor::setUsesRelations(const std::shared_ptr<ReadNode>) {}

//
void PKBPreprocessor::setUsesRelationsH(const Expr node,
                                        std::shared_ptr<Node> parent_node) {
  std::visit(
      [this, parent_node](const auto &n) { setUsesRelationsH(n, parent_node); },
      node);
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
  if (node == nullptr) {
    return;
  }
  Line cur_line_number = storage->getLineFromNode(parent_node);
  Procedure proc = storage->getProcedureFromLine(cur_line_number);
  storage->storeProcedureUsesVarRelation(proc, node->Name);
  storage->storeLineUsesVarRelation(cur_line_number, node->Name);
}

void PKBPreprocessor::setUsesRelationsIterator(
    const std::vector<StmtNode> stmt_lst) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setUsesRelations(s); }, stmt);
  }
}

void PKBPreprocessor::setModifiesRelations(
    const std::shared_ptr<ProcedureNode> node) {
  setModifiesRelationsIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setModifiesRelations(const std::shared_ptr<PrintNode>) {}

void PKBPreprocessor::setModifiesRelations(const std::shared_ptr<IfNode> node) {
  setModifiesRelationsIterator(node->StmtListThen->StmtList);
  setModifiesRelationsIterator(node->StmtListElse->StmtList);
}

void PKBPreprocessor::setModifiesRelations(
    const std::shared_ptr<WhileNode> node) {
  setModifiesRelationsIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setModifiesRelations(
    const std::shared_ptr<ReadNode> node) {
  setModifiesRelationsH(node->Var, node);
}

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
}

void PKBPreprocessor::setModifiesRelationsIterator(
    const std::vector<StmtNode> stmt_lst) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setModifiesRelations(s); }, stmt);
  }
}

}  // namespace PKB
