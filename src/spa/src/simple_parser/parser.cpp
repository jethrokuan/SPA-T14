#include "simple_parser/exceptions.h"
#include "simple_parser/parser.h"

#include <unordered_set>

using namespace Simple;

#include <iostream>

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
    throw SimpleParseException("Expected '" + s + "', got '" + peek()->Val +
                               "'.");
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
  auto current_loc = current;
  if (match(TokenType::NUMBER)) {
    std::string num = static_cast<NumberToken*>(previous())->Val;
    auto result = std::make_shared<NumberNode>(num);
    return result;
  } else {
    current = current_loc;
    return nullptr;
  }
}

std::shared_ptr<VariableNode> Parser::parseVariable() {
  auto current_loc = current;
  if (match(TokenType::SYMBOL)) {
    std::string name = static_cast<SymbolToken*>(previous())->Val;
    auto result = std::make_shared<VariableNode>(name);
    return result;
  } else {
    current = current_loc;
    return nullptr;
  }
};

std::shared_ptr<ProcedureNode> Parser::parseProcedure() {
  if (!match("procedure")) {
    throw SimpleParseException("Expected 'procedure', got '" + peek()->Val +
                               "'.");
  }

  std::string procName;
  if (match(TokenType::SYMBOL)) {
    procName = static_cast<SymbolToken*>(previous())->Val;
  } else {
    throw SimpleParseException("Expected a valid procedure name, got '" +
                               peek()->Val + "'.");
  }

  expect("{");

  auto StmtList = parseStmtList();

  expect("}");

  return std::make_shared<ProcedureNode>(procName, std::move(StmtList));
};

std::shared_ptr<StmtListNode> Parser::parseStmtList() {
  std::vector<StmtNode> stmts;
  while (true) {
    auto stmt = parseStatement();
    if (stmt) {
      stmts.push_back(std::move(*stmt));
    } else {
      break;
    }
  }

  if (stmts.size() == 0) {
    throw SimpleParseException("Statement list cannot be empty.");
  }

  return std::make_shared<StmtListNode>(std::move(stmts));
};

std::optional<StmtNode> Parser::parseStatement() {
  if (check("}")) {
    // End of statement list, return early
    return std::nullopt;
  }

  auto current_loc = current;
  auto readStmt = parseRead();
  if (readStmt) {
    return readStmt;
  }

  auto printStmt = parsePrint();
  if (printStmt) {
    return printStmt;
  }

  auto whileStmt = parseWhile();
  if (whileStmt) {
    return whileStmt;
  }

  auto ifStmt = parseIf();
  if (ifStmt) {
    return ifStmt;
  }

  // We try the assign statement last, since it's the "most generic",
  // accepting any symbol token. Any parse error is attributed here
  auto assignStmt = parseAssign();

  if (assignStmt) {
    return assignStmt;
  }

  current = current_loc;
  // Shouldn't reach here
  return std::nullopt;
};

Factor Parser::parseFactor() {
  auto Var = parseVariable();
  if (Var) {
    return Var;
  }

  auto Number = parseNumber();

  if (Number) {
    return Number;
  }

  expect("(");
  auto Expr = parseExpr();
  expect(")");
  return Expr;
};

// TODO: Refactor
Expr Parser::nud(Token* t) {
  if (t->T == TokenType::NUMBER) {
    return std::make_shared<NumberNode>(t->Val);
  } else if (t->T == TokenType::SYMBOL) {
    return std::make_shared<VariableNode>(t->Val);
  } else if (t->Val == "(") {
    Expr expr = prattParse();
    expect(")");
    return expr;
  } else {
    throw SimpleParseException("Expected an expression, got '" + t->Val + "'.");
    // // TODO: Handle Error
    // std::cout << "nud called on invalid token" << t->Val << std::endl;
    // return std::make_shared<VariableNode>("FAIL");
  }
}

int Parser::lbp(Token* t) {
  std::unordered_set<std::string> valid_ops({">", ">=", "<", "<=", "==", "!="});
  if (t->Val == ";" || t->Val == ")" ||
      valid_ops.find(t->Val) != valid_ops.end()) {
    // Any of these tokens signify end of expr
    return 0;
  } else if (t->Val == "+" || t->Val == "-") {
    return 10;
  } else if (t->Val == "*" || t->Val == "/" || t->Val == "%") {
    return 20;
  } else {
    throw SimpleParseException("Unexpected token '" + t->Val + "'.");
  }
}

