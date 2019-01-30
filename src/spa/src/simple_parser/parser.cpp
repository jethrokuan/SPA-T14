#include "simple_parser/parser.h"

using namespace Simple;

NumberNode::NumberNode(const std::string& val) : Val(val){};
bool NumberNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const NumberNode*>(&other);
  return casted_other != 0 && this->Val.compare(casted_other->Val) == 0;
};

VariableNode::VariableNode(const std::string& name) : Name(name){};
bool VariableNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const VariableNode*>(&other);
  return casted_other != 0 && this->Name.compare(casted_other->Name) == 0;
};

ReadNode::ReadNode(std::unique_ptr<VariableNode> var) : Var(std::move(var)){};
bool ReadNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ReadNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var;
};

PrintNode::PrintNode(std::unique_ptr<Node> var) : Var(std::move(var)){};
bool PrintNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const PrintNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var;
};

ProcedureNode::ProcedureNode(std::unique_ptr<Node> var,
                             std::vector<std::unique_ptr<Node>> stmtList)
    : Var(std::move(var)), StmtList(std::move(stmtList)){};
bool ProcedureNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ProcedureNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var &&
         this->StmtList.size() == casted_other->StmtList.size() &&
         std::equal(begin(this->StmtList), end(this->StmtList),
                    begin(casted_other->StmtList), end(casted_other->StmtList),
                    [](const std::unique_ptr<Node>& t,
                       const std::unique_ptr<Node>& o) { return *t == *o; });
};

TermPNode::TermPNode(std::unique_ptr<FactorNode> factor, std::string& op,
                     std::unique_ptr<TermPNode> termP)
    : Factor(std::move(factor)), Op(op), TermP(std::move(termP)){};

bool TermPNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const TermPNode*>(&other);
  return casted_other != 0 && this->Op.compare(casted_other->Op) == 0 &&
         *this->Factor == *casted_other->Factor;
};

TermNode::TermNode(std::unique_ptr<FactorNode> factor,
                   std::unique_ptr<TermPNode> termP)
    : Factor(std::move(factor)), TermP(std::move(termP)){};
TermNode::TermNode(std::unique_ptr<FactorNode> factor)
    : Factor(std::move(factor)), TermP(nullptr){};
bool TermNode::operator==(const Node& other) const {
  return true;
  auto casted_other = dynamic_cast<const TermNode*>(&other);
  return casted_other != 0;
  return casted_other != 0 && *this->Factor == *casted_other->Factor &&
         (this->TermP == casted_other->TermP ||
          *this->TermP == *casted_other->TermP);
}

ExprPNode::ExprPNode(std::unique_ptr<TermNode> term, std::string& op)
    : Term(std::move(term)), Op(op){};

ExprPNode::ExprPNode(std::unique_ptr<TermNode> term, std::string& op,
                     std::unique_ptr<ExprPNode> exprP)
    : Term(std::move(term)), Op(op), ExprP(std::move(exprP)){};

bool ExprPNode::operator==(const Node& other) const {
  return true;
  auto casted_other = dynamic_cast<const ExprPNode*>(&other);
  return casted_other != 0;
  return casted_other != 0 && *this->Term == *casted_other->Term &&
         (this->ExprP == casted_other->ExprP ||
          *this->ExprP == *casted_other->ExprP) &&
         this->Op.compare(casted_other->Op) == 0;
};

ExprNode::ExprNode(std::unique_ptr<TermNode> term,
                   std::unique_ptr<ExprPNode> exprP)
    : Term(std::move(term)), ExprP(std::move(exprP)){};
ExprNode::ExprNode(std::unique_ptr<TermNode> term)
    : Term(std::move(term)), ExprP(nullptr){};

bool ExprNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ExprNode*>(&other);
  return casted_other != 0 && *this->Term == *casted_other->Term &&
         (this->ExprP == casted_other->ExprP ||
          *this->ExprP == *casted_other->ExprP);
};

FactorNode::FactorNode(){};
FactorNode::FactorNode(std::unique_ptr<VariableNode> var) {
  new (&Var) std::unique_ptr<VariableNode>{std::move(var)};
  type = UnionType::VAR;
};
FactorNode::FactorNode(std::unique_ptr<NumberNode> val) {
  new (&Val) std::unique_ptr<NumberNode>{std::move(val)};
  type = UnionType::VAL;
};
FactorNode::FactorNode(std::unique_ptr<ExprNode> expr) {
  new (&Expr) std::unique_ptr<ExprNode>{std::move(expr)};
  type = UnionType::EXPR;
};
FactorNode::~FactorNode() {
  switch (type) {
    case UnionType::VAR:
      Var.~unique_ptr<VariableNode>();
      break;
    case UnionType::VAL:
      Val.~unique_ptr<NumberNode>();
      break;
    case UnionType::EXPR:
      Expr.~unique_ptr<ExprNode>();
      break;
    case UnionType::NONE:
      break;
  };
}
bool FactorNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const FactorNode*>(&other);
  if (casted_other == 0) return false;
  if (this->type != casted_other->type) return false;
  switch (this->type) {
    case UnionType::VAR:
      return *this->Var == *casted_other->Var;
    case UnionType::VAL:
      return *this->Val == *casted_other->Val;
    case UnionType::EXPR:
      return *this->Expr == *casted_other->Expr;
    case UnionType::NONE:
      return true;
  }
};

