#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "simple_parser/token.h"

namespace Simple {
// Expr is an interface that all expressions will need to implement.
class Expr {
 public:
  virtual ~Expr(){};
  virtual bool operator==(const Expr& other) const = 0;
  virtual std::string str() = 0;
};

// NumberExprAST - Expression class for numeric literals
class NumberExpr : public Expr {
 public:
  std::string Val;
  NumberExpr(const std::string& Val) : Val(Val){};
  bool operator==(const Expr& other) const {
    auto casted_other = dynamic_cast<const NumberExpr*>(&other);
    return casted_other != 0 && this->Val.compare(casted_other->Val) == 0;
  };
  std::string str() { return this->Val; };
};

class VariableExpr : public Expr {
 public:
  std::string Name;
  VariableExpr(const std::string& Name) : Name(Name){};
  bool operator==(const Expr& other) const {
    auto casted_other = dynamic_cast<const VariableExpr*>(&other);
    return casted_other != 0 && this->Name.compare(casted_other->Name) == 0;
  };

  std::string str() { return this->Name; }
};

class AssignExpr : public Expr {
 public:
  std::unique_ptr<Expr> Var, RHS;
  AssignExpr(std::unique_ptr<Expr> var, std::unique_ptr<Expr> rhs)
      : Var(std::move(var)), RHS(std::move(rhs)) {}
  bool operator==(const Expr& other) const {
    auto casted_other = dynamic_cast<const AssignExpr*>(&other);
    return casted_other != 0 && *this->Var == *casted_other->Var &&
           *this->RHS == *casted_other->RHS;
  };
  std::string str() { return this->Var->str() + " = " + this->RHS->str(); }
};

class ProcedureExpr : public Expr {
  std::unique_ptr<Expr> Var, StmtList;

 public:
  ProcedureExpr(std::unique_ptr<Expr> var, std::unique_ptr<Expr> stmtList)
      : Var(std::move(var)), StmtList(std::move(stmtList)) {}
  bool operator==(const Expr& other) const {
    auto casted_other = dynamic_cast<const ProcedureExpr*>(&other);
    return casted_other != 0 && *this->Var == *casted_other->Var &&
           *this->StmtList == *casted_other->StmtList;
  };

  std::string str() {
    return "procedure " + this->Var->str() + " -> " + this->StmtList->str();
  }
};

class Node {
 public:
  Node* next = nullptr;
  Node(){};
  virtual std::string to_str() { return ""; };
};

class ProcedureNode : public Node {
 public:
  std::string procName;
  ProcedureNode(std::string name) : Node(){};
};

class Parser {
 private:
  int current = 0;
  std::vector<Token*> tokens;
  Node* root = nullptr;

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

  std::unique_ptr<NumberExpr> parseNumberExpr() {
    if (match(TokenType::NUMBER)) {
      std::string num = static_cast<NumberToken*>(previous())->number;
      auto Result = std::make_unique<NumberExpr>(num);
      return std::move(Result);
    } else {
      return nullptr;
    }
  }

  std::unique_ptr<VariableExpr> parseVariableExpr() {
    if (match(TokenType::SYMBOL)) {
      std::string name = static_cast<SymbolToken*>(previous())->name;
      auto Result = std::make_unique<VariableExpr>(name);
      return std::move(Result);
    } else {
      return nullptr;
    }
  };

  std::unique_ptr<ProcedureExpr> parseProcedure() {
    if (!match(TokenType::PROCEDURE)) {
      return nullptr;
    }

    auto Var = parseVariableExpr();

    if (!Var) {
      return nullptr;
    }

    expect(TokenType::L_BRACE);

    auto StmtList = parseStatementList();

    if (!StmtList) {
      return nullptr;
    }

    if (!match(TokenType::R_BRACE)) {
      return nullptr;
    }

    return std::make_unique<ProcedureExpr>(std::move(Var), std::move(StmtList));
  };

  std::unique_ptr<Expr> parseStatementList() { return parseStatement(); };

  std::unique_ptr<Expr> parseStatement() { return parseAssign(); };

  std::unique_ptr<AssignExpr> parseAssign() {
    auto Var = parseVariableExpr();
    if (!Var) {
      return nullptr;
    }
    expect(TokenType::EQUAL);
    auto Num = parseNumberExpr();
    if (!Num) {
      return nullptr;
    }
    expect(TokenType::SEMI);
    return std::make_unique<AssignExpr>(std::move(Var), std::move(Num));
  };

 public:
  Parser(std::vector<Token*> t) { tokens = t; }
  std::unique_ptr<Expr> parse() { return parseProcedure(); }
};
}  // namespace Simple
