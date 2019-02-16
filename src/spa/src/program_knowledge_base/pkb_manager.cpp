#include "program_knowledge_base/pkb_manager.h"
#include <iostream>
template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

namespace PKB {
PKBManager::PKBManager(const std::shared_ptr<ProcedureNode> ast) {
  pkb_storage->storeAST(ast);
  // create Preprocessor
  std::unique_ptr<PKBPreprocessor> pkb_preprocessor =
      std::make_unique<PKBPreprocessor>(ast, pkb_storage);
  // TODO indicate finished
}

PKBManager::~PKBManager() {}

void PKBManager::setLineNumbers(const std::shared_ptr<ProcedureNode> node) {
  setLineNumbersIterator(node->StmtList->StmtList);
}

void PKBManager::setLineNumbers(const std::shared_ptr<IfNode> node) {
  lines.push_back(node);
  setLineNumbersIterator(node->StmtListThen->StmtList);
  setLineNumbersIterator(node->StmtListElse->StmtList);
}

void PKBManager::setLineNumbers(const std::shared_ptr<WhileNode> node) {
  lines.push_back(node);
  setLineNumbersIterator(node->StmtList->StmtList);
}

void PKBManager::setLineNumbers(const std::shared_ptr<ReadNode> node) {
  lines.push_back(node);
}

void PKBManager::setLineNumbers(const std::shared_ptr<PrintNode> node) {
  lines.push_back(node);
}

void PKBManager::setLineNumbers(const std::shared_ptr<AssignNode> node) {
  lines.push_back(node);
}

// TODO add return value
void PKBManager::setLineNumbersIterator(const std::vector<StmtNode> stmt_lst) {
  // iterate through AST via DFS

  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setLineNumbers(s); }, stmt);
  }
}

// TODO error handling
// TODO refactor to use hashmap instead
// would need to implement hash function of the node
// as well as a hash table
// return line number
// returns empty string if node does not exist
std::string PKBManager::getLineFromNode(
    const std::vector<std::shared_ptr<Node>> ls,
    const std::shared_ptr<Node> node) {
  // loop through vector of nodes
  for (std::size_t i = 0; i < ls.size(); i++) {
    if (ls[i] == node) {
      return std::to_string(i + 1);  // +1 due to 0 based index
    }
  }
  return "";  // TODO error handling
}

void PKBManager::setParentRelations(const std::shared_ptr<ProcedureNode> node) {
  setParentRelationsIterator(node->StmtList->StmtList, node);
}

void PKBManager::setParentRelations(const std::shared_ptr<IfNode> node) {
  setParentRelationsIterator(node->StmtListThen->StmtList, node);
  setParentRelationsIterator(node->StmtListElse->StmtList, node);
}

void PKBManager::setParentRelations(const std::shared_ptr<WhileNode> node) {
  setParentRelationsIterator(node->StmtList->StmtList, node);
}

void PKBManager::setParentRelationsIterator(
    const std::vector<StmtNode> stmt_lst,
    const std::shared_ptr<Node> parent_node) {
  for (const auto &stmt : stmt_lst) {
    std::visit(
        [this, parent_node](const auto &s) {
          auto current_line = getLineFromNode(lines, s);
          parent_set.insert(std::pair<ParentLine, Line>(
              getNodeValue(parent_node), current_line));
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

void PKBManager::setUsesRelations(const std::shared_ptr<ProcedureNode> node) {
  setUsesRelationsIterator(node->StmtList->StmtList);
}

void PKBManager::setUsesRelations(const std::shared_ptr<IfNode> node) {
  setUsesRelationsH(node->CondExpr, node);
  setUsesRelationsIterator(node->StmtListThen->StmtList);
  setUsesRelationsIterator(node->StmtListElse->StmtList);
}

void PKBManager::setUsesRelations(const std::shared_ptr<WhileNode> node) {
  setUsesRelationsH(node->CondExpr, node);
  setUsesRelationsIterator(node->StmtList->StmtList);
}

void PKBManager::setUsesRelations(const std::shared_ptr<PrintNode> node) {
  setUsesRelationsH(node->Var, node);
}

void PKBManager::setUsesRelations(const std::shared_ptr<AssignNode> node) {
  setUsesRelationsH(node->Exp, node);
}

void PKBManager::setUsesRelations(const std::shared_ptr<ReadNode>) {}

//
void PKBManager::setUsesRelationsH(const Expr node,
                                   std::shared_ptr<Node> parent_node) {
  std::visit(
      [this, parent_node](const auto &n) { setUsesRelationsH(n, parent_node); },
      node);
}

void PKBManager::setUsesRelationsH(const std::shared_ptr<CondExprNode> node,
                                   const std::shared_ptr<Node> parent_node) {
  if (node == nullptr) {
    return;
  }
  setUsesRelationsH(node->RelExpr, parent_node);
  setUsesRelationsH(node->CondLHS, parent_node);
  setUsesRelationsH(node->CondRHS, parent_node);
}

void PKBManager::setUsesRelationsH(const std::shared_ptr<RelExprNode> node,
                                   const std::shared_ptr<Node> parent_node) {
  if (node == nullptr) {
    return;
  }
  setUsesRelationsH(node->LHS, parent_node);
  setUsesRelationsH(node->RHS, parent_node);
}

void PKBManager::setUsesRelationsH(std::shared_ptr<NumberNode>,
                                   const std::shared_ptr<Node>){};

void PKBManager::setUsesRelationsH(const std::shared_ptr<VariableNode> node,
                                   const std::shared_ptr<Node> parent_node) {
  if (node == nullptr) {
    return;
  }
  variables_set.insert(node->Name);
  uses_set.insert(std::pair<Line, Variable>(getLineFromNode(lines, parent_node),
                                            node->Name));
  addToVectorMap(uses_map, getLineFromNode(lines, parent_node), node->Name);
}

void PKBManager::setUsesRelationsIterator(
    const std::vector<StmtNode> stmt_lst) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setUsesRelations(s); }, stmt);
  }
}

