// TODO remove debug print statements and iostream
#include <iostream>
#include <stack>
#include "program_knowledge_base/pkb.h"

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

PKB::PKB(const std::shared_ptr<ProcedureNode> proc) {
  ast = proc;
  setLineNumbers(ast);
  setFollowsRelations(ast);
  setParentRelations(ast);
  setUsesRelations(ast);
  setModifiesRelations(ast);
}

PKB::~PKB() {}

void PKB::setLineNumbers(const std::shared_ptr<ProcedureNode> node) {
  setLineNumbersIterator(node->StmtList->StmtList);
}

void PKB::setLineNumbers(const std::shared_ptr<IfNode> node) {
  lines.push_back(node);
  setLineNumbersIterator(node->StmtListThen->StmtList);
  setLineNumbersIterator(node->StmtListElse->StmtList);
}

void PKB::setLineNumbers(const std::shared_ptr<WhileNode> node) {
  lines.push_back(node);
  setLineNumbersIterator(node->StmtList->StmtList);
}

void PKB::setLineNumbers(const std::shared_ptr<ReadNode> node) {
  lines.push_back(node);
}

void PKB::setLineNumbers(const std::shared_ptr<PrintNode> node) {
  lines.push_back(node);
}

void PKB::setLineNumbers(const std::shared_ptr<AssignNode> node) {
  lines.push_back(node);
}