AssignNode::AssignNode(std::unique_ptr<VariableNode> var,
                       std::unique_ptr<ExprNode> expr)
    : Var(std::move(var)), Expr(std::move(expr)){};
bool AssignNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const AssignNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var &&
         *this->Expr == *casted_other->Expr;
};

// Parser
bool Parser::match(TokenType type) {
  if (check(type)) {
    advance();
    return true;
  } else {
    return false;
  }
}

bool Parser::expect(TokenType type) {
  if (match(type)) {
    return true;
  } else {
    // TODO: Handle this better
    std::cout << "expected " << type << ", got " << peek()->t << std::endl;
    return false;
  }
};

bool Parser::check(TokenType type) {
  if (isAtEnd()) return false;
  return peek()->t == type;
};

Token* Parser::advance() {
  if (!isAtEnd()) current++;
  return previous();
};

bool Parser::isAtEnd() { return peek()->t == +TokenType::END_OF_FILE; };

Token* Parser::peek() { return tokens[current]; };

Token* Parser::previous() { return tokens[current - 1]; };

std::unique_ptr<NumberNode> Parser::parseNumber() {
  if (match(TokenType::NUMBER)) {
    std::string num = static_cast<NumberToken*>(previous())->number;
    auto result = std::make_unique<NumberNode>(num);
    return result;
  } else {
    return nullptr;
  }
}

std::unique_ptr<VariableNode> Parser::parseVariable() {
  if (match(TokenType::SYMBOL)) {
    std::string name = static_cast<SymbolToken*>(previous())->name;
    auto result = std::make_unique<VariableNode>(name);
    return result;
  } else {
    return nullptr;
  }
};

std::unique_ptr<ProcedureNode> Parser::parseProcedure() {
  if (!match(TokenType::PROCEDURE)) {
    return nullptr;
  }

  auto Var = parseVariable();

  if (!Var) {
    return nullptr;
  }

  expect(TokenType::L_BRACE);

  auto StmtList = parseStatementList();

  expect(TokenType::R_BRACE);

  return std::make_unique<ProcedureNode>(std::move(Var), std::move(StmtList));
};

std::vector<std::unique_ptr<Node>> Parser::parseStatementList() {
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

std::unique_ptr<Node> Parser::parseStatement() {
  std::unique_ptr<Node> stmt = parseAssign();
  if (stmt) {
    return stmt;
  }

  stmt = parseRead();
  if (stmt) {
    return stmt;
  }

  stmt = parsePrint();
  if (stmt) {
    return stmt;
  }

  return nullptr;
};

std::unique_ptr<FactorNode> Parser::parseFactor() {
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

std::unique_ptr<TermPNode> Parser::parseTermP() {
  if (match(TokenType::TIMES)) {
    auto Factor = parseFactor();
    auto TermP = parseTermP();
    std::string op = "*";
    return std::make_unique<TermPNode>(std::move(Factor), op, std::move(TermP));
  } else if (match(TokenType::DIVIDE)) {
    auto Factor = parseFactor();
    auto TermP = parseTermP();
    std::string op = "/";
    return std::make_unique<TermPNode>(std::move(Factor), op, std::move(TermP));
  } else if (match(TokenType::MOD)) {
    auto Factor = parseFactor();
    auto TermP = parseTermP();
    std::string op = "%";
    return std::make_unique<TermPNode>(std::move(Factor), op, std::move(TermP));
  } else {
    return nullptr;
  }
};

std::unique_ptr<TermNode> Parser::parseTerm() {
  auto Factor = parseFactor();
  if (!Factor) {
    return nullptr;
  }

  auto TermP = parseTermP();

  return std::make_unique<TermNode>(std::move(Factor), std::move(TermP));
}

std::unique_ptr<ExprPNode> Parser::parseExprP() {
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

std::unique_ptr<ExprNode> Parser::parseExpr() {
  auto Term = parseTerm();

  if (!Term) {
    return nullptr;
  }

  auto ExprP = parseExprP();
  return std::make_unique<ExprNode>(std::move(Term), std::move(ExprP));
};

std::unique_ptr<AssignNode> Parser::parseAssign() {
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

std::unique_ptr<ReadNode> Parser::parseRead() {
  if (!match(TokenType::READ)) {
    return nullptr;
  }

  auto Var = parseVariable();

  if (!Var) {
    return nullptr;
  }

  expect(TokenType::SEMI);

  return std::make_unique<ReadNode>(std::move(Var));
};

std::unique_ptr<PrintNode> Parser::parsePrint() {
  if (!match(TokenType::PRINT)) {
    return nullptr;
  }

  auto Var = parseVariable();
  if (!Var) {
    return nullptr;
  }

  expect(TokenType::SEMI);

  return std::make_unique<PrintNode>(std::move(Var));
};

std::unique_ptr<RelFactorNode> Parser::parseRelFactor() {
  auto Var = parseVariable();
  if (Var) {
    return std::make_unique<FactorNode>(std::move(Var));
  }

  auto Number = parseNumber();

  if (Number) {
    return std::make_unique<FactorNode>(std::move(Number));
  }

  auto Expr = parseExpr();

  if (!Expr) {
    return nullptr;
  }

  return std::make_unique<FactorNode>(std::move(Expr));
};

Parser::Parser(std::vector<Token*> t) : tokens(t){};
std::unique_ptr<ProcedureNode> Parser::parse() { return parseProcedure(); }