// TODO: Refactor
Expr Parser::led(Token* t, Expr left) {
  if (t->Val.compare("+") == 0) {
    Expr right = prattParse(10);
    return std::make_shared<BinOpNode>(left, right, "+");
  } else if (t->Val.compare("-") == 0) {
    Expr right = prattParse(10);
    return std::make_shared<BinOpNode>(left, right, "-");
  } else if (t->Val.compare("*") == 0) {
    Expr right = prattParse(20);
    return std::make_shared<BinOpNode>(left, right, "*");
  } else if (t->Val.compare("/") == 0) {
    Expr right = prattParse(20);
    return std::make_shared<BinOpNode>(left, right, "/");
  } else if (t->Val.compare("%") == 0) {
    Expr right = prattParse(20);
    return std::make_shared<BinOpNode>(left, right, "%");
  } else {
    throw SimpleParseException("led called on invalid token " + t->Val);
  }

  // return left;
}

Expr Parser::prattParse(int rbp) {
  Token* t = advance();
  Expr left = nud(t);

  while (rbp < lbp(peek())) {
    t = advance();
    left = led(t, left);
  }

  return left;
}

Expr Parser::prattParse() { return prattParse(0); }

Expr Parser::parseExpr() { return prattParse(); };

std::shared_ptr<AssignNode> Parser::parseAssign() {
  auto Var = parseVariable();

  if (!Var) {
    throw SimpleParseException(
        "Expected a variable name for assign statement, got '" + peek()->Val +
        "'.");
  }

  if (!match("=")) {
    throw SimpleParseException("Expected '=', got '" + peek()->Val + "'.");
  }

  auto Expr = parseExpr();

  expect(";");

  return std::make_shared<AssignNode>(std::move(Var), std::move(Expr));
};

std::shared_ptr<ReadNode> Parser::parseRead() {
  auto current_loc = current;
  if (!match("read")) {
    current = current_loc;
    return nullptr;
  }

  auto Var = parseVariable();

  if (!Var) {
    current = current_loc;
    return nullptr;
  }

  expect(";");

  return std::make_shared<ReadNode>(std::move(Var));
};

std::shared_ptr<PrintNode> Parser::parsePrint() {
  auto current_loc = current;
  if (!match("print")) {
    current = current_loc;
    return nullptr;
  }

  auto Var = parseVariable();
  if (!Var) {
    current = current_loc;
    return nullptr;
  }

  expect(";");

  return std::make_shared<PrintNode>(std::move(Var));
};

RelFactor Parser::parseRelFactor() {
  auto Expr = parseExpr();

  return Expr;
};

// rel_expr: rel_factor > rel_factor
// | rel_factor >= rel_factor
// | rel_factor < rel_factor
// | rel_factor <= rel_factor
// | rel_factor == rel_factor
// | rel_factor != rel_factor
std::shared_ptr<RelExprNode> Parser::parseRelExpr() {
  auto lhs = parseRelFactor();

  std::unordered_set<std::string> valid_ops({">", ">=", "<", "<=", "==", "!="});
  auto op = peek()->Val;
  if (valid_ops.find(op) == valid_ops.end()) {
    throw SimpleParseException("Expected a comparator, got '" + op + "'.");
  }

  advance();

  auto rhs = parseRelFactor();

  return std::make_shared<RelExprNode>(lhs, op, rhs);
}

// cond_expr: rel_expr
// | ! ( cond_expr )
// | ( cond_expr ) && ( cond_expr )
// | ( cond_expr ) || ( cond_expr )

std::shared_ptr<CondExprNode> Parser::parseCondExpr() {
  if (match("!")) {  // !(condExpr)
    expect("(");
    auto condExpr = parseCondExpr();
    expect(")");
    return std::make_shared<CondExprNode>(std::move(condExpr));
  } else if (check("(")) {  // () op ()
    expect("(");
    auto condLHS = parseCondExpr();
    expect(")");

    std::string op;

    if (match("&&")) {
      op = "&&";
    } else if (match("||")) {
      op = "||";
    } else {
      throw SimpleParseException("Expected '||' or '&&', got '" + peek()->Val +
                                 "'.");
    }

    expect("(");
    auto condRHS = parseCondExpr();
    expect(")");

    return std::make_shared<CondExprNode>(std::move(condLHS), op,
                                          std::move(condRHS));
  } else {  // relExpr
    auto relExpr = parseRelExpr();

    if (relExpr) {
      return std::make_shared<CondExprNode>(std::move(relExpr));
    }
  }
  // Shouldn't reach here
  return nullptr;
};

std::shared_ptr<WhileNode> Parser::parseWhile() {
  auto current_loc = current;
  if (!match("while")) {
    current = current_loc;
    return nullptr;
  }

  if (!match("(")) {
    current = current_loc;
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
  auto current_loc = current;
  if (!match("if")) {
    current = current_loc;
    return nullptr;
  }

  if (!match("(")) {
    current = current_loc;
    return nullptr;
  }

  // From this point on, it is safe to assume that the parser is trying to
  // parse an if statement.

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
