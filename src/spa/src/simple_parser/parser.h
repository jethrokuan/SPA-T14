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
  std::shared_ptr<NumberNode> parseNumber();
  std::shared_ptr<VariableNode> parseVariable();
  std::shared_ptr<ProcedureNode> parseProcedure();
  std::shared_ptr<StmtListNode> parseStmtList();
  std::shared_ptr<Node> parseStatement();
  std::shared_ptr<FactorNode> parseFactor();
  std::shared_ptr<RelFactorNode> parseRelFactor();
  std::shared_ptr<TermPNode> parseTermP();
  std::shared_ptr<TermNode> parseTerm();
  std::shared_ptr<ExprPNode> parseExprP();
  std::shared_ptr<ExprNode> parseExpr();
  std::shared_ptr<RelExprNode> parseRelExpr();
  std::shared_ptr<AssignNode> parseAssign();
  std::shared_ptr<ReadNode> parseRead();
  std::shared_ptr<PrintNode> parsePrint();
  std::shared_ptr<CondExprNode> parseCondExpr();
  std::shared_ptr<WhileNode> parseWhile();
  std::shared_ptr<IfNode> parseIf();

 public:
  Parser(std::vector<Token*> t);
  std::shared_ptr<ProcedureNode> parse();
};
}  // namespace Simple
