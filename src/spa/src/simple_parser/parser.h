#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "simple_parser/token.h"

namespace Simple {
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
};  // namespace Simple

class StmtListNode : public Node {
 public:
  std::vector<std::unique_ptr<Node>> StmtList;
  StmtListNode(std::vector<std::unique_ptr<Node>> stmtList);
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
  std::unique_ptr<Node> Var;
  ReadNode(std::unique_ptr<VariableNode> var);
  bool operator==(const Node& other) const;
};

class PrintNode : public Node {
 public:
  std::unique_ptr<Node> Var;
  PrintNode(std::unique_ptr<Node> var);
  bool operator==(const Node& other) const;
};

class ProcedureNode : public Node {
 public:
  std::unique_ptr<Node> Var;
  std::unique_ptr<StmtListNode> StmtList;
  ProcedureNode(std::unique_ptr<Node> var,
                std::unique_ptr<StmtListNode> stmtList);
  bool operator==(const Node& other) const;
};

class TermPNode : public Node {
 public:
  std::unique_ptr<FactorNode> Factor;
  std::string Op;
  std::unique_ptr<TermPNode> TermP;
  TermPNode(std::unique_ptr<FactorNode> factor, std::string& op,
            std::unique_ptr<TermPNode> termP);
  bool operator==(const Node& other) const;
};

class TermNode : public Node {
 public:
  std::unique_ptr<FactorNode> Factor;
  std::unique_ptr<TermPNode> TermP;

  TermNode(std::unique_ptr<FactorNode> factor,
           std::unique_ptr<TermPNode> termP);
  TermNode(std::unique_ptr<FactorNode> factor);
  bool operator==(const Node& other) const;
};

class ExprPNode : public Node {
 public:
  std::unique_ptr<TermNode> Term;
  std::string Op;
  std::unique_ptr<ExprPNode> ExprP;

  ExprPNode(std::unique_ptr<TermNode> term, std::string& op);
  ExprPNode(std::unique_ptr<TermNode> term, std::string& op,
            std::unique_ptr<ExprPNode> exprP);
  bool operator==(const Node& other) const;
};

class ExprNode : public Node {
 public:
  std::unique_ptr<TermNode> Term;
  std::unique_ptr<ExprPNode> ExprP;

  ExprNode(std::unique_ptr<TermNode> term, std::unique_ptr<ExprPNode> exprP);
  ExprNode(std::unique_ptr<TermNode> term);
  bool operator==(const Node& other) const;
};

class FactorNode : public Node {
 public:
  union {
    std::unique_ptr<VariableNode> Var;
    std::unique_ptr<NumberNode> Val;
    std::unique_ptr<ExprNode> Expr;
  };
  enum class UnionType { VAR, VAL, EXPR, NONE } type = UnionType::NONE;
  FactorNode();
  FactorNode(std::unique_ptr<VariableNode> var);
  FactorNode(std::unique_ptr<NumberNode> val);
  FactorNode(std::unique_ptr<ExprNode> expr);
  ~FactorNode();
  bool operator==(const Node& other) const;
};

class AssignNode : public Node {
 public:
  std::unique_ptr<VariableNode> Var;
  std::unique_ptr<ExprNode> Expr;
  AssignNode(std::unique_ptr<VariableNode> var, std::unique_ptr<ExprNode> expr);
  bool operator==(const Node& other) const;
};

class RelExprNode : public Node {
 public:
  std::unique_ptr<RelFactorNode> LHS;
  std::string Op;
  std::unique_ptr<RelFactorNode> RHS;
  RelExprNode(std::unique_ptr<RelFactorNode> lhs, std::string& op,
              std::unique_ptr<RelFactorNode> rhs);
  bool operator==(const Node& other) const;
};

class CondExprNode : public Node {
 public:
  std::unique_ptr<RelExprNode> RelExpr = nullptr;
  std::unique_ptr<CondExprNode> CondLHS = nullptr;
  std::string Op = "";
  std::unique_ptr<CondExprNode> CondRHS = nullptr;

  // rel_expr
  CondExprNode(std::unique_ptr<RelExprNode> relExpr);
  // ! ( cond_expr )
  CondExprNode(std::unique_ptr<CondExprNode> condLHS);
  // ( cond_expr ) && ( cond_expr )
  CondExprNode(std::unique_ptr<CondExprNode> condLHS, std::string& op,
               std::unique_ptr<CondExprNode> condRHS);
  bool operator==(const Node& other) const;
};

class WhileNode : public Node {
 public:
  std::unique_ptr<CondExprNode> CondExpr;
  std::unique_ptr<StmtListNode> StmtList;

  WhileNode(std::unique_ptr<CondExprNode> condExpr,
            std::unique_ptr<StmtListNode> stmtList);
  bool operator==(const Node& other) const;
};

class IfNode : public Node {
 public:
  std::unique_ptr<CondExprNode> CondExpr;
  std::unique_ptr<StmtListNode> StmtListThen;
  std::unique_ptr<StmtListNode> StmtListElse;

  IfNode(std::unique_ptr<CondExprNode> condExpr,
         std::unique_ptr<StmtListNode> stmtListThen,
         std::unique_ptr<StmtListNode> stmtListElse);
  bool operator==(const Node& other) const;
};

class Parser {
 private:
  int current = 0;
  int last_good = 0;
  std::vector<Token*> tokens;

  // Utilities
  bool match(TokenType t);
  bool match(std::string s);
  bool check(TokenType t);
  bool check(std::string s);
  bool expect(TokenType t);
  bool expect(std::string s);
  bool isAtEnd();
  void save_loc() { last_good = current; };
  void reset() { current = last_good; };
  Token* peek();
  Token* previous();
  Token* advance();

  // Parsing
  std::unique_ptr<NumberNode> parseNumber();
  std::unique_ptr<VariableNode> parseVariable();
  std::unique_ptr<ProcedureNode> parseProcedure();
  std::unique_ptr<StmtListNode> parseStmtList();
  std::unique_ptr<Node> parseStatement();
  std::unique_ptr<FactorNode> parseFactor();
  std::unique_ptr<RelFactorNode> parseRelFactor();
  std::unique_ptr<TermPNode> parseTermP();
  std::unique_ptr<TermNode> parseTerm();
  std::unique_ptr<ExprPNode> parseExprP();
  std::unique_ptr<ExprNode> parseExpr();
  std::unique_ptr<RelExprNode> parseRelExpr();
  std::unique_ptr<AssignNode> parseAssign();
  std::unique_ptr<ReadNode> parseRead();
  std::unique_ptr<PrintNode> parsePrint();
  std::unique_ptr<CondExprNode> parseCondExpr();
  std::unique_ptr<WhileNode> parseWhile();
  std::unique_ptr<IfNode> parseIf();

 public:
  Parser(std::vector<Token*> t);
  std::unique_ptr<ProcedureNode> parse();
};
}  // namespace Simple
