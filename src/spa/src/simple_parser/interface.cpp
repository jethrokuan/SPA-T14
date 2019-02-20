#include "simple_parser/interface.h"
#include "simple_parser/lexer.h"
#include "simple_parser/parser.h"
#include "simple_parser/pratt.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <unordered_set>

using namespace Simple;

std::shared_ptr<ProcedureNode> SimpleInterface::getAstFromFile(
    std::string filename) {
  std::ifstream input(filename);

  Lexer lexer = Lexer(input);
  lexer.parse();

  Parser parser = Parser(lexer.tokens);
  auto ast = parser.parse();
  return ast;
}

Expr SimpleInterface::parseExpression(std::string expression) {
  std::stringstream s(expression);
  Lexer lexer = Lexer(s);
  lexer.parse();

  int current = 0;
  std::unordered_set<std::string> delimiters;

  ExprParser parser = ExprParser(lexer.tokens, current, delimiters);
  Expr expr = parser.parse(0);

  return expr;
}
