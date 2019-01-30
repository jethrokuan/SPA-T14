/* Lexer for SIMPLE language.

   Here we use a handwritten DFA to lex the tokens, because the
   language is relatively simple, and this makes lexing fast compared
   to regex approaches. The DFA can easily be extended to handle
   changes to the language.
*/

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
        tokens.push_back(new Token(row, "PROCEDURE"));
      } else if (str.compare("read") == 0) {
        tokens.push_back(new Token(row, "READ"));
      } else if (str.compare("print") == 0) {
        tokens.push_back(new Token(row, "PRINT"));
      } else if (str.compare("while") == 0) {
        tokens.push_back(new Token(row, "WHILE"));
      } else if (str.compare("if") == 0) {
        tokens.push_back(new Token(row, "IF"));
      } else if (str.compare("then") == 0) {
        tokens.push_back(new Token(row, "THEN"));
      } else if (str.compare("else") == 0) {
        tokens.push_back(new Token(row, "ELSE"));
      } else {  // A regular symbol
        tokens.push_back(new SymbolToken(row, str));
      }
    } else if (isdigit(nextChar)) {  // Numbers
      getNumber(stream, str);
      tokens.push_back(new NumberToken(row, str));
    } else if (nextChar == '{') {
      tokens.push_back(new Token(row, "L_BRACE"));
    } else if (nextChar == '}') {
      tokens.push_back(new Token(row, "R_BRACE"));
    } else if (nextChar == '(') {
      tokens.push_back(new Token(row, "L_PAREN"));
    } else if (nextChar == ')') {
      tokens.push_back(new Token(row, "R_PAREN"));
    } else if (nextChar == '!') {
      if (stream.peek() == '=') {
        stream.get();
        tokens.push_back(new Token(row, "BANG_EQUAL"));
      } else {
        tokens.push_back(new Token(row, "BANG"));
      }
    } else if (nextChar == '=') {
      if (stream.peek() == '=') {
        stream.get();  // Consume character
        tokens.push_back(new Token(row, "EQUAL_EQUAL"));
      } else {
        tokens.push_back(new Token(row, "EQUAL"));
      }
    } else if (nextChar == '&') {
      if (stream.peek() == '&') {
        stream.get();
        tokens.push_back(new Token(row, "AND"));
      } else {
        std::cout << "Expecting another &";  // TODO: Handle Error properly
      }
    } else if (nextChar == '|') {
      if (stream.peek() == '|') {
        stream.get();
        tokens.push_back(new Token(row, "OR"));
      } else {
        std::cout << "Expecting another |";  // TODO: Handle Error properly
      }
    } else if (nextChar == '=') {
      if (stream.peek() == '=') {
        stream.get();  // Consume character
        tokens.push_back(new Token(row, "EQUAL_EQUAL"));
      } else {
        tokens.push_back(new Token(row, "EQUAL"));
      }
    } else if (nextChar == '>') {
      if (stream.peek() == '=') {
        stream.get();  // Consume character
        tokens.push_back(new Token(row, "GREATER_EQUAL"));
      } else {
        tokens.push_back(new Token(row, "GREATER"));
      }
    } else if (nextChar == '<') {
      if (stream.peek() == '=') {
        stream.get();  // Consume character
        tokens.push_back(new Token(row, "LESS_EQUAL"));
      } else {
        tokens.push_back(new Token(row, "LESS"));
      }
    } else if (nextChar == '+') {
      tokens.push_back(new Token(row, "PLUS"));
    } else if (nextChar == '-') {
      tokens.push_back(new Token(row, "MINUS"));
    } else if (nextChar == '*') {
      tokens.push_back(new Token(row, "TIMES"));
    } else if (nextChar == '/') {
      tokens.push_back(new Token(row, "DIVIDE"));
    } else if (nextChar == '%') {
      tokens.push_back(new Token(row, "MOD"));
    } else if (nextChar == ';') {
      tokens.push_back(new Token(row, "SEMI"));
    } else if (nextChar == '\n') {
      row++;
    } else if (iswspace(nextChar)) {  // Ignore whitespaces
      ;
    } else {
      std::cout << nextChar;  // TODO: Properly handle error
    }
    str = "";
  }
  tokens.push_back(new Token(row, "END_OF_FILE"));
};
