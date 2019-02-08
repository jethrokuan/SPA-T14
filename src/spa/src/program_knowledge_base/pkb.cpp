// TODO remove debug print statements and iostream
#include "program_knowledge_base/pkb.h"
#include <iostream>
#include <stack>

PKB::PKB(std::shared_ptr<ProcedureNode> proc) {
  ast = proc;
  setLineNumbers(ast);
  setFollowsRelations(ast);
  setParentRelations(ast);
  setUsesRelations(ast);
  setModifiesRelations(ast);
}

PKB::~PKB() {}

// TODO change iterative functions to use accumulate function instead
// this should help to reduce the repeated code??
// could also standardise to use recursion (parent is using that)

// TODO add return value
void PKB::setLineNumbers(std::shared_ptr<Node> node) {
  // iterate through AST via DFS

  // visit node
  // can only visit top level nodes (those with line numbers)
  if (dynamic_cast<IfNode *>(node.get()) != 0) {
    lines.push_back(node);
  } else if (dynamic_cast<WhileNode *>(node.get()) != 0) {
    lines.push_back(node);
  } else if (dynamic_cast<ReadNode *>(node.get()) != 0) {
    lines.push_back(node);
  } else if (dynamic_cast<PrintNode *>(node.get()) != 0) {
    lines.push_back(node);
  } else if (dynamic_cast<AssignNode *>(node.get()) != 0) {
    lines.push_back(node);
  } else {
    // TODO throw error
  }

  // add stmt_lst in
  std::vector<std::shared_ptr<Node>> stmt_lst;
  if (dynamic_cast<ProcedureNode *>(node.get()) != 0) {
    std::shared_ptr<ProcedureNode> derived =
        std::dynamic_pointer_cast<ProcedureNode>(node);
    stmt_lst = derived->StmtList->StmtList;
  } else if (dynamic_cast<WhileNode *>(node.get()) != 0) {
    std::shared_ptr<WhileNode> derived =
        std::dynamic_pointer_cast<WhileNode>(node);
    stmt_lst = derived->StmtList->StmtList;
  } else if (dynamic_cast<IfNode *>(node.get()) != 0) {
    std::shared_ptr<IfNode> derived = std::dynamic_pointer_cast<IfNode>(node);
    std::vector<std::shared_ptr<Node>> then_stmt_lst =
        derived->StmtListThen->StmtList;
    std::vector<std::shared_ptr<Node>> else_stmt_lst =
        derived->StmtListElse->StmtList;
    then_stmt_lst.insert(then_stmt_lst.end(), else_stmt_lst.begin(),
                         else_stmt_lst.end());  // concat
    stmt_lst = then_stmt_lst;
  }
  // reverse iterator to do DFS
  for (auto it = stmt_lst.begin(); it != stmt_lst.end(); it++) {
    setLineNumbers(*it);
  }
}

// TODO refactor to use hashmap instead
// would need to implement hash function of the node
// as well as a hash table
// return line number
// returns -1 if node does not exist
int PKB::getLineNumberFromNode(std::vector<std::shared_ptr<Node>> ls,
                               std::shared_ptr<Node> node) {
  // loop through vector of nodes
  for (std::size_t i = 0; i < ls.size(); i++) {
    if (ls[i] == node) {
      return i + 1;  // +1 due to 0 based index
    }
  }
  return -1;
}

std::shared_ptr<Node> PKB::getNodeFromLineNumber(
    std::vector<std::shared_ptr<Node>> ls, int line_number) {
  return ls.at(line_number - 1);  // -1 due to 0 based index
}

