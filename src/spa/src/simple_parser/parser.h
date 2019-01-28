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
};

class VariableExpr : public Expr {
 public:
  std::string Name;
  VariableExpr(const std::string& Name) : Name(Name){};
  bool operator==(const Expr& other) const {
    auto casted_other = dynamic_cast<const VariableExpr*>(&other);
    return casted_other != 0 && this->Name.compare(casted_other->Name) == 0;
  };
};

class ReadExpr : public Expr {
 public:
  std::unique_ptr<Expr> Var;
  ReadExpr(std::unique_ptr<Expr> var) : Var(std::move(var)){};
  bool operator==(const Expr& other) const {
    auto casted_other = dynamic_cast<const ReadExpr*>(&other);
    return casted_other != 0 && *this->Var == *casted_other->Var;
  };
};

class PrintExpr : public Expr {
 public:
  std::unique_ptr<Expr> Var;
  PrintExpr(std::unique_ptr<Expr> var) : Var(std::move(var)){};
  bool operator==(const Expr& other) const {
    auto casted_other = dynamic_cast<const PrintExpr*>(&other);
    return casted_other != 0 && *this->Var == *casted_other->Var;
  };
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
};

class ProcedureExpr : public Expr {
  std::unique_ptr<Expr> Var;
  std::vector<std::unique_ptr<Expr>> StmtList;

 public:
  ProcedureExpr(std::unique_ptr<Expr> var,
                std::vector<std::unique_ptr<Expr>> stmtList)
      : Var(std::move(var)), StmtList(std::move(stmtList)) {}
  bool operator==(const Expr& other) const {
    auto casted_other = dynamic_cast<const ProcedureExpr*>(&other);
    return casted_other != 0 && *this->Var == *casted_other->Var &&
           this->StmtList.size() == casted_other->StmtList.size() &&
           std::equal(begin(this->StmtList), end(this->StmtList),
                      begin(casted_other->StmtList),
                      end(casted_other->StmtList),
                      [](const std::unique_ptr<Expr>& t,
                         const std::unique_ptr<Expr>& o) { return *t == *o; });
  };
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

  std::unique_ptr<NumberExpr> parseNumberExpr() {
    if (match(TokenType::NUMBER)) {
      std::string num = static_cast<NumberToken*>(previous())->number;
      auto result = std::make_unique<NumberExpr>(num);
      return result;
    } else {
      return nullptr;
    }
  }

  std::unique_ptr<VariableExpr> parseVariableExpr() {
    if (match(TokenType::SYMBOL)) {
      std::string name = static_cast<SymbolToken*>(previous())->name;
      auto result = std::make_unique<VariableExpr>(name);
      return result;
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

    if (!match(TokenType::R_BRACE)) {
      return nullptr;
    }

    return std::make_unique<ProcedureExpr>(std::move(Var), std::move(StmtList));
  };

  std::vector<std::unique_ptr<Expr>> parseStatementList() {
    std::vector<std::unique_ptr<Expr>> stmts;
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

  std::unique_ptr<Expr> parseStatement() {
    std::unique_ptr<Expr> stmt = parseAssign();
    if (stmt) return stmt;

    stmt = parseRead();
    if (stmt) return stmt;

    stmt = parsePrint();
    if (stmt) return stmt;

    return nullptr;
  };

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

  std::unique_ptr<ReadExpr> parseRead() {
    expect(TokenType::READ);

    auto Var = parseVariableExpr();
    if (!Var) {
      return nullptr;
    }

    expect(TokenType::SEMI);

    return std::make_unique<ReadExpr>(std::move(Var));
  };

  std::unique_ptr<PrintExpr> parsePrint() {
    expect(TokenType::PRINT);

    auto Var = parseVariableExpr();
    if (!Var) {
      return nullptr;
    }

    expect(TokenType::SEMI);

    return std::make_unique<PrintExpr>(std::move(Var));
  };

 public:
  Parser(std::vector<Token*> t) { tokens = t; }
  std::unique_ptr<Expr> parse() { return parseProcedure(); }
};
}  // namespace Simple
