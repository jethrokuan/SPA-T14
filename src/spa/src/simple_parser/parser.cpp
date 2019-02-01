#include "simple_parser/parser.h"

#include <unordered_set>

using namespace Simple;

StmtListNode::StmtListNode(std::vector<std::unique_ptr<Node>> stmtList)
    : StmtList(std::move(stmtList)){};
bool StmtListNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const StmtListNode*>(&other);
  return casted_other != 0 &&
         this->StmtList.size() == casted_other->StmtList.size() &&
         std::equal(begin(this->StmtList), end(this->StmtList),
                    begin(casted_other->StmtList), end(casted_other->StmtList),
                    [](const std::unique_ptr<Node>& t,
                       const std::unique_ptr<Node>& o) { return *t == *o; });
};

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
                             std::unique_ptr<StmtListNode> stmtList)
    : Var(std::move(var)), StmtList(std::move(stmtList)){};
bool ProcedureNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const ProcedureNode*>(&other);
  return casted_other != 0 && *this->Var == *casted_other->Var &&
         *this->StmtList == *casted_other->StmtList;
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
  auto casted_other = dynamic_cast<const ExprPNode*>(&other);
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

RelExprNode::RelExprNode(std::unique_ptr<RelFactorNode> lhs, std::string& op,
                         std::unique_ptr<RelFactorNode> rhs)
    : LHS(std::move(lhs)), Op(op), RHS(std::move(rhs)){};
bool RelExprNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const RelExprNode*>(&other);
  return casted_other != 0 && *this->LHS == *casted_other->LHS &&
         *this->RHS == *casted_other->RHS &&
         this->Op.compare(casted_other->Op) == 0;
};

CondExprNode::CondExprNode(std::unique_ptr<RelExprNode> relExpr)
    : RelExpr(std::move(relExpr)){};
CondExprNode::CondExprNode(std::unique_ptr<CondExprNode> condLHS)
    : CondLHS(std::move(condLHS)), Op("!"){};
CondExprNode::CondExprNode(std::unique_ptr<CondExprNode> condLHS,
                           std::string& op,
                           std::unique_ptr<CondExprNode> condRHS)
    : CondLHS(std::move(condLHS)), Op(op), CondRHS(std::move(condRHS)){};
bool CondExprNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const CondExprNode*>(&other);
  return casted_other != 0 &&
         (this->RelExpr == casted_other->RelExpr ||
          *this->RelExpr == *casted_other->RelExpr) &&
         (this->CondLHS == casted_other->CondLHS ||
          *this->CondLHS == *casted_other->CondLHS) &&
         (this->CondRHS == casted_other->CondRHS ||
          *this->CondRHS == *casted_other->CondRHS) &&
         (this->Op.compare(casted_other->Op) == 0);
};

WhileNode::WhileNode(std::unique_ptr<CondExprNode> condExpr,
                     std::unique_ptr<StmtListNode> stmtList)
    : CondExpr(std::move(condExpr)), StmtList(std::move(stmtList)){};
bool WhileNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const WhileNode*>(&other);
  return casted_other != 0 && *this->CondExpr == *casted_other->CondExpr &&
         *this->StmtList == *casted_other->StmtList;
};

IfNode::IfNode(std::unique_ptr<CondExprNode> condExpr,
               std::unique_ptr<StmtListNode> stmtListThen,
               std::unique_ptr<StmtListNode> stmtListElse)
    : CondExpr(std::move(condExpr)),
      StmtListThen(std::move(stmtListThen)),
      StmtListElse(std::move(stmtListElse)){};
