#ifndef NODE_H
#define NODE_H
#include <memory>
#include <vector>
#include <string>

class Node;
class NumberNode;
class VariableNode;
class ReadNode;
class PrintNode;
class FactorNode;
class ProcedureNode;
class TermNode;
class TermPNode;
class ExprNode;
class ExprPNode;
class AssignNode;
class FactorNode;
using RelFactorNode = FactorNode;
class RelExprNode;
class CondExprNode;
class WhileNode;

class Node {
 public:
  virtual ~Node(){};
  virtual bool operator==(const Node& other) const = 0;
  virtual bool operator!=(const Node& other) const {
    return !operator==(other);
  };
};

class StmtListNode : public Node {
 public:
  std::vector<std::shared_ptr<Node>> StmtList;
  StmtListNode(std::vector<std::shared_ptr<Node>> stmtList);
  bool operator==(const Node& other) const;
};

class NumberNode : public Node {
 public:
  std::string Val;
  NumberNode(const std::string& val);
  bool operator==(const Node& other) const;
};

class VariableNode : public Node {
 public:
  std::string Name;
  VariableNode(const std::string& name);
  bool operator==(const Node& other) const;
};

class ReadNode : public Node {
 public:
  std::shared_ptr<VariableNode> Var;
  ReadNode(std::shared_ptr<VariableNode> var);
  bool operator==(const Node& other) const;
};

class PrintNode : public Node {
 public:
  std::shared_ptr<VariableNode> Var;
  PrintNode(std::shared_ptr<VariableNode> var);
  bool operator==(const Node& other) const;
};

class ProcedureNode : public Node {
 public:
  std::string Name;
  std::shared_ptr<StmtListNode> StmtList;
  ProcedureNode(const std::string& name,
                std::shared_ptr<StmtListNode> stmtList);
  bool operator==(const Node& other) const;
};

class TermPNode : public Node {
 public:
  std::shared_ptr<FactorNode> Factor;
  std::string Op;
  std::shared_ptr<TermPNode> TermP;
  TermPNode(std::shared_ptr<FactorNode> factor, std::string& op,
            std::shared_ptr<TermPNode> termP);
  bool operator==(const Node& other) const;
};

class TermNode : public Node {
 public:
  std::shared_ptr<FactorNode> Factor;
  std::shared_ptr<TermPNode> TermP;

  TermNode(std::shared_ptr<FactorNode> factor,
           std::shared_ptr<TermPNode> termP);
  TermNode(std::shared_ptr<FactorNode> factor);
  bool operator==(const Node& other) const;
};

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

class ExprNode : public Node {
 public:
  std::shared_ptr<TermNode> Term;
  std::shared_ptr<ExprPNode> ExprP;

  ExprNode(std::shared_ptr<TermNode> term, std::shared_ptr<ExprPNode> exprP);
  ExprNode(std::shared_ptr<TermNode> term);
  bool operator==(const Node& other) const;
};

class FactorNode : public Node {
 public:
  union {
    std::shared_ptr<VariableNode> Var;
    std::shared_ptr<NumberNode> Val;
    std::shared_ptr<ExprNode> Expr;
  };
  enum class UnionType { VAR, VAL, EXPR, NONE } type = UnionType::NONE;
  FactorNode();
  FactorNode(std::shared_ptr<VariableNode> var);
  FactorNode(std::shared_ptr<NumberNode> val);
  FactorNode(std::shared_ptr<ExprNode> expr);
  ~FactorNode();
  bool operator==(const Node& other) const;
};

class AssignNode : public Node {
 public:
  std::shared_ptr<VariableNode> Var;
  std::shared_ptr<ExprNode> Expr;
  AssignNode(std::shared_ptr<VariableNode> var, std::shared_ptr<ExprNode> expr);
  bool operator==(const Node& other) const;
};

class RelExprNode : public Node {
 public:
  std::shared_ptr<RelFactorNode> LHS;
  std::string Op;
  std::shared_ptr<RelFactorNode> RHS;
  RelExprNode(std::shared_ptr<RelFactorNode> lhs, std::string& op,
              std::shared_ptr<RelFactorNode> rhs);
  bool operator==(const Node& other) const;
};

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

class WhileNode : public Node {
 public:
  std::shared_ptr<CondExprNode> CondExpr;
  std::shared_ptr<StmtListNode> StmtList;

  WhileNode(std::shared_ptr<CondExprNode> condExpr,
            std::shared_ptr<StmtListNode> stmtList);
  bool operator==(const Node& other) const;
};

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
#endif