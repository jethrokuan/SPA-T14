/*!
  \file   node.h
  \brief  Node definitions for the SIMPLE AST.

  \date   2019-02-05
*/

#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

class Node;
class NumberNode;
class VariableNode;
class ReadNode;
class PrintNode;
class ProcedureNode;
class TermNode;
class TermPNode;
class BinOpNode;
using ExprNNode =
    std::variant<std::shared_ptr<VariableNode>, std::shared_ptr<NumberNode>,
                 std::shared_ptr<BinOpNode>>;
class ExprNode;
class ExprPNode;
class AssignNode;
using FactorNode =
    std::variant<std::shared_ptr<NumberNode>, std::shared_ptr<VariableNode>,
                 std::shared_ptr<ExprNode>>;
using RelFactorNode = FactorNode;
class RelExprNode;
class CondExprNode;
class WhileNode;
class IfNode;

using StmtNode =
    std::variant<std::shared_ptr<ReadNode>, std::shared_ptr<PrintNode>,
                 std::shared_ptr<WhileNode>, std::shared_ptr<IfNode>,
                 std::shared_ptr<AssignNode>>;

//! Abstract base class for other AST nodes.
class Node {
 public:
  virtual ~Node(){};
  virtual bool operator==(const Node& other) const = 0;
  virtual bool operator!=(const Node& other) const {
    return !operator==(other);
  };
};

//! AST Node representing a Statement List
class StmtListNode : public Node {
 public:
  std::vector<StmtNode> StmtList;
  StmtListNode(std::vector<StmtNode> stmtList);
  bool operator==(const Node& other) const;
};

//! AST Node representing a Number (const_value)
class NumberNode : public Node {
 public:
  std::string Val;
  NumberNode(const std::string& val);
  bool operator==(const Node& other) const;
};

//! AST Node representing a Variable
class VariableNode : public Node {
 public:
  std::string Name;
  VariableNode(const std::string& name);
  bool operator==(const Node& other) const;
};

//! AST Node representing a Read statement
class ReadNode : public Node {
 public:
  std::shared_ptr<VariableNode> Var;
  ReadNode(std::shared_ptr<VariableNode> var);
  bool operator==(const Node& other) const;
};

//! AST Node representing a Print statement
class PrintNode : public Node {
 public:
  std::shared_ptr<VariableNode> Var;
  PrintNode(std::shared_ptr<VariableNode> var);
  bool operator==(const Node& other) const;
};

//! AST Node representing a procedure
class ProcedureNode : public Node {
 public:
  std::string Name;
  std::shared_ptr<StmtListNode> StmtList;
  ProcedureNode(const std::string& name,
                std::shared_ptr<StmtListNode> stmtList);
  bool operator==(const Node& other) const;
};

class BinOpNode : public Node {
 public:
  ExprNNode Left;
  ExprNNode Right;
  std::string Op;
  BinOpNode(ExprNNode left, ExprNNode right, std::string& op);
  bool operator==(const Node& other) const;
};

//! AST Node representing term'
class TermPNode : public Node {
 public:
  FactorNode Factor;
  std::string Op;
  std::shared_ptr<TermPNode> TermP;
  TermPNode(FactorNode factor, std::string& op,
            std::shared_ptr<TermPNode> termP);
  bool operator==(const Node& other) const;
};

//! AST Node representing term
class TermNode : public Node {
 public:
  FactorNode Factor;
  std::shared_ptr<TermPNode> TermP;

  TermNode(FactorNode factor, std::shared_ptr<TermPNode> termP);
  TermNode(FactorNode factor);
  bool operator==(const Node& other) const;
};

//! AST Node representing expr'
class ExprPNode : public Node {
 public:
  std::shared_ptr<TermNode> Term;
  std::string Op;
  std::shared_ptr<ExprPNode> ExprP;

  ExprPNode(std::shared_ptr<TermNode> term, std::string& op);
  ExprPNode(std::shared_ptr<TermNode> term, std::string& op,
            std::shared_ptr<ExprPNode> exprP);
  bool operator==(const Node& other) const;
};

//! AST Node representing expr
class ExprNode : public Node {
 public:
  std::shared_ptr<TermNode> Term;
  std::shared_ptr<ExprPNode> ExprP;

  ExprNode(std::shared_ptr<TermNode> term, std::shared_ptr<ExprPNode> exprP);
  ExprNode(std::shared_ptr<TermNode> term);
  bool operator==(const Node& other) const;
};

//! AST Node representing an Assign statement
class AssignNode : public Node {
 public:
  std::shared_ptr<VariableNode> Var;
  std::shared_ptr<ExprNode> Expr;
  AssignNode(std::shared_ptr<VariableNode> var, std::shared_ptr<ExprNode> expr);
  bool operator==(const Node& other) const;
};

//! AST Node representing a RelExpr
class RelExprNode : public Node {
 public:
  RelFactorNode LHS;
  std::string Op;
  RelFactorNode RHS;
  RelExprNode(RelFactorNode lhs, std::string& op, RelFactorNode rhs);
  bool operator==(const Node& other) const;
};

//! AST Node representing a CondExpr
class CondExprNode : public Node {
 public:
  std::shared_ptr<RelExprNode> RelExpr = nullptr;
  std::shared_ptr<CondExprNode> CondLHS = nullptr;
  std::string Op = "";
  std::shared_ptr<CondExprNode> CondRHS = nullptr;

  // rel_expr
  CondExprNode(std::shared_ptr<RelExprNode> relExpr);
  // ! ( cond_expr )
  CondExprNode(std::shared_ptr<CondExprNode> condLHS);
  // ( cond_expr ) && ( cond_expr )
  CondExprNode(std::shared_ptr<CondExprNode> condLHS, std::string& op,
               std::shared_ptr<CondExprNode> condRHS);
  bool operator==(const Node& other) const;
};

//! AST Node representing a While statement
class WhileNode : public Node {
 public:
  std::shared_ptr<CondExprNode> CondExpr;
  std::shared_ptr<StmtListNode> StmtList;

  WhileNode(std::shared_ptr<CondExprNode> condExpr,
            std::shared_ptr<StmtListNode> stmtList);
  bool operator==(const Node& other) const;
};

//! AST Node representing a If statement
class IfNode : public Node {
 public:
  std::shared_ptr<CondExprNode> CondExpr;
  std::shared_ptr<StmtListNode> StmtListThen;
  std::shared_ptr<StmtListNode> StmtListElse;

  IfNode(std::shared_ptr<CondExprNode> condExpr,
         std::shared_ptr<StmtListNode> stmtListThen,
         std::shared_ptr<StmtListNode> stmtListElse);
  bool operator==(const Node& other) const;
};