// TODO BUG: else statement list currently follows if's
// TODO set a return value
// TODO error handling
void PKB::setFollowsRelations(std::shared_ptr<Node> node) {
  // iterate through AST via BFS

  // can only visit top level nodes (those with line numbers)
  // get statement list from node
  std::vector<std::shared_ptr<Node>> stmt_lst;
  if (dynamic_cast<ProcedureNode *>(node.get()) != 0) {
    std::shared_ptr<ProcedureNode> derived =
        std::dynamic_pointer_cast<ProcedureNode>(node);
    stmt_lst = derived->StmtList->StmtList;
  } else if (dynamic_cast<WhileNode *>(node.get()) != 0) {
    std::shared_ptr<WhileNode> derived =
        std::dynamic_pointer_cast<WhileNode>(node);
    stmt_lst = derived->StmtList->StmtList;
  } else if (dynamic_cast<IfNode *>(node.get()) != 0) {
    std::shared_ptr<IfNode> derived = std::dynamic_pointer_cast<IfNode>(node);
    std::vector<std::shared_ptr<Node>> then_stmt_lst =
        derived->StmtListThen->StmtList;
    std::vector<std::shared_ptr<Node>> else_stmt_lst =
        derived->StmtListElse->StmtList;
    // concat
    then_stmt_lst.insert(then_stmt_lst.end(), else_stmt_lst.begin(),
                         else_stmt_lst.end());
    stmt_lst = then_stmt_lst;
  }
  // add relations
  for (std::size_t i = 0; i < stmt_lst.size(); i++) {
    int ref_line = getLineNumberFromNode(lines, stmt_lst[i]);
    for (std::size_t j = i + 1; j < stmt_lst.size(); j++) {
      int following_line = getLineNumberFromNode(lines, stmt_lst[j]);
      follows_set.insert(std::pair<int, int>(ref_line, following_line));
      if (follows_map.find(ref_line) == follows_map.end()) {
        // create new vector
        std::vector<int> v;
        v.push_back(following_line);
        follows_map[ref_line] = v;
      } else {
        // retrieve vector and add element
        follows_map.at(ref_line).push_back(following_line);
      }
    }
  }
  // reverse iterator to do DFS
  for (auto it = stmt_lst.begin(); it != stmt_lst.end(); it++) {
    setFollowsRelations(*it);
  }
}

// helper function for setParentRelations
void PKB::setParentRelations(std::shared_ptr<Node> node) {
  // iterate through AST via DFS

  // visit node
  // can only visit top level nodes (those with line numbers)
  // get statement list from node
  std::vector<std::shared_ptr<Node>> stmt_lst;
  if (dynamic_cast<ProcedureNode *>(node.get()) != 0) {
    std::shared_ptr<ProcedureNode> derived =
        std::dynamic_pointer_cast<ProcedureNode>(node);
    stmt_lst = derived->StmtList->StmtList;
  } else if (dynamic_cast<WhileNode *>(node.get()) != 0) {
    std::shared_ptr<WhileNode> derived =
        std::dynamic_pointer_cast<WhileNode>(node);
    stmt_lst = derived->StmtList->StmtList;
  } else if (dynamic_cast<IfNode *>(node.get()) != 0) {
    std::shared_ptr<IfNode> derived = std::dynamic_pointer_cast<IfNode>(node);
    std::vector<std::shared_ptr<Node>> then_stmt_lst =
        derived->StmtListThen->StmtList;
    std::vector<std::shared_ptr<Node>> else_stmt_lst =
        derived->StmtListElse->StmtList;
    // concat
    then_stmt_lst.insert(then_stmt_lst.end(), else_stmt_lst.begin(),
                         else_stmt_lst.end());
    stmt_lst = then_stmt_lst;
  }

  int current_line = getLineNumberFromNode(lines, node);
  bool is_statement_node = false;

  // if there are children for the node
  for (auto it = stmt_lst.begin(); it != stmt_lst.end(); it++) {
    // if statement node
    if (dynamic_cast<IfNode *>((*it).get()) != 0) {
      is_statement_node = true;
    } else if (dynamic_cast<WhileNode *>((*it).get()) != 0) {
      is_statement_node = true;
    } else if (dynamic_cast<ReadNode *>((*it).get()) != 0) {
      is_statement_node = true;
    } else if (dynamic_cast<PrintNode *>((*it).get()) != 0) {
      is_statement_node = true;
    } else if (dynamic_cast<AssignNode *>((*it).get()) != 0) {
      is_statement_node = true;
    }

    if (is_statement_node) {
      int following_line = getLineNumberFromNode(lines, *it);
      parent_set.insert(std::pair<int, int>(current_line, following_line));
      setParentRelations(*it);
      is_statement_node = false;
      // DEBUG
      // std::cout << current_line;
      // std::cout << " is the parent of ";
      // std::cout << following_line << std::endl;
    }
  }
}

