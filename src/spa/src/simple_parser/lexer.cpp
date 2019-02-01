/* Lexer for SIMPLE language.

    Here we use a handwritten DFA to lex the tokens, because the
    language is relatively simple, and this makes lexing fast compared
    to regex approaches. The DFA can easily be extended to handle
    changes to the language.
 */

#include <cctype>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "simple_parser/lexer.h"
#include "simple_parser/token.h"

using namespace Simple;

std::unordered_set<std::string> single_token_puncts({"{", "}", "(", ")", "+",
                                                     "-", "*", "/", "%", ";"});

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

  while (!stream.eof()) {
    nextChar = stream.get();
    if (nextChar == EOF) break;
    str += nextChar;
    if (isspace(nextChar)) {  // Ignore whitespaces
      ;
    } else if (isalpha(nextChar)) {  // Symbols
      getSymbol(stream, str);
      tokens.push_back(new SymbolToken(str));
    } else if (isdigit(nextChar)) {  // Numbers
      getNumber(stream, str);
      tokens.push_back(new NumberToken(str));
    } else {  // Punctuations
      if (nextChar == '!') {
        if (stream.peek() == '=') {
          str += stream.get();
        }
      } else if (nextChar == '=') {
        if (stream.peek() == '=') {
          str += stream.get();
        }
      } else if (nextChar == '&') {
        if (stream.peek() == '&') {
          str += stream.get();
        } else {
          std::cout << "Expecting another &";  // TODO: Handle Error properly
        }
      } else if (nextChar == '|') {
        if (stream.peek() == '|') {
          str += stream.get();
        } else {
          std::cout << "Expecting another |";  // TODO: Handle Error properly
        }
      } else if (nextChar == '=') {
        if (stream.peek() == '=') {
          str += stream.get();
        }
      } else if (nextChar == '>') {
        if (stream.peek() == '=') {
          str += stream.get();
        }
      } else if (nextChar == '<') {
        if (stream.peek() == '=') {
          str += stream.get();
        }
      } else if (single_token_puncts.find(str) != single_token_puncts.end()) {
        // Is valid single punct character
        ;
      } else {
        // TODO: HANDLE PARSE ERROR
        std::cout << "WAT" << int(nextChar);
      }
      tokens.push_back(new PunctToken(str));
    }
    str = "";
  }
  tokens.push_back(new EndOfFileToken());
};
