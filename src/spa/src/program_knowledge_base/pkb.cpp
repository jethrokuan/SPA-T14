// TODO remove debug print statements and iostream
#include "program_knowledge_base/pkb.h"
#include <iostream>
#include <stack>

PKB::PKB(std::shared_ptr<ProcedureNode> proc) {
  ast = proc;
  setLineNumbers(ast);
  setFollowsRelations(ast);
  // setParentRelations(ast);
}

PKB::~PKB() {}

// TODO change iterative functions to use accumulate function instead
// this should help to reduce the repeated code??

// TODO add return value
void PKB::setLineNumbers(std::shared_ptr<ProcedureNode> proc_node) {
  // iterate through AST via DFS
  std::stack<std::shared_ptr<Node>> visit_stack;
  visit_stack.push(proc_node);
  std::shared_ptr<Node> cur_node;

  while (!visit_stack.empty()) {
    // visit node
    // can only visit top level nodes (those with line numbers)
    cur_node = visit_stack.top();
    visit_stack.pop();
    if (dynamic_cast<IfNode*>(cur_node.get()) != nullptr) {
      // std::cout << "THIS IS A IfNode NODE" << std::endl;
      lines.push_back(cur_node);
    } else if (dynamic_cast<WhileNode*>(cur_node.get()) != nullptr) {
      // std::cout << "THIS IS A WhileNode NODE" << std::endl;
      lines.push_back(cur_node);
    } else if (dynamic_cast<ReadNode*>(cur_node.get()) != nullptr) {
      // std::cout << "THIS IS A ReadNode NODE" << std::endl;
      lines.push_back(cur_node);
    } else if (dynamic_cast<PrintNode*>(cur_node.get()) != nullptr) {
      // std::cout << "THIS IS A PrintNode NODE" << std::endl;
      lines.push_back(cur_node);
    } else if (dynamic_cast<AssignNode*>(cur_node.get()) != nullptr) {
      // std::cout << "THIS IS A AssignNode NODE" << std::endl;
      lines.push_back(cur_node);
    } else {
      // TODO throw error
    }

    // add stmt_lst in
    std::vector<std::shared_ptr<Node>> stmt_lst;
    if (dynamic_cast<ProcedureNode*>(cur_node.get()) != nullptr) {
      // std::cout << "ADDING STATEMENT LIST FOR PROCEDURE NODE" << std::endl;
      std::shared_ptr<ProcedureNode> derived =
          std::dynamic_pointer_cast<ProcedureNode>(cur_node);
      stmt_lst = derived->StmtList->StmtList;
    } else if (dynamic_cast<WhileNode*>(cur_node.get()) != nullptr) {
      // std::cout << "ADDING STATEMENT LIST FOR WHILE NODE" << std::endl;
      std::shared_ptr<WhileNode> derived =
          std::dynamic_pointer_cast<WhileNode>(cur_node);
      stmt_lst = derived->StmtList->StmtList;
    } else if (dynamic_cast<IfNode*>(cur_node.get()) != nullptr) {
      // std::cout << "ADDING STATEMENT LIST FOR IF NODE" << std::endl;
      std::shared_ptr<IfNode> derived =
          std::dynamic_pointer_cast<IfNode>(cur_node);
      std::vector<std::shared_ptr<Node>> then_stmt_lst =
          derived->StmtListThen->StmtList;
      std::vector<std::shared_ptr<Node>> else_stmt_lst =
          derived->StmtListElse->StmtList;
      // concat
      then_stmt_lst.insert(then_stmt_lst.end(), else_stmt_lst.begin(),
                           else_stmt_lst.end());
      stmt_lst = then_stmt_lst;
    }
    // reverse iterator to do DFS
    for (auto it = stmt_lst.rbegin(); it != stmt_lst.rend(); it++) {
      visit_stack.push(*it);
    }
  }
  // std::cout << "\n";
  // std::cout << lines.size() << std::endl;
}

// TODO refactor to use hashmap instead
// would need to implement hash function of the node
// as well as a hash table
// return line number
// returns -1 if node does not exist
int PKB::getNodeLineNumber(std::vector<std::shared_ptr<Node>> ls,
                           std::shared_ptr<Node> node) {
  // loop through vector of nodes
  for (std::size_t i = 0; i < ls.size(); i++) {
    if (ls[i] == node) {
      return i + 1;  // +1 due to 0 based index
    }
  }
  return -1;
}

// TODO set a return value
void PKB::setFollowsRelations(std::shared_ptr<ProcedureNode> proc_node) {
  // iterate through AST via DFS
  std::stack<std::shared_ptr<Node>> visit_stack;
  visit_stack.push(proc_node);
  std::shared_ptr<Node> cur_node;

  // std::vector<std::shared_ptr<Node>> stmt_lst;

  while (!visit_stack.empty()) {
    // visit node
    // can only visit top level nodes (those with line numbers)
    cur_node = visit_stack.top();
    visit_stack.pop();
    // get statement list from node

    // add stmt_lst in
    std::vector<std::shared_ptr<Node>> stmt_lst;
    if (dynamic_cast<ProcedureNode*>(cur_node.get()) != nullptr) {
      // std::cout << "ADDING STATEMENT LIST FOR PROCEDURE NODE" << std::endl;
      std::shared_ptr<ProcedureNode> derived =
          std::dynamic_pointer_cast<ProcedureNode>(cur_node);
      stmt_lst = derived->StmtList->StmtList;
    } else if (dynamic_cast<WhileNode*>(cur_node.get()) != nullptr) {
      // std::cout << "ADDING STATEMENT LIST FOR WHILE NODE" << std::endl;
      std::shared_ptr<WhileNode> derived =
          std::dynamic_pointer_cast<WhileNode>(cur_node);
      stmt_lst = derived->StmtList->StmtList;
    } else if (dynamic_cast<IfNode*>(cur_node.get()) != nullptr) {
      // std::cout << "ADDING STATEMENT LIST FOR IF NODE" << std::endl;
      std::shared_ptr<IfNode> derived =
          std::dynamic_pointer_cast<IfNode>(cur_node);
      std::vector<std::shared_ptr<Node>> then_stmt_lst =
          derived->StmtListThen->StmtList;
      std::vector<std::shared_ptr<Node>> else_stmt_lst =
          derived->StmtListElse->StmtList;
      // concat
      then_stmt_lst.insert(then_stmt_lst.end(), else_stmt_lst.begin(),
                           else_stmt_lst.end());
      stmt_lst = then_stmt_lst;
    }
    if (stmt_lst.size() > 2) {
      // add relations
      for (std::size_t i = 0; i < stmt_lst.size(); i++) {
      int ref_line = getNodeLineNumber(lines, stmt_lst[i]);
        for (std::size_t j = i + 1; j < stmt_lst.size(); j++) {
          int following_line = getNodeLineNumber(lines, stmt_lst[j]);
          follows_set.insert(std::pair<int, int>(ref_line, following_line));
        }
      }
    }
    // reverse iterator to do DFS
    for (auto it = stmt_lst.rbegin(); it != stmt_lst.rend(); it++) {
      visit_stack.push(*it);
    }
  }
}

// ExprNode
// should return a vector of variables used
void PKB::getVariablesUsed() {}

// TODO deprecate temp testing methods
bool PKB::testFollows(int a, int b) {
  return follows_set.find(std::pair<int, int>(a, b)) != follows_set.end();
}