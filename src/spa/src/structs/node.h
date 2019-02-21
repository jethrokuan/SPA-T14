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
class BinOpNode;
using Expr =
    std::variant<std::shared_ptr<VariableNode>, std::shared_ptr<NumberNode>,
                 std::shared_ptr<BinOpNode>>;
class AssignNode;
using Factor =
    std::variant<std::shared_ptr<VariableNode>, std::shared_ptr<NumberNode>,
                 std::shared_ptr<BinOpNode>>;
using RelFactor = Factor;
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
  virtual ~Node() = default;
  virtual bool operator==(const Node& other) const = 0;
  virtual bool operator!=(const Node& other) const {
    return !operator==(other);
  };
  virtual std::string to_str() = 0;
};

//! AST Node representing a Statement List
class StmtListNode : public Node {
 public:
  std::vector<StmtNode> StmtList;
  explicit StmtListNode(std::vector<StmtNode> stmtList);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

//! AST Node representing a Number (const_value)
class NumberNode : public Node {
 public:
  std::string Val;
  explicit NumberNode(const std::string val);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

//! AST Node representing a Variable
class VariableNode : public Node {
 public:
  std::string Name;
  explicit VariableNode(const std::string name);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

//! AST Node representing a Read statement
class ReadNode : public Node {
 public:
  std::shared_ptr<VariableNode> Var;
  explicit ReadNode(std::shared_ptr<VariableNode> var);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

//! AST Node representing a Print statement
class PrintNode : public Node {
 public:
  std::shared_ptr<VariableNode> Var;
  explicit PrintNode(std::shared_ptr<VariableNode> var);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

//! AST Node representing a procedure
class ProcedureNode : public Node {
 public:
  std::string Name;
  std::shared_ptr<StmtListNode> StmtList;
  explicit ProcedureNode(const std::string name,
                         std::shared_ptr<StmtListNode> stmtList);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

class BinOpNode : public Node {
 public:
  Expr Left;
  Expr Right;
  std::string Op;
  explicit BinOpNode(Expr left, Expr right, std::string op);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

//! AST Node representing an Assign statement
class AssignNode : public Node {
 public:
  std::shared_ptr<VariableNode> Var;
  Expr Exp;
  explicit AssignNode(std::shared_ptr<VariableNode> var, Expr exp);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

//! AST Node representing a RelExpr
class RelExprNode : public Node {
 public:
  RelFactor LHS;
  std::string Op;
  RelFactor RHS;
  explicit RelExprNode(RelFactor lhs, std::string op, RelFactor rhs);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

//! AST Node representing a CondExpr
class CondExprNode : public Node {
 public:
  std::shared_ptr<RelExprNode> RelExpr = nullptr;
  std::shared_ptr<CondExprNode> CondLHS = nullptr;
  std::string Op = "";
  std::shared_ptr<CondExprNode> CondRHS = nullptr;

  // rel_expr
  explicit CondExprNode(std::shared_ptr<RelExprNode> relExpr);
  // ! ( cond_expr )
  explicit CondExprNode(std::shared_ptr<CondExprNode> condLHS);
  // ( cond_expr ) && ( cond_expr )
  explicit CondExprNode(std::shared_ptr<CondExprNode> condLHS, std::string op,
                        std::shared_ptr<CondExprNode> condRHS);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

//! AST Node representing a While statement
class WhileNode : public Node {
 public:
  std::shared_ptr<CondExprNode> CondExpr;
  std::shared_ptr<StmtListNode> StmtList;

  explicit WhileNode(std::shared_ptr<CondExprNode> condExpr,
                     std::shared_ptr<StmtListNode> stmtList);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};

//! AST Node representing a If statement
class IfNode : public Node {
 public:
  std::shared_ptr<CondExprNode> CondExpr;
  std::shared_ptr<StmtListNode> StmtListThen;
  std::shared_ptr<StmtListNode> StmtListElse;

  explicit IfNode(std::shared_ptr<CondExprNode> condExpr,
                  std::shared_ptr<StmtListNode> stmtListThen,
                  std::shared_ptr<StmtListNode> stmtListElse);
  bool operator==(const Node& other) const override;
  std::string to_str() override;
};
