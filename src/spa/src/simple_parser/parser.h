#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "simple_parser/token.h"

namespace Simple {
class Node;
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
  std::unique_ptr<ExprNode> RHS;
  AssignNode(std::unique_ptr<VariableNode> var, std::unique_ptr<ExprNode> rhs);
  bool operator==(const Node& other) const;
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

  std::unique_ptr<NumberNode> parseNumber() {
    if (match(TokenType::NUMBER)) {
      std::string num = static_cast<NumberToken*>(previous())->number;
      auto result = std::make_unique<NumberNode>(num);
      return result;
    } else {
      return nullptr;
    }
  }

  std::unique_ptr<VariableNode> parseVariable() {
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

    auto Var = parseVariable();

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

  std::unique_ptr<FactorNode> parseFactor() {
    auto Var = parseVariable();
    if (Var) {
      return std::make_unique<FactorNode>(std::move(Var));
    }

    auto Number = parseNumber();

    if (Number) {
      return std::make_unique<FactorNode>(std::move(Number));
    }

    expect(TokenType::L_PAREN);
    auto Expr = parseExpr();
    expect(TokenType::R_PAREN);
    return std::make_unique<FactorNode>(std::move(Expr));
  };

  std::unique_ptr<TermPNode> parseTermP() {
    if (!match(TokenType::TIMES)) {
      auto Factor = parseFactor();
      auto TermP = parseTermP();
      std::string op = "*";
      return std::make_unique<TermPNode>(std::move(Factor), op,
                                         std::move(TermP));
    } else if (!match(TokenType::DIVIDE)) {
      auto Factor = parseFactor();
      auto TermP = parseTermP();
      std::string op = "/";
      return std::make_unique<TermPNode>(std::move(Factor), op,
                                         std::move(TermP));
    } else if (!match(TokenType::MOD)) {
      auto Factor = parseFactor();
      auto TermP = parseTermP();
      std::string op = "%";
      return std::make_unique<TermPNode>(std::move(Factor), op,
                                         std::move(TermP));
    } else {
      return nullptr;
    }
  };

  std::unique_ptr<TermNode> parseTerm() {
    auto Factor = parseFactor();
    if (!Factor) {
      return nullptr;
    }

    auto TermP = parseTermP();

    if (!TermP) {
      return nullptr;
    }

    return std::make_unique<TermNode>(std::move(Factor), std::move(TermP));
  }

  std::unique_ptr<ExprPNode> parseExprP() {
    if (match(TokenType::MINUS)) {
      auto Term = parseTerm();
      auto ExprP = parseExprP();
      std::string op = "-";
      return std::make_unique<ExprPNode>(std::move(Term), op, std::move(ExprP));
    } else if (match(TokenType::PLUS)) {
      auto Term = parseTerm();
      auto ExprP = parseExprP();
      std::string op = "+";
      return std::make_unique<ExprPNode>(std::move(Term), op, std::move(ExprP));
    } else {
      return nullptr;
    }
  };

  std::unique_ptr<ExprNode> parseExpr() {
    auto Term = parseTerm();

    if (!Term) {
      return nullptr;
    }

    auto ExprP = parseExprP();
    return std::make_unique<ExprNode>(std::move(Term), std::move(ExprP));
  };

  std::unique_ptr<AssignNode> parseAssign() {
    auto Var = parseVariable();
    if (!Var) {
      return nullptr;
    }
    expect(TokenType::EQUAL);

    auto Expr = parseExpr();

    if (!Expr) {
      return nullptr;
    }

    expect(TokenType::SEMI);
    return std::make_unique<AssignNode>(std::move(Var), std::move(Expr));
  };

  std::unique_ptr<ReadNode> parseRead() {
    expect(TokenType::READ);

    auto Var = parseVariable();
    if (!Var) {
      return nullptr;
    }

    expect(TokenType::SEMI);

    return std::make_unique<ReadNode>(std::move(Var));
  };

  std::unique_ptr<PrintNode> parsePrint() {
    expect(TokenType::PRINT);

    auto Var = parseVariable();
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
