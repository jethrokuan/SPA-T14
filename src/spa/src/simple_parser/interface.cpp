#include "simple_parser/interface.h"

#include <fstream>
#include <memory>

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