bool IfNode::operator==(const Node& other) const {
  auto casted_other = dynamic_cast<const IfNode*>(&other);
  return casted_other != 0 && *this->CondExpr == *casted_other->CondExpr &&
         *this->StmtListThen == *casted_other->StmtListThen &&
         *this->StmtListElse == *casted_other->StmtListElse;
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

bool Parser::match(std::string s) {
  if (check(s)) {
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
    return false;
  }
};

bool Parser::expect(std::string s) {
  if (match(s)) {
    return true;
  } else {
    // TODO: Handle this better
    std::cout << "expected " << s << ", got " << peek()->Val << std::endl;
    return false;
  }
};

bool Parser::check(TokenType type) {
  if (isAtEnd()) return false;
  return peek()->T == type;
};

bool Parser::check(std::string s) {
  if (isAtEnd()) return false;
  return peek()->Val.compare(s) == 0;
};

Token* Parser::advance() {
  if (!isAtEnd()) current++;
  return previous();
};

bool Parser::isAtEnd() { return peek()->T == TokenType::END_OF_FILE; };

Token* Parser::peek() { return tokens[current]; };

Token* Parser::previous() { return tokens[current - 1]; };

std::unique_ptr<NumberNode> Parser::parseNumber() {
  save_loc();
  if (match(TokenType::NUMBER)) {
    std::string num = static_cast<NumberToken*>(previous())->Val;
    auto result = std::make_unique<NumberNode>(num);
    return result;
  } else {
    reset();
    return nullptr;
  }
}

std::unique_ptr<VariableNode> Parser::parseVariable() {
  save_loc();
  if (match(TokenType::SYMBOL)) {
    std::string name = static_cast<SymbolToken*>(previous())->Val;
    auto result = std::make_unique<VariableNode>(name);
    return result;
  } else {
    reset();
    return nullptr;
  }
};

std::unique_ptr<ProcedureNode> Parser::parseProcedure() {
  save_loc();
  if (!match("procedure")) {
    reset();
    return nullptr;
  }

  auto Var = parseVariable();

  if (!Var) {
    reset();
    return nullptr;
  }

  expect("{");

  auto StmtList = parseStmtList();

  expect("}");

  return std::make_unique<ProcedureNode>(std::move(Var), std::move(StmtList));
};

std::unique_ptr<StmtListNode> Parser::parseStmtList() {
  save_loc();
  std::vector<std::unique_ptr<Node>> stmts;
  while (true) {
    auto stmt = parseStatement();
    if (stmt) {
      stmts.push_back(std::move(stmt));
    } else {
      break;
    }
  }

  // TODO: Handle case where statement list is empty

  return std::make_unique<StmtListNode>(std::move(stmts));
};

std::unique_ptr<Node> Parser::parseStatement() {
  save_loc();
  std::unique_ptr<Node> stmt = parseRead();
  if (stmt) {
    return stmt;
  }

  stmt = parsePrint();
  if (stmt) {
    return stmt;
  }

  stmt = parseAssign();
  if (stmt) {
    return stmt;
  }

  stmt = parseWhile();
  if (stmt) {
    return stmt;
  }

  stmt = parseIf();
  if (stmt) {
    return stmt;
  }

  reset();
  return nullptr;
};

std::unique_ptr<FactorNode> Parser::parseFactor() {
  save_loc();
  auto Var = parseVariable();
  if (Var) {
    return std::make_unique<FactorNode>(std::move(Var));
  }

  auto Number = parseNumber();

  if (Number) {
    return std::make_unique<FactorNode>(std::move(Number));
  }

  expect("(");
  auto Expr = parseExpr();
  expect(")");
  return std::make_unique<FactorNode>(std::move(Expr));
};

std::unique_ptr<TermPNode> Parser::parseTermP() {
  save_loc();
  std::unordered_set<std::string> valid_ops({"*", "/", "%"});

  if (valid_ops.find(peek()->Val) != valid_ops.end()) {
    auto Factor = parseFactor();
    auto TermP = parseTermP();
    return std::make_unique<TermPNode>(std::move(Factor), advance()->Val,
                                       std::move(TermP));
  } else {
    reset();
    return nullptr;
  }
};

std::unique_ptr<TermNode> Parser::parseTerm() {
  save_loc();
  auto Factor = parseFactor();
  if (!Factor) {
    reset();
    return nullptr;
  }

  auto TermP = parseTermP();

  return std::make_unique<TermNode>(std::move(Factor), std::move(TermP));
}

std::unique_ptr<ExprPNode> Parser::parseExprP() {
  save_loc();
  std::unordered_set<std::string> valid_ops({"+", "-"});
  if (valid_ops.find(peek()->Val) != valid_ops.end()) {
    auto Term = parseTerm();
    auto ExprP = parseExprP();
    return std::make_unique<ExprPNode>(std::move(Term), advance()->Val,
                                       std::move(ExprP));
  } else {
    reset();
    return nullptr;
  }
};

std::unique_ptr<ExprNode> Parser::parseExpr() {
  save_loc();
  auto Term = parseTerm();

  if (!Term) {
    reset();
    return nullptr;
  }

  auto ExprP = parseExprP();
  return std::make_unique<ExprNode>(std::move(Term), std::move(ExprP));
};

std::unique_ptr<AssignNode> Parser::parseAssign() {
  save_loc();
  auto Var = parseVariable();

  if (!Var) {
    reset();
    return nullptr;
  }

  if (!match("=")) {
    reset();
    return nullptr;
  }

  auto Expr = parseExpr();

  if (!Expr) {
    reset();
    return nullptr;
  }

  expect(";");

  return std::make_unique<AssignNode>(std::move(Var), std::move(Expr));
};

std::unique_ptr<ReadNode> Parser::parseRead() {
  save_loc();
  if (!match("read")) {
    reset();
    return nullptr;
  }

  auto Var = parseVariable();

  if (!Var) {
    reset();
    return nullptr;
  }

  expect(";");

  return std::make_unique<ReadNode>(std::move(Var));
};

std::unique_ptr<PrintNode> Parser::parsePrint() {
  save_loc();
  if (!match("print")) {
    reset();
    return nullptr;
  }

  auto Var = parseVariable();
  if (!Var) {
    reset();
    return nullptr;
  }

  expect(";");

  return std::make_unique<PrintNode>(std::move(Var));
};

std::unique_ptr<RelFactorNode> Parser::parseRelFactor() {
  save_loc();
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
    reset();
    return nullptr;
  }

  return std::make_unique<FactorNode>(std::move(Expr));
};

