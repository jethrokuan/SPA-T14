/*!
  \file   lexer.h
  \brief  Lexer for the SIMPLE language.

  The Lexer class provides a skeleton for processing the SIMPLE language.
  The implementation is provided in lexer.cpp.

  Usage:

  ~~~~~{.cpp}
  std::string filename = "source.txt";
  std::ifstream input(filename);

  Simple::Lexer lexer = Simple::Lexer(input);
  try {
    lexer.parse();
  } catch(const SimpleLexException& e) {
    std::cout << e.what() << std::endl;
  }
 ~~~~~
*/

#pragma once

#include <fstream>
#include <vector>

#include "simple_parser/token.h"

namespace Simple {
class Lexer {
 private:
  int lineno = 1;
  int colno = 0;
  std::istream& stream;
  std::string str;  // Represents the current string

  char peek() { return stream.peek(); }
  char advance() {
    colno++;
    return stream.get();
  }

  // Reads the full symbol from the stream.
  void readSymbol();

  // Reads the full number from the stream.
  void readNumber();

 public:
  std::vector<Token*> tokens;

  // Initiializes the Lexer with an input stream
  // This could be anything from a string stream, to an input file stream
  Lexer(std::istream& stream_);

  // Lexes the text in the stream, and stores them in tokens.
  void parse();
};
}  // namespace Simple
