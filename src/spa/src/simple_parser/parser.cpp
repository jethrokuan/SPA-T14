#include "simple_parser/parser.h"

#include <unordered_set>

using namespace Simple;

#include "simple_parser/parser.h"

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

std::shared_ptr<NumberNode> Parser::parseNumber() {
  save_loc();
  if (match(TokenType::NUMBER)) {
    std::string num = static_cast<NumberToken*>(previous())->Val;
    auto result = std::make_shared<NumberNode>(num);
    return result;
  } else {
    reset();
    return nullptr;
  }
}

std::shared_ptr<VariableNode> Parser::parseVariable() {
  save_loc();
  if (match(TokenType::SYMBOL)) {
    std::string name = static_cast<SymbolToken*>(previous())->Val;
    auto result = std::make_shared<VariableNode>(name);
    return result;
  } else {
    reset();
    return nullptr;
  }
};

std::shared_ptr<ProcedureNode> Parser::parseProcedure() {
  save_loc();
  if (!match("procedure")) {
    reset();
    return nullptr;
  }

  auto Var = parseVariable();
  auto procName = Var->Name;

  if (!Var) {
    reset();
    return nullptr;
  }

  expect("{");

  auto StmtList = parseStmtList();

  expect("}");

  return std::make_shared<ProcedureNode>(procName, std::move(StmtList));
};

std::shared_ptr<StmtListNode> Parser::parseStmtList() {
  save_loc();
  std::vector<StmtNode> stmts;
  while (true) {
    auto stmt = parseStatement();
    if (stmt) {
      stmts.push_back(std::move(*stmt));
    } else {
      break;
    }
  }

  // TODO: Handle case where statement list is empty

  return std::make_shared<StmtListNode>(std::move(stmts));
};

std::optional<StmtNode> Parser::parseStatement() {
  save_loc();
  auto readStmt = parseRead();
  if (readStmt) {
    return readStmt;
  }

  auto printStmt = parsePrint();
  if (printStmt) {
    return printStmt;
  }

  auto assignStmt = parseAssign();
  if (assignStmt) {
    return assignStmt;
  }

  auto whileStmt = parseWhile();
  if (whileStmt) {
    return whileStmt;
  }

  auto ifStmt = parseIf();
  if (ifStmt) {
    return ifStmt;
  }

  reset();
  return std::nullopt;
};

std::shared_ptr<FactorNode> Parser::parseFactor() {
  save_loc();
  auto Var = parseVariable();
  if (Var) {
    return std::make_shared<FactorNode>(std::move(Var));
  }

  auto Number = parseNumber();

  if (Number) {
    return std::make_shared<FactorNode>(std::move(Number));
  }

  expect("(");
  auto Expr = parseExpr();
  expect(")");
  return std::make_shared<FactorNode>(std::move(Expr));
};

std::shared_ptr<TermPNode> Parser::parseTermP() {
  save_loc();
  std::unordered_set<std::string> valid_ops({"*", "/", "%"});

  if (valid_ops.find(peek()->Val) != valid_ops.end()) {
    auto Factor = parseFactor();
    auto TermP = parseTermP();
    return std::make_shared<TermPNode>(std::move(Factor), advance()->Val,
                                       std::move(TermP));
  } else {
    reset();
    return nullptr;
  }
};

std::shared_ptr<TermNode> Parser::parseTerm() {
  save_loc();
  auto Factor = parseFactor();
  if (!Factor) {
    reset();
    return nullptr;
  }

  auto TermP = parseTermP();

  return std::make_shared<TermNode>(std::move(Factor), std::move(TermP));
}

std::shared_ptr<ExprPNode> Parser::parseExprP() {
  save_loc();
  std::unordered_set<std::string> valid_ops({"+", "-"});
  if (valid_ops.find(peek()->Val) != valid_ops.end()) {
    auto Term = parseTerm();
    auto ExprP = parseExprP();
    return std::make_shared<ExprPNode>(std::move(Term), advance()->Val,
                                       std::move(ExprP));
  } else {
    reset();
    return nullptr;
  }
};

std::shared_ptr<ExprNode> Parser::parseExpr() {
  save_loc();
  auto Term = parseTerm();

  if (!Term) {
    reset();
    return nullptr;
  }

  auto ExprP = parseExprP();
  return std::make_shared<ExprNode>(std::move(Term), std::move(ExprP));
};

std::shared_ptr<AssignNode> Parser::parseAssign() {
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

  return std::make_shared<AssignNode>(std::move(Var), std::move(Expr));
};

std::shared_ptr<ReadNode> Parser::parseRead() {
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

  return std::make_shared<ReadNode>(std::move(Var));
};

std::shared_ptr<PrintNode> Parser::parsePrint() {
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

  return std::make_shared<PrintNode>(std::move(Var));
};

std::shared_ptr<RelFactorNode> Parser::parseRelFactor() {
  save_loc();
  auto Var = parseVariable();
  if (Var) {
    return std::make_shared<FactorNode>(std::move(Var));
  }

  auto Number = parseNumber();

  if (Number) {
    return std::make_shared<FactorNode>(std::move(Number));
  }

  auto Expr = parseExpr();

  if (!Expr) {
    reset();
    return nullptr;
  }

  return std::make_shared<FactorNode>(std::move(Expr));
};

// rel_expr: rel_factor > rel_factor
// | rel_factor >= rel_factor
// | rel_factor < rel_factor
// | rel_factor <= rel_factor
// | rel_factor == rel_factor
// | rel_factor != rel_factor
std::shared_ptr<RelExprNode> Parser::parseRelExpr() {
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

  return std::make_shared<RelExprNode>(std::move(lhs), op, std::move(rhs));
}

// cond_expr: rel_expr
// | ! ( cond_expr )
// | ( cond_expr ) && ( cond_expr )
// | ( cond_expr ) || ( cond_expr )

std::shared_ptr<CondExprNode> Parser::parseCondExpr() {
  save_loc();
  auto relExpr = parseRelExpr();
  if (relExpr) {
    return std::make_shared<CondExprNode>(std::move(relExpr));
  }

  if (match("!")) {
    expect("(");
    auto condExpr = parseCondExpr();
    expect(")");
    return std::make_shared<CondExprNode>(std::move(condExpr));
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

  return std::make_shared<CondExprNode>(std::move(condLHS), op,
                                        std::move(condRHS));
};

std::shared_ptr<WhileNode> Parser::parseWhile() {
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
  return std::make_shared<WhileNode>(std::move(condExpr), std::move(stmtList));
};

std::shared_ptr<IfNode> Parser::parseIf() {
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
  return std::make_shared<IfNode>(std::move(condExpr), std::move(stmtListThen),
                                  std::move(stmtListElse));
};

Parser::Parser(std::vector<Token*> t) : tokens(t){};
std::shared_ptr<ProcedureNode> Parser::parse() { return parseProcedure(); }
