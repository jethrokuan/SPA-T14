#include "program_knowledge_base/pkb_preprocessor.h"
#include <iostream>
#include "structs/node.h"

namespace PKB {

PKBPreprocessor::PKBPreprocessor(const std::shared_ptr<ProcedureNode> ast,
                                 std::shared_ptr<PKBStorage> pkb_storage) {
  storage = pkb_storage;
  setLineNumbers(ast);
  setDesignEntities(ast);
  // setFollowsRelations(ast);
  // setParentRelations(ast);
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
  setDesignEntitiesIterator(node->StmtListThen->StmtList);
  setDesignEntitiesIterator(node->StmtListElse->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<WhileNode> node) {
  storage->storeWhile(storage->getLineFromNode(node));
  setDesignEntitiesIterator(node->StmtList->StmtList);
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<ReadNode> node) {
  storage->storeRead(storage->getLineFromNode(node));
}

void PKBPreprocessor::setDesignEntities(const std::shared_ptr<PrintNode> node) {
  storage->storePrint(storage->getLineFromNode(node));
}

void PKBPreprocessor::setDesignEntities(
    const std::shared_ptr<AssignNode> node) {
  storage->storeAssign(storage->getLineFromNode(node));
}

// void PKBPreprocessor::setDesignEntities(
//     const Expr node) {
//   std::visit(
//       [this](const auto &n) { setDesignEntities(n); },
//       node);
// }

// void PKBPreprocessor::setDesignEntities(
//     const std::shared_ptr<CondExprNode> node) {
//   if (node == nullptr) {
//     return;
//   }
//   setDesignEntities(node->RelExpr);
//   setDesignEntities(node->CondLHS);
//   setDesignEntities(node->CondRHS);
// }

// void PKBPreprocessor::setDesignEntities(
//     const std::shared_ptr<RelExprNode> node) {
//   if (node == nullptr) {
//     return;
//   }
//   setDesignEntities(node->LHS);
//   setDesignEntities(node->RHS);
// }

// void PKBPreprocessor::setDesignEntities(
//     const std::shared_ptr<NumberNode> node) {
//   if (node == nullptr) {
//     return;
//   }
//   storage->storeConstant(node->Val);
// }

// void PKBPreprocessor::setDesignEntities(
//     const std::shared_ptr<VariableNode> node) {
//   if (node == nullptr) {
//     return;
//   }
//   storage->storeVariable(storage->getLineFromNode(node));
// }

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

}  // namespace PKB
