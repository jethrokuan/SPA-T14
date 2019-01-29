#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "simple_parser/token.h"

namespace Simple {
// Node is an interface that all expressions will need to implement.
class Node {
 public:
  virtual ~Node(){};
  virtual bool operator==(const Node& other) const = 0;
};

// NumberNodeAST - Nodeession class for numeric literals
class NumberNode : public Node {
 public:
  std::string Val;
  NumberNode(const std::string& Val) : Val(Val){};
  bool operator==(const Node& other) const {
    auto casted_other = dynamic_cast<const NumberNode*>(&other);
    return casted_other != 0 && this->Val.compare(casted_other->Val) == 0;
  };
};

class VariableNode : public Node {
 public:
  std::string Name;
  VariableNode(const std::string& Name) : Name(Name){};
  bool operator==(const Node& other) const {
    auto casted_other = dynamic_cast<const VariableNode*>(&other);
    return casted_other != 0 && this->Name.compare(casted_other->Name) == 0;
  };
};

class ReadNode : public Node {
 public:
  std::unique_ptr<Node> Var;
  ReadNode(std::unique_ptr<Node> var) : Var(std::move(var)){};
  bool operator==(const Node& other) const {
    auto casted_other = dynamic_cast<const ReadNode*>(&other);
    return casted_other != 0 && *this->Var == *casted_other->Var;
  };
};

class PrintNode : public Node {
 public:
  std::unique_ptr<Node> Var;
  PrintNode(std::unique_ptr<Node> var) : Var(std::move(var)){};
  bool operator==(const Node& other) const {
    auto casted_other = dynamic_cast<const PrintNode*>(&other);
    return casted_other != 0 && *this->Var == *casted_other->Var;
  };
};

class AssignNode : public Node {
 public:
  std::unique_ptr<Node> Var, RHS;
  AssignNode(std::unique_ptr<Node> var, std::unique_ptr<Node> rhs)
      : Var(std::move(var)), RHS(std::move(rhs)) {}
  bool operator==(const Node& other) const {
    auto casted_other = dynamic_cast<const AssignNode*>(&other);
    return casted_other != 0 && *this->Var == *casted_other->Var &&
           *this->RHS == *casted_other->RHS;
  };
};

class ProcedureNode : public Node {
  std::unique_ptr<Node> Var;
  std::vector<std::unique_ptr<Node>> StmtList;

 public:
  ProcedureNode(std::unique_ptr<Node> var,
                std::vector<std::unique_ptr<Node>> stmtList)
      : Var(std::move(var)), StmtList(std::move(stmtList)) {}
  bool operator==(const Node& other) const {
    auto casted_other = dynamic_cast<const ProcedureNode*>(&other);
    return casted_other != 0 && *this->Var == *casted_other->Var &&
           this->StmtList.size() == casted_other->StmtList.size() &&
           std::equal(begin(this->StmtList), end(this->StmtList),
                      begin(casted_other->StmtList),
                      end(casted_other->StmtList),
                      [](const std::unique_ptr<Node>& t,
                         const std::unique_ptr<Node>& o) { return *t == *o; });
  };
};

class FactorNode : public Node {
 public:
  union {
    std::unique_ptr<VariableNode> Var;
    std::unique_ptr<NumberNode> Val;
    std::unique_ptr<Node> Expr;
  };
  enum class UnionType { VAR, VAL, EXPR, NONE } type = UnionType::NONE;
  FactorNode(){};
  FactorNode(std::unique_ptr<VariableNode> var) {
    new (&Var) std::unique_ptr<VariableNode>{std::move(var)};
    type = UnionType::VAR;
  };
  FactorNode(std::unique_ptr<NumberNode> val) {
    new (&Val) std::unique_ptr<NumberNode>{std::move(val)};
    type = UnionType::VAL;
  };
  FactorNode(std::unique_ptr<Node> expr) {
    new (&Expr) std::unique_ptr<Node>{std::move(expr)};
    type = UnionType::EXPR;
  };
  ~FactorNode() {
    switch (type) {
      case UnionType::VAR:
        Var.~unique_ptr<VariableNode>();
        break;
      case UnionType::VAL:
        Val.~unique_ptr<NumberNode>();
        break;
      case UnionType::EXPR:
        Expr.~unique_ptr<Node>();
        break;
      case UnionType::NONE:
        break;
    };
  }
};

class TermPNode : public Node {
 public:
  std::unique_ptr<FactorNode> Factor;
  std::string Op;
  std::unique_ptr<TermPNode> TermP;

  TermPNode(std::unique_ptr<FactorNode> factor, std::string op, std::unique_ptr<TermPNode> termP)
    : Factor(std::move(factor)), Op(std::move(op)), TermP(std::move(termP)){};
};

class TermNode : public Node {
 public:
  std::unique_ptr<FactorNode> Factor;
  std::unique_ptr<TermPNode> TermP;

