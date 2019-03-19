/*
  Viz:

  Takes in a source text, and produces output for graphviz processing.

  Example Usage:
  viz source.txt | dot -Tpng > source-cfg.png

 */
#include <iostream>

#include "program_knowledge_base/pkb_manager.h"
#include "simple_parser/interface.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Received " << argc - 1 << " arguments, expecting 1.";
    exit(1);
  }

  std::string source_path = std::string(argv[1]);

  AST ast = Simple::SimpleInterface::getAstFromFile(source_path);
  PKB::PKBManager* pkb = new PKB::PKBManager(ast);
  auto adj_list = pkb->getCFG();

  std::cout << "digraph cfg {" << std::endl;
  for (const auto& node : adj_list) {
    std::cout << node.first;
    for (const auto& outward_node : node.second) {
      std::cout << " -> " << outward_node;
    }
    std::cout << std::endl;
  }
  std::cout << "}";

  return 0;
}