void PKB::setUsesRelations(std::shared_ptr<ProcedureNode> proc_node) {
  // iterate through AST via DFS
  std::stack<std::shared_ptr<Node>> visit_stack;
  visit_stack.push(proc_node);
  std::shared_ptr<Node> cur_node;

  while (!visit_stack.empty()) {
    // visit node
    // can only visit top level nodes (those with line numbers)
    cur_node = visit_stack.top();
    visit_stack.pop();

    // call helper function to traverse down the nodes to form relationships
    int line_number;
    if (dynamic_cast<AssignNode *>(cur_node.get()) != 0) {
      std::shared_ptr<AssignNode> derived =
          std::dynamic_pointer_cast<AssignNode>(cur_node);
      line_number = getLineNumberFromNode(lines, derived);
      setUsesRelationsHelper(derived->Expr, line_number);
    } else if (dynamic_cast<IfNode *>(cur_node.get()) != 0) {
      std::shared_ptr<IfNode> derived =
          std::dynamic_pointer_cast<IfNode>(cur_node);
      line_number = getLineNumberFromNode(lines, derived);
      setUsesRelationsHelper(derived->CondExpr, line_number);
    } else if (dynamic_cast<WhileNode *>(cur_node.get()) != 0) {
      std::shared_ptr<WhileNode> derived =
          std::dynamic_pointer_cast<WhileNode>(cur_node);
      line_number = getLineNumberFromNode(lines, derived);
      setUsesRelationsHelper(derived->CondExpr, line_number);
    } else if (dynamic_cast<PrintNode *>(cur_node.get()) != 0) {
      std::shared_ptr<PrintNode> derived =
          std::dynamic_pointer_cast<PrintNode>(cur_node);
      line_number = getLineNumberFromNode(lines, derived);
      setUsesRelationsHelper(derived->Var, line_number);
    } else {
      // TODO throw error
    }

    // add stmt_lst in
    std::vector<std::shared_ptr<Node>> stmt_lst;
    if (dynamic_cast<ProcedureNode *>(cur_node.get()) != 0) {
      std::shared_ptr<ProcedureNode> derived =
          std::dynamic_pointer_cast<ProcedureNode>(cur_node);
      stmt_lst = derived->StmtList->StmtList;
    } else if (dynamic_cast<WhileNode *>(cur_node.get()) != 0) {
      std::shared_ptr<WhileNode> derived =
          std::dynamic_pointer_cast<WhileNode>(cur_node);
      stmt_lst = derived->StmtList->StmtList;
    } else if (dynamic_cast<IfNode *>(cur_node.get()) != 0) {
      std::shared_ptr<IfNode> derived =
          std::dynamic_pointer_cast<IfNode>(cur_node);
      std::vector<std::shared_ptr<Node>> then_stmt_lst =
          derived->StmtListThen->StmtList;
      std::vector<std::shared_ptr<Node>> else_stmt_lst =
          derived->StmtListElse->StmtList;
      then_stmt_lst.insert(then_stmt_lst.end(), else_stmt_lst.begin(),
                           else_stmt_lst.end());  // concat
      stmt_lst = then_stmt_lst;
    }
    // reverse iterator to do DFS
    for (auto it = stmt_lst.rbegin(); it != stmt_lst.rend(); it++) {
      visit_stack.push(*it);
    }
  }
}

