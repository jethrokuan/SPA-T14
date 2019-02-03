#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "simple_parser/token.h"
#include "structs/node.h"

namespace Simple {
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
