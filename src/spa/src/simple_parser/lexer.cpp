#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "simple_parser/lexer.h"
#include "simple_parser/token.h"

using namespace Simple;

void getSymbol(std::istream& stream, std::string& str);
void getNumber(std::istream& stream, std::string& str);

void getSymbol(std::istream& stream, std::string& str) {
  while (isalnum(stream.peek())) {
    str += stream.get();
  }
}

void getNumber(std::istream& stream, std::string& str) {
  while (isdigit(stream.peek())) {
    str += stream.get();
  }
}

Simple::Lexer::Lexer(std::istream& stream) {
  char nextChar;
  std::string str;
  int row = 1;

  while (!stream.eof()) {
    nextChar = stream.get();
    str += nextChar;
    if (isalpha(nextChar)) {  // Reserved keyword or variable
      getSymbol(stream, str);
      if (str.compare("procedure") == 0) {
        tokens.push_back(new Token(row, TokenType::PROCEDURE));
      } else if (str.compare("while") == 0) {
        tokens.push_back(new Token(row, TokenType::WHILE));
      } else if (str.compare("if") == 0) {
        tokens.push_back(new Token(row, TokenType::IF));
      } else if (str.compare("then") == 0) {
        tokens.push_back(new Token(row, TokenType::THEN));
      } else if (str.compare("else") == 0) {
        tokens.push_back(new Token(row, TokenType::ELSE));
      } else {  // A regular symbol
        tokens.push_back(new SymbolToken(row, str));
      }
    } else if (isdigit(nextChar)) {  // Numbers
      getNumber(stream, str);
      tokens.push_back(new NumberToken(row, str));
    } else if (nextChar == '{') {
      tokens.push_back(new Token(row, TokenType::L_BRACE));
    } else if (nextChar == '}') {
      tokens.push_back(new Token(row, TokenType::R_BRACE));
    } else if (nextChar == '(') {
      tokens.push_back(new Token(row, TokenType::L_PAREN));
    } else if (nextChar == ')') {
      tokens.push_back(new Token(row, TokenType::R_PAREN));
    } else if (nextChar == '=') {
      if (stream.peek() == '=') {
        stream.get();  // Consume character
        tokens.push_back(new Token(row, TokenType::EQUALEQUAL));
      } else {
        tokens.push_back(new Token(row, TokenType::EQUAL));
      }
    } else if (nextChar == '+') {
      tokens.push_back(new Token(row, TokenType::PLUS));
    } else if (nextChar == ';') {
      tokens.push_back(new Token(row, TokenType::SEMI));
    } else if (nextChar == '\n') {
      row++;
    } else if (iswspace(nextChar)) {  // Ignore whitespaces
      ;
    } else {
      std::cout << nextChar;  // TODO: Properly handle error
    }
    str = "";
  }
  tokens.push_back(new Token(row, END_OF_FILE));
};