// TODO add return value
void PKB::setLineNumbersIterator(const std::vector<StmtNode> stmt_lst) {
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
std::string PKB::getLineNumberFromNode(
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

std::shared_ptr<Node> PKB::getNodeFromLineNumber(
    const std::vector<std::shared_ptr<Node>> ls, const int line_number) {
  return ls.at(line_number - 1);  // -1 due to 0 based index
}

// TODO set a return value
// TODO error handling
void PKB::setFollowsRelations(const std::shared_ptr<ProcedureNode> node) {
  setFollowsRelationsIterator(node->StmtList->StmtList);
}

void PKB::setFollowsRelations(const std::shared_ptr<IfNode> node) {
  setFollowsRelationsIterator(node->StmtListThen->StmtList);
  setFollowsRelationsIterator(node->StmtListElse->StmtList);
}

void PKB::setFollowsRelations(const std::shared_ptr<WhileNode> node) {
  setFollowsRelationsIterator(node->StmtList->StmtList);
}

void PKB::setFollowsRelationsIterator(const std::vector<StmtNode> stmt_lst) {
  // add relations
  for (std::size_t i = 0; i < stmt_lst.size() - 1; i++) {
    auto cur_line_number = std::visit(
        [this](const auto &s) { return getLineNumberFromNode(lines, s); },
        stmt_lst[i]);

    auto next_line_number = std::visit(
        [this](const auto &s) { return getLineNumberFromNode(lines, s); },
        stmt_lst[i + 1]);
    follows_set.insert(
        std::pair<LineNumber, LineNumber>(cur_line_number, next_line_number));
    addToVectorMap(follows_map, cur_line_number, next_line_number);
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

void PKB::setParentRelations(const std::shared_ptr<ProcedureNode> node) {
  setParentRelationsIterator(node->StmtList->StmtList, node);
}

void PKB::setParentRelations(const std::shared_ptr<IfNode> node) {
  setParentRelationsIterator(node->StmtListThen->StmtList, node);
  setParentRelationsIterator(node->StmtListElse->StmtList, node);
}

void PKB::setParentRelations(const std::shared_ptr<WhileNode> node) {
  setParentRelationsIterator(node->StmtList->StmtList, node);
}

void PKB::setParentRelationsIterator(const std::vector<StmtNode> stmt_lst,
                                     const std::shared_ptr<Node> parent_node) {
  for (const auto &stmt : stmt_lst) {
    std::visit(
        [this, parent_node](const auto &s) {
          auto current_line = getLineNumberFromNode(lines, s);
          parent_set.insert(std::pair<Parent, LineNumber>(
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

void PKB::setUsesRelations(const std::shared_ptr<ProcedureNode> node) {
  setUsesRelationsIterator(node->StmtList->StmtList);
}

void PKB::setUsesRelations(const std::shared_ptr<IfNode> node) {
  setUsesRelationsH(node->CondExpr, node);
  setUsesRelationsIterator(node->StmtListThen->StmtList);
  setUsesRelationsIterator(node->StmtListElse->StmtList);
}

void PKB::setUsesRelations(const std::shared_ptr<WhileNode> node) {
  setUsesRelationsH(node->CondExpr, node);
  setUsesRelationsIterator(node->StmtList->StmtList);
}

void PKB::setUsesRelations(const std::shared_ptr<PrintNode> node) {
  setUsesRelationsH(node->Var, node);
}

void PKB::setUsesRelations(const std::shared_ptr<AssignNode> node) {
  setUsesRelationsH(node->Expr, node);
}

void PKB::setUsesRelations(const std::shared_ptr<ReadNode>) {}

//
void PKB::setUsesRelationsH(const ExprNode node,
                            std::shared_ptr<Node> parent_node) {
  std::visit(
      [this, parent_node](const auto &n) { setUsesRelationsH(n, parent_node); },
      node);
}

// void PKB::setUsesRelationsH(const FactorNode node,
//                             const std::shared_ptr<Node> parent_node) {
//   std::visit(
//       [this, parent_node](const auto &n) { setUsesRelationsH(n, parent_node);
//       }, node);
// }

void PKB::setUsesRelationsH(const std::shared_ptr<CondExprNode> node,
                            const std::shared_ptr<Node> parent_node) {
  if (node == nullptr) {
    return;
  }
  setUsesRelationsH(node->RelExpr, parent_node);
  setUsesRelationsH(node->CondLHS, parent_node);
  setUsesRelationsH(node->CondRHS, parent_node);
}

void PKB::setUsesRelationsH(std::shared_ptr<RelExprNode> node,
                            const std::shared_ptr<Node> parent_node) {
  if (node == nullptr) {
    return;
  }
  setUsesRelationsH(node->LHS, parent_node);
  setUsesRelationsH(node->RHS, parent_node);
}

// void PKB::setUsesRelationsH(const std::shared_ptr<RelFactorNode> node,
//                             const std::shared_ptr<Node> parent_node) {
//   setUsesRelationsH(node->Var, parent_node);
//   // doesn't seem necessary for now
//   // setUsesRelationsH(node->Val, parent_node);
//   setUsesRelationsH(node->Expr, parent_node);
// }

void PKB::setUsesRelationsH(std::shared_ptr<NumberNode>,
                            const std::shared_ptr<Node>){};

void PKB::setUsesRelationsH(const std::shared_ptr<VariableNode> node,
                            const std::shared_ptr<Node> parent_node) {
  if (node == nullptr) {
    return;
  }
  variables_set.insert(node->Name);
  uses_set.insert(std::pair<LineNumber, VariableName>(
      getLineNumberFromNode(lines, parent_node), node->Name));
  addToVectorMap(uses_map, getLineNumberFromNode(lines, parent_node),
                 node->Name);
}

void PKB::setUsesRelationsIterator(const std::vector<StmtNode> stmt_lst) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setUsesRelations(s); }, stmt);
  }
}

void PKB::setModifiesRelations(const std::shared_ptr<ProcedureNode> node) {
  setModifiesRelationsIterator(node->StmtList->StmtList);
}

void PKB::setModifiesRelations(const std::shared_ptr<PrintNode>) {}

void PKB::setModifiesRelations(const std::shared_ptr<IfNode> node) {
  setModifiesRelationsIterator(node->StmtListThen->StmtList);
  setModifiesRelationsIterator(node->StmtListElse->StmtList);
}

void PKB::setModifiesRelations(const std::shared_ptr<WhileNode> node) {
  setModifiesRelationsIterator(node->StmtList->StmtList);
}

void PKB::setModifiesRelations(const std::shared_ptr<ReadNode> node) {
  setModifiesRelationsH(node->Var, node);
}

void PKB::setModifiesRelations(const std::shared_ptr<AssignNode> node) {
  setModifiesRelationsH(node->Var, node);
}

void PKB::setModifiesRelationsH(const std::shared_ptr<VariableNode> node,
                                const std::shared_ptr<Node> parent_node) {
  variables_set.insert(node->Name);
  modifies_set.insert(std::pair<LineNumber, VariableName>(
      getLineNumberFromNode(lines, parent_node), node->Name));
  addToVectorMap(modifies_map, getLineNumberFromNode(lines, parent_node),
                 node->Name);
}

void PKB::setModifiesRelationsIterator(const std::vector<StmtNode> stmt_lst) {
  for (const auto &stmt : stmt_lst) {
    std::visit([this](const auto &s) { setModifiesRelations(s); }, stmt);
  }
}

ProcedureName PKB::getNodeValue(std::shared_ptr<Node> node) {
  if (dynamic_cast<ProcedureNode *>(node.get()) != 0) {
    std::shared_ptr<ProcedureNode> derived =
        std::dynamic_pointer_cast<ProcedureNode>(node);
    return derived->Name;
  } else {
    return getLineNumberFromNode(lines, node);
  }
}

// TODO deprecate temp testing methods
bool PKB::testFollows(LineNumber a, LineNumber b) {
  return follows_set.find(std::pair<LineNumber, LineNumber>(a, b)) !=
         follows_set.end();
}

bool PKB::testParent(Parent a, LineNumber b) {
  return parent_set.find(std::pair<Parent, LineNumber>(a, b)) !=
         parent_set.end();
}

bool PKB::testUses(LineNumber a, VariableName b) {
  return uses_set.find(std::pair<LineNumber, VariableName>(a, b)) !=
         uses_set.end();
}

bool PKB::testModifies(LineNumber a, VariableName b) {
  return modifies_set.find(std::pair<LineNumber, VariableName>(a, b)) !=
         modifies_set.end();
}

std::vector<LineNumber> PKB::getFollows(LineNumber line) {
  std::vector<LineNumber> res;
  try {
    res = follows_map.at(line);
    return res;
  } catch (const std::out_of_range &e) {
    return res;
  }
}

std::vector<LineNumber> PKB::getUses(LineNumber line) {
  std::vector<LineNumber> res;
  try {
    res = uses_map.at(line);
    return res;
  } catch (const std::out_of_range &e) {
    return res;
  }
}

// TODO shift this out to a utils component
void PKB::addToVectorMap(
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