// rel_expr: rel_factor > rel_factor
// | rel_factor >= rel_factor
// | rel_factor < rel_factor
// | rel_factor <= rel_factor
// | rel_factor == rel_factor
// | rel_factor != rel_factor
std::unique_ptr<RelExprNode> Parser::parseRelExpr() {
  save_loc();
  auto lhs = parseRelFactor();

  if (!lhs) {
    reset();
    return nullptr;
  }
  std::unordered_set<std::string> valid_ops({">", ">=", "<", "<=", "==", "!="});
  auto op = peek()->Val;
  if (valid_ops.find(op) == valid_ops.end()) {
    std::cout << "Expected an op, got " << op << std::endl;
    reset();
    return nullptr;
  }

  advance();

  auto rhs = parseRelFactor();

  if (!rhs) {
    reset();
    return nullptr;
  }

  return std::make_unique<RelExprNode>(std::move(lhs), op, std::move(rhs));
}

// cond_expr: rel_expr
// | ! ( cond_expr )
// | ( cond_expr ) && ( cond_expr )
// | ( cond_expr ) || ( cond_expr )

std::unique_ptr<CondExprNode> Parser::parseCondExpr() {
  save_loc();
  auto relExpr = parseRelExpr();
  if (relExpr) {
    return std::make_unique<CondExprNode>(std::move(relExpr));
  }

  if (match("!")) {
    expect("(");
    auto condExpr = parseCondExpr();
    expect(")");
    return std::make_unique<CondExprNode>(std::move(condExpr));
  }

  expect("(");
  auto condLHS = parseCondExpr();
  expect(")");

  std::string op;

  if (match("&&")) {
    op = "&&";
  } else if (match("||")) {
    op = "||";
  } else {
    // TODO: HANDLE BETTER
    std::cout << "EXPECTING AN OP";
    reset();
  }

  expect("(");
  auto condRHS = parseCondExpr();
  expect(")");

  return std::make_unique<CondExprNode>(std::move(condLHS), op,
                                        std::move(condRHS));
};

std::unique_ptr<WhileNode> Parser::parseWhile() {
  save_loc();
  if (!match("while")) {
    reset();
    return nullptr;
  }

  if (!match("(")) {
    reset();
    return nullptr;
  }

  auto condExpr = parseCondExpr();
  if (!condExpr) {
    // TODO: HANDLE ERROR BETTER
    std::cout << "Expected a cond expression";
  }
  expect(")");
  expect("{");
  auto stmtList = parseStmtList();
  if (!stmtList) {
    // TODO: HANDLE ERROR BETTER
    std::cout << "Expected a stmtlist";
  }
  expect("}");
  return std::make_unique<WhileNode>(std::move(condExpr), std::move(stmtList));
};

std::unique_ptr<IfNode> Parser::parseIf() {
  save_loc();
  if (!match("if")) {
    reset();
    return nullptr;
  }

  expect("(");
  auto condExpr = parseCondExpr();
  if (!condExpr) {
    // TODO: HANDLE ERROR BETTER
    std::cout << "Expected a cond expression";
  }
  expect(")");
  expect("then");
  expect("{");
  auto stmtListThen = parseStmtList();
  if (!stmtListThen) {
    // TODO: HANDLE ERROR BETTER
    std::cout << "Expected a stmtlist";
  }
  expect("}");
  expect("else");
  expect("{");
  auto stmtListElse = parseStmtList();
  if (!stmtListElse) {
    // TODO: HANDLE ERROR BETTER
    std::cout << "Expected a stmtlist";
  }
  expect("}");
  return std::make_unique<IfNode>(std::move(condExpr), std::move(stmtListThen),
                                  std::move(stmtListElse));
};

Parser::Parser(std::vector<Token*> t) : tokens(t){};
std::unique_ptr<ProcedureNode> Parser::parse() { return parseProcedure(); }
