#include "simple_parser/interface.h"
#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"
#include "simple_parser/pratt.h"
#include "structs/node.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>

using Simple::SimpleInterface;

AST SimpleInterface::getAstFromFile(std::string filename) {
  std::ifstream input(filename);

  Lexer lexer = Lexer(&input);
  lexer.lex();

  Parser parser = Parser(lexer.tokens);
  AST ast = parser.parse();
  return ast;
}

Expr SimpleInterface::parseExpression(std::string expression) {
  std::stringstream s(expression);
  Lexer lexer = Lexer(&s);
  lexer.lex();

  int current = 0;
  std::unordered_set<std::string> delimiters;

  ExprParser parser = ExprParser(lexer.tokens, &current, delimiters);
  Expr expr = parser.parse(0);

  return expr;
}
