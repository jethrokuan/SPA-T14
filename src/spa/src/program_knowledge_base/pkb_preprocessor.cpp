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
  // setUsesRelations(ast);
  // setModifiesRelations(ast);
}

PKBPreprocessor::~PKBPreprocessor() {}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<ProcedureNode> node) {
  storage->storeProcedure(node->Name);
  setDesignEntitiesIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<IfNode> node) {
  storage->storeIf(storage->getLineFromNode(node));
  setDesignEntities(node->CondExpr);
  setDesignEntitiesIterator(node->StmtListThen->StmtList);
  setDesignEntitiesIterator(node->StmtListElse->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<WhileNode> node) {
  storage->storeWhile(storage->getLineFromNode(node));
  setDesignEntities(node->CondExpr);
  setDesignEntitiesIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<ReadNode> node) {
  storage->storeRead(storage->getLineFromNode(node));
  setDesignEntities(node->Var);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<PrintNode> node) {
  storage->storePrint(storage->getLineFromNode(node));
  setDesignEntities(node->Var);
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<AssignNode> node) {
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

void PKBPreprocessor::setLineNumbers(
    const std::shared_ptr<ProcedureNode> node) {
  setLineNumbersIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<IfNode> node) {
  storage->storeLine(node);
  setLineNumbersIterator(node->StmtListThen->StmtList);
  setLineNumbersIterator(node->StmtListElse->StmtList);
}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<WhileNode> node) {
  storage->storeLine(node);
  setLineNumbersIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<ReadNode> node) {
  storage->storeLine(node);
}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<PrintNode> node) {
  storage->storeLine(node);
}

void PKBPreprocessor::setLineNumbers(const std::shared_ptr<AssignNode> node) {
  storage->storeLine(node);
}

void PKBPreprocessor::setLineNumbersIterator(
    const std::vector<StmtNode> stmt_lst) {
  // iterate through AST via DFS
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setLineNumbers(s); }, stmt);
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
  // add relations
  for (std::size_t i = 0; i < stmt_lst.size() - 1; i++) {
    auto cur_line_number = std::visit(
        [this](const auto &s) { return storage->getLineFromNode(s); },
        stmt_lst[i]);

    auto next_line_number = std::visit(
        [this](const auto &s) { return storage->getLineFromNode(s); },
        stmt_lst[i + 1]);
    storage->storeFollowsRelation(cur_line_number, next_line_number);
    // DEBUG
    // std::cout << cur_line_number;
    // std::cout << " is followed by ";
    // std::cout << next_line_number << std::endl;
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
  setParentRelationsIterator(node->StmtList->StmtList, node);
}

void PKBPreprocessor::setParentRelations(const std::shared_ptr<IfNode> node) {
  setParentRelationsIterator(node->StmtListThen->StmtList, node);
  setParentRelationsIterator(node->StmtListElse->StmtList, node);
}

void PKBPreprocessor::setParentRelations(
    const std::shared_ptr<WhileNode> node) {
  setParentRelationsIterator(node->StmtList->StmtList, node);
}

void PKBPreprocessor::setParentRelationsIterator(
    const std::vector<StmtNode> stmt_lst,
    const std::shared_ptr<Node> parent_node) {
  for (const auto &stmt : stmt_lst) {
    std::visit(
        [this, parent_node](const auto &s) {
          auto current_line = storage->getLineFromNode(s);
          storage->storeParentRelation(storage->getLineFromNode(parent_node),
                                       current_line);
          // DEBUG
          // std::cout << storage->getLineFromNode(parent_node);
          // std::cout << " is parent of ";
          // std::cout << current_line << std::endl;
        },
        stmt);
  }

  for (const auto &stmt : stmt_lst) {
    std::visit(
        [this](const auto &s) {
          using T = std::decay_t<decltype(s)>;
          if constexpr (std::is_same_v<T, std::shared_ptr<ProcedureNode>> ||
                        std::is_same_v<T, std::shared_ptr<IfNode>> ||
                        std::is_same_v<T, std::shared_ptr<WhileNode>>) {
            setParentRelations(s);
          }
        },
        stmt);
  }
}

}  // namespace PKB