  TermNode(std::unique_ptr<FactorNode> factor, std::unique_ptr<TermPNode> termP)
      : Factor(std::move(factor)), TermP(std::move(termP)){};

  TermNode(std::unique_ptr<FactorNode> factor)
      : Factor(std::move(factor)), TermP(nullptr){};
};

class ExprPNode : public Node {
 public:
  std::unique_ptr<TermNode> Term;
  std::string Op;
  std::unique_ptr<ExprPNode> ExprP;

  ExprPNode(std::unique_ptr<TermNode> term, std::string& op)
      : Term(std::move(term)), Op(std::move(op)){};

  ExprPNode(std::unique_ptr<TermNode> term, std::string& op, std::unique_ptr<ExprPNode> exprP)
    : Term(std::move(term)), Op(std::move(op)), ExprP(std::move(exprP)){};
};

  class ExprNode : public Node {
 public:
  std::unique_ptr<TermNode> Term;
  std::unique_ptr<ExprPNode> ExprP;

  ExprNode(std::unique_ptr<TermNode> term, std::unique_ptr<ExprPNode> exprP)
      : Term(std::move(term)), ExprP(std::move(exprP)){};
  ExprNode(std::unique_ptr<TermNode> term)
      : Term(std::move(term)), ExprP(nullptr){};
};

class Parser {
 private:
  int current = 0;
  std::vector<Token*> tokens;

  bool match(TokenType type) {
    if (check(type)) {
      advance();
      return true;
    } else {
      return false;
    }
  };

  bool expect(TokenType type) {
    if (match(type)) {
      return true;
    } else {
      // TODO: Handle this better
      std::cout << "expected a different symbol." << std::endl;
      return false;
    }
  };

  bool check(TokenType type) {
    if (isAtEnd()) return false;
    return peek()->t == type;
  };

  Token* advance() {
    if (!isAtEnd()) current++;
    return previous();
  };

  bool isAtEnd() { return peek()->t == TokenType::END_OF_FILE; };

  Token* peek() { return tokens[current]; };

  Token* previous() { return tokens[current - 1]; };

  std::unique_ptr<NumberNode> parseNumberNode() {
    if (match(TokenType::NUMBER)) {
      std::string num = static_cast<NumberToken*>(previous())->number;
      auto result = std::make_unique<NumberNode>(num);
      return result;
    } else {
      return nullptr;
    }
  }

  std::unique_ptr<VariableNode> parseVariableNode() {
    if (match(TokenType::SYMBOL)) {
      std::string name = static_cast<SymbolToken*>(previous())->name;
      auto result = std::make_unique<VariableNode>(name);
      return result;
    } else {
      return nullptr;
    }
  };

  std::unique_ptr<ProcedureNode> parseProcedure() {
    if (!match(TokenType::PROCEDURE)) {
      return nullptr;
    }

    auto Var = parseVariableNode();

    if (!Var) {
      return nullptr;
    }

    expect(TokenType::L_BRACE);

    auto StmtList = parseStatementList();

    if (!match(TokenType::R_BRACE)) {
      return nullptr;
    }

    return std::make_unique<ProcedureNode>(std::move(Var), std::move(StmtList));
  };

  std::vector<std::unique_ptr<Node>> parseStatementList() {
    std::vector<std::unique_ptr<Node>> stmts;
    while (true) {
      auto stmt = parseStatement();
      if (stmt) {
        stmts.push_back(std::move(stmt));
      } else {
        break;
      }
    }
    return stmts;
  };

  std::unique_ptr<Node> parseStatement() {
    std::unique_ptr<Node> stmt = parseAssign();
    if (stmt) return stmt;

    stmt = parseRead();
    if (stmt) return stmt;

    stmt = parsePrint();
    if (stmt) return stmt;

    return nullptr;
  };

  std::unique_ptr<AssignNode> parseAssign() {
    auto Var = parseVariableNode();
    if (!Var) {
      return nullptr;
    }
    expect(TokenType::EQUAL);
    auto Num = parseNumberNode();
    if (!Num) {
      return nullptr;
    }
    expect(TokenType::SEMI);
    return std::make_unique<AssignNode>(std::move(Var), std::move(Num));
  };

  std::unique_ptr<ReadNode> parseRead() {
    expect(TokenType::READ);

    auto Var = parseVariableNode();
    if (!Var) {
      return nullptr;
    }

    expect(TokenType::SEMI);

    return std::make_unique<ReadNode>(std::move(Var));
  };

  std::unique_ptr<PrintNode> parsePrint() {
    expect(TokenType::PRINT);

    auto Var = parseVariableNode();
    if (!Var) {
      return nullptr;
    }

    expect(TokenType::SEMI);

    return std::make_unique<PrintNode>(std::move(Var));
  };

 public:
  Parser(std::vector<Token*> t) { tokens = t; }
  std::unique_ptr<Node> parse() { return parseProcedure(); }
};
}  // namespace Simple
