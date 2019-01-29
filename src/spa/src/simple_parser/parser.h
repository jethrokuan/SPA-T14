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

class Node {
 public:
  virtual ~Node(){};
  virtual bool operator==(const Node& other) const = 0;
  virtual bool operator!=(const Node& other) const {
    return !operator==(other);
  };
};  // namespace Simple

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
  std::vector<std::unique_ptr<Node>> StmtList;
  ProcedureNode(std::unique_ptr<Node> var,
                std::vector<std::unique_ptr<Node>> stmtList);
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

class Parser {
 private:
  int current = 0;
  std::vector<Token*> tokens;

  // Utilities
  bool match(TokenType type);
  bool check(TokenType type);
  bool expect(TokenType type);
  bool isAtEnd();
  Token* peek();
  Token* previous();
  Token* advance();

  // Parsing
  std::unique_ptr<NumberNode> parseNumber();
  std::unique_ptr<VariableNode> parseVariable();
  std::unique_ptr<ProcedureNode> parseProcedure();
  std::vector<std::unique_ptr<Node>> parseStatementList();
  std::unique_ptr<Node> parseStatement();
  std::unique_ptr<FactorNode> parseFactor();
  std::unique_ptr<TermPNode> parseTermP();
  std::unique_ptr<TermNode> parseTerm();
  std::unique_ptr<ExprPNode> parseExprP();
  std::unique_ptr<ExprNode> parseExpr();
  std::unique_ptr<AssignNode> parseAssign();
  std::unique_ptr<ReadNode> parseRead();
  std::unique_ptr<PrintNode> parsePrint();

 public:
  Parser(std::vector<Token*> t);
  std::unique_ptr<ProcedureNode> parse();
};
}  // namespace Simple