// recursive function
// node and vector as arguments
void PKB::setUsesRelationsHelper(std::shared_ptr<Node> node, int line_number) {
  if (dynamic_cast<ExprNode *>(node.get()) != 0) {
    std::shared_ptr<ExprNode> derived =
        std::dynamic_pointer_cast<ExprNode>(node);
    // check if it has a ExprPNode
    if (derived->ExprP != 0) {
      // traverse both TermNode and ExprPNode
      setUsesRelationsHelper(derived->ExprP, line_number);
      setUsesRelationsHelper(derived->Term, line_number);
    } else {
      // traverse down TermNode only
      setUsesRelationsHelper(derived->Term, line_number);
    }
  } else if (dynamic_cast<ExprPNode *>(node.get()) != 0) {
    std::shared_ptr<ExprPNode> derived =
        std::dynamic_pointer_cast<ExprPNode>(node);
    // check if it has a ExprPNode
    if (derived->ExprP != 0) {
      // traverse both TermNode and ExprPNode
      setUsesRelationsHelper(derived->ExprP, line_number);
      setUsesRelationsHelper(derived->Term, line_number);
    } else {
      // traverse down TermNode only
      setUsesRelationsHelper(derived->Term, line_number);
    }
  } else if (dynamic_cast<TermNode *>(node.get()) != 0) {
    std::shared_ptr<TermNode> derived =
        std::dynamic_pointer_cast<TermNode>(node);
    // check if it has a ExprPNode
    if (derived->TermP != 0) {
      // traverse both TermPNode and FactorPNode
      setUsesRelationsHelper(derived->TermP, line_number);
      setUsesRelationsHelper(derived->Factor, line_number);
    } else {
      // traverse down TermNode only
      setUsesRelationsHelper(derived->Factor, line_number);
    }
  } else if (dynamic_cast<TermPNode *>(node.get()) != 0) {
    std::shared_ptr<TermPNode> derived =
        std::dynamic_pointer_cast<TermPNode>(node);
    setUsesRelationsHelper(derived->TermP, line_number);
    setUsesRelationsHelper(derived->Factor, line_number);
  } else if (dynamic_cast<FactorNode *>(node.get()) != 0) {
    std::shared_ptr<FactorNode> derived =
        std::dynamic_pointer_cast<FactorNode>(node);
    // check if it has a ExprPNode
    if (derived->Var != 0) {
      setUsesRelationsHelper(derived->Var, line_number);
    } else if (derived->Val != 0) {
      setUsesRelationsHelper(derived->Val, line_number);
    } else {
      setUsesRelationsHelper(derived->Expr, line_number);
    }
  } else if (dynamic_cast<CondExprNode *>(node.get()) != 0) {
    std::shared_ptr<CondExprNode> derived =
        std::dynamic_pointer_cast<CondExprNode>(node);
    if (derived->RelExpr != 0) {
      setUsesRelationsHelper(derived->RelExpr, line_number);
    } else if (derived->CondRHS != 0) {
      setUsesRelationsHelper(derived->CondRHS, line_number);
    } else {
      setUsesRelationsHelper(derived->CondLHS, line_number);
      setUsesRelationsHelper(derived->CondRHS, line_number);
    }
  } else if (dynamic_cast<RelExprNode *>(node.get()) != 0) {
    std::shared_ptr<RelExprNode> derived =
        std::dynamic_pointer_cast<RelExprNode>(node);
    setUsesRelationsHelper(derived->LHS, line_number);
    setUsesRelationsHelper(derived->RHS, line_number);
  } else if (dynamic_cast<RelFactorNode *>(node.get()) != 0) {
    std::shared_ptr<RelFactorNode> derived =
        std::dynamic_pointer_cast<RelFactorNode>(node);
    // check if it has a ExprPNode
    if (derived->Var != 0) {
      setUsesRelationsHelper(derived->Var, line_number);
    } else if (derived->Val != 0) {
      setUsesRelationsHelper(derived->Val, line_number);
    } else {
      setUsesRelationsHelper(derived->Expr, line_number);
    }
  } else if (dynamic_cast<VariableNode *>(node.get()) != 0) {
    std::shared_ptr<VariableNode> derived =
        std::dynamic_pointer_cast<VariableNode>(node);
    // add to map
    // TODO abstract this function
    uses_set.insert(std::pair<int, std::string>(line_number, derived->Name));
    if (uses_map.find(line_number) == uses_map.end()) {
      // create new vector
      std::vector<std::string> v;
      v.push_back(derived->Name);
      uses_map[line_number] = v;
    } else {
      // retrieve vector and add element
      uses_map.at(line_number).push_back(derived->Name);
    }
  } else {
    // throw error
  }
}