void PKBManager::setModifiesRelations(
    const std::shared_ptr<ProcedureNode> node) {
  setModifiesRelationsIterator(node->StmtList->StmtList);
}

void PKBManager::setModifiesRelations(const std::shared_ptr<PrintNode>) {}

void PKBManager::setModifiesRelations(const std::shared_ptr<IfNode> node) {
  setModifiesRelationsIterator(node->StmtListThen->StmtList);
  setModifiesRelationsIterator(node->StmtListElse->StmtList);
}

void PKBManager::setModifiesRelations(const std::shared_ptr<WhileNode> node) {
  setModifiesRelationsIterator(node->StmtList->StmtList);
}

void PKBManager::setModifiesRelations(const std::shared_ptr<ReadNode> node) {
  setModifiesRelationsH(node->Var, node);
}

void PKBManager::setModifiesRelations(const std::shared_ptr<AssignNode> node) {
  setModifiesRelationsH(node->Var, node);
}

void PKBManager::setModifiesRelationsH(
    const std::shared_ptr<VariableNode> node,
    const std::shared_ptr<Node> parent_node) {
  variables_set.insert(node->Name);
  modifies_set.insert(std::pair<Line, Variable>(
      getLineFromNode(lines, parent_node), node->Name));
  addToVectorMap(modifies_map, getLineFromNode(lines, parent_node), node->Name);
}

void PKBManager::setModifiesRelationsIterator(
    const std::vector<StmtNode> stmt_lst) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setModifiesRelations(s); }, stmt);
  }
}

Procedure PKBManager::getNodeValue(std::shared_ptr<Node> node) {
  if (dynamic_cast<ProcedureNode *>(node.get()) != 0) {
    std::shared_ptr<ProcedureNode> derived =
        std::dynamic_pointer_cast<ProcedureNode>(node);
    return derived->Name;
  } else {
    return getLineFromNode(lines, node);
  }
}

void PKBManager::addToVectorMap(
    std::unordered_map<std::string, std::vector<std::string>> umap,
    std::string index, std::string data) {
  if (umap.find(index) == umap.end()) {
    // create new vector
    std::vector<std::string> v;
    v.push_back(data);
    umap[index] = v;
  } else {
    // retrieve vector and add element
    umap.at(index).push_back(data);
  }
}

// TODO deprecate temp testing methods
bool PKBManager::testParent(ParentLine a, Line b) {
  return parent_set.find(std::pair<ParentLine, Line>(a, b)) != parent_set.end();
}

bool PKBManager::testUses(Line a, Variable b) {
  return uses_set.find(std::pair<Line, Variable>(a, b)) != uses_set.end();
}

bool PKBManager::testModifies(Line a, Variable b) {
  return modifies_set.find(std::pair<Line, Variable>(a, b)) !=
         modifies_set.end();
}

std::vector<Line> PKBManager::getUses(Line line) {
  std::vector<Line> res;
  try {
    res = uses_map.at(line);
    return res;
  } catch (const std::out_of_range &e) {
    return res;
  }
}

// WORKING API
std::vector<Line> PKBManager::getAssignList() {
  return pkb_storage->assign_list;
}

std::vector<Line> PKBManager::getPrintList() { return pkb_storage->print_list; }

std::vector<Line> PKBManager::getReadList() { return pkb_storage->read_list; }

std::vector<Procedure> PKBManager::getProcedureList() {
  return pkb_storage->procedure_list;
}

bool PKBManager::isAssignExists(Line line) {
  if (pkb_storage->assign_set.find(line) != pkb_storage->assign_set.end()) {
    return true;
  } else {
    return false;
  }
}

bool PKBManager::isPrintExists(Line line) {
  if (pkb_storage->print_set.find(line) != pkb_storage->assign_set.end()) {
    return true;
  } else {
    return false;
  }
}

bool PKBManager::isReadExists(Line line) {
  if (pkb_storage->read_set.find(line) != pkb_storage->assign_set.end()) {
    return true;
  } else {
    return false;
  }
}

bool PKBManager::isProcedureExists(Procedure proc) {
  if (pkb_storage->procedure_set.find(proc) !=
      pkb_storage->procedure_set.end()) {
    return true;
  } else {
    return false;
  }
}

bool PKBManager::isLineFollowLine(const LineBefore line_before,
                                  const LineAfter line_after) {
  if (pkb_storage->follows_set.find(std::pair<LineBefore, LineAfter>(
          line_before, line_after)) != pkb_storage->follows_set.end()) {
    return true;
  } else {
    return false;
  }
}

}  // namespace PKB