void PKB::setModifiesRelations(std::shared_ptr<ProcedureNode> proc_node) {
  // iterate through AST via DFS
  std::stack<std::shared_ptr<Node>> visit_stack;
  visit_stack.push(proc_node);
  std::shared_ptr<Node> cur_node;

  while (!visit_stack.empty()) {
    // visit node
    // can only visit top level nodes (those with line numbers)
    cur_node = visit_stack.top();
    visit_stack.pop();

    // call helper function to traverse down the nodes to form relationships
    int line_number;
    std::vector<std::shared_ptr<Node>> stmt_lst;

    if (dynamic_cast<AssignNode *>(cur_node.get()) != 0) {
      std::shared_ptr<AssignNode> derived =
          std::dynamic_pointer_cast<AssignNode>(cur_node);
      line_number = getLineNumberFromNode(lines, derived);
      setModifiesRelationsHelper(derived->Var, line_number);
    } else if (dynamic_cast<IfNode *>(cur_node.get()) != 0) {
      std::shared_ptr<IfNode> derived =
          std::dynamic_pointer_cast<IfNode>(cur_node);
      line_number = getLineNumberFromNode(lines, derived);
      std::vector<std::shared_ptr<Node>> then_stmt_lst =
          derived->StmtListThen->StmtList;
      std::vector<std::shared_ptr<Node>> else_stmt_lst =
          derived->StmtListElse->StmtList;
      then_stmt_lst.insert(then_stmt_lst.end(), else_stmt_lst.begin(),
                           else_stmt_lst.end());  // concat
      stmt_lst = then_stmt_lst;
      for (auto it = stmt_lst.begin(); it != stmt_lst.end(); it++) {
        setModifiesRelationsHelper(*it, line_number);
      }
    } else if (dynamic_cast<WhileNode *>(cur_node.get()) != 0) {
      std::shared_ptr<WhileNode> derived =
          std::dynamic_pointer_cast<WhileNode>(cur_node);
      line_number = getLineNumberFromNode(lines, derived);
      stmt_lst = derived->StmtList->StmtList;
      for (auto it = stmt_lst.begin(); it != stmt_lst.end(); it++) {
        setModifiesRelationsHelper(*it, line_number);
      }
    } else if (dynamic_cast<ReadNode *>(cur_node.get()) != 0) {
      std::shared_ptr<ReadNode> derived =
          std::dynamic_pointer_cast<ReadNode>(cur_node);
      line_number = getLineNumberFromNode(lines, derived);
      setModifiesRelationsHelper(derived->Var, line_number);
    } else {
      // TODO throw error
    }

    // DFS
    if (dynamic_cast<ProcedureNode *>(cur_node.get()) != 0) {
      std::shared_ptr<ProcedureNode> derived =
          std::dynamic_pointer_cast<ProcedureNode>(cur_node);
      stmt_lst = derived->StmtList->StmtList;
    } else if (dynamic_cast<WhileNode *>(cur_node.get()) != 0) {
      std::shared_ptr<WhileNode> derived =
          std::dynamic_pointer_cast<WhileNode>(cur_node);
      stmt_lst = derived->StmtList->StmtList;
    } else if (dynamic_cast<IfNode *>(cur_node.get()) != 0) {
      std::shared_ptr<IfNode> derived =
          std::dynamic_pointer_cast<IfNode>(cur_node);
      std::vector<std::shared_ptr<Node>> then_stmt_lst =
          derived->StmtListThen->StmtList;
      std::vector<std::shared_ptr<Node>> else_stmt_lst =
          derived->StmtListElse->StmtList;
      then_stmt_lst.insert(then_stmt_lst.end(), else_stmt_lst.begin(),
                           else_stmt_lst.end());  // concat
      stmt_lst = then_stmt_lst;
    }
    // reverse iterator to do DFS
    for (auto it = stmt_lst.rbegin(); it != stmt_lst.rend(); it++) {
      visit_stack.push(*it);
    }
  }
}

// recursive function
// node and vector as arguments
void PKB::setModifiesRelationsHelper(std::shared_ptr<Node> node,
                                     int line_number) {
  if (dynamic_cast<VariableNode *>(node.get()) != 0) {
    std::shared_ptr<VariableNode> derived =
        std::dynamic_pointer_cast<VariableNode>(node);
    // add to map
    // TODO abstract this function
    modifies_set.insert(
        std::pair<int, std::string>(line_number, derived->Name));
    if (modifies_map.find(line_number) == modifies_map.end()) {
      // create new vector
      std::vector<std::string> v;
      v.push_back(derived->Name);
      modifies_map[line_number] = v;
    } else {
      // retrieve vector and add element
      modifies_map.at(line_number).push_back(derived->Name);
    }
  } else {
    // throw error
  }
}

// TODO deprecate temp testing methods
bool PKB::testFollows(int a, int b) {
  return follows_set.find(std::pair<int, int>(a, b)) != follows_set.end();
}

std::vector<int> PKB::getFollows(int line) {
  std::vector<int> res;
  try {
    res = follows_map.at(line);
    return res;
  } catch (const std::out_of_range &e) {
    return res;
  }
}

bool PKB::testParent(int a, int b) {
  return parent_set.find(std::pair<int, int>(a, b)) != parent_set.end();
}

bool PKB::testUses(int line, std::string v) {
  return uses_set.find(std::pair<int, std::string>(line, v)) != uses_set.end();
}

bool PKB::testModifies(int line, std::string v) {
  return modifies_set.find(std::pair<int, std::string>(line, v)) !=
         modifies_set.end();
}

std::vector<std::string> PKB::getUses(int line) {
  std::vector<std::string> res;
  try {
    res = uses_map.at(line);
    return res;
  } catch (const std::out_of_range &e) {
    return res;
  }
}
