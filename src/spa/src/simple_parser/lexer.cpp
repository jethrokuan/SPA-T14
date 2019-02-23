/*!
  \file   lexer.cpp
  \brief  Implementation for the SIMPLE Lexer.

  Here we use a handwritten DFA to lex the tokens, because the
  language is relatively simple, and this makes lexing fast compared
  to regex approaches. The DFA can easily be extended to handle
  changes to the language.

  \date   2019-02-04
*/

#include <cctype>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "simple_parser/exceptions.h"
#include "simple_parser/lexer.h"
#include "simple_parser/token.h"

using namespace Simple;

std::unordered_set<std::string> single_token_puncts({"{", "}", "(", ")", "+",
                                                     "-", "*", "/", "%", ";"});

char Lexer::peek() { return stream->peek(); }

char Lexer::advance() {
  colno++;
  return stream->get();
}

void Lexer::readSymbol() {
  while (isalnum(peek()) != 0) {
    str += advance();
  }
}

void Lexer::readNumber() {
  while (isdigit(peek()) != 0) {
    str += advance();
  }
}

Lexer::Lexer(std::istream* stream_) : stream(stream_){};

void Lexer::lex() {
  char nextChar;

  while (!stream->eof()) {
    nextChar = advance();
    if (nextChar == EOF) {
      break;
    }
    str += nextChar;
    if (nextChar == '\n') {
      lineno++;
      colno = 1;
    } else if (isspace(nextChar) != 0) {  // Ignore whitespaces
      ;
    } else if (isalpha(nextChar) != 0) {  // Symbols
      readSymbol();
      tokens.push_back(new SymbolToken(str));
    } else if (isdigit(nextChar) != 0) {  // Numbers
      readNumber();
      tokens.push_back(new NumberToken(str));
    } else {  // Punctuations
      if (nextChar == '!') {
        if (peek() == '=') {
          str += advance();
        }
      } else if (nextChar == '=') {
        if (peek() == '=') {
          str += advance();
        }
      } else if (nextChar == '&') {
        if (peek() == '&') {
          str += advance();
        } else {
          throw SimpleLexException(lineno, colno, '&', peek());
        }
      } else if (nextChar == '|') {
        if (peek() == '|') {
          str += advance();
        } else {
          throw SimpleLexException(lineno, colno, '|', peek());
        }
      } else if (nextChar == '=') {
        if (peek() == '=') {
          str += advance();
        }
      } else if (nextChar == '>') {
        if (peek() == '=') {
          str += advance();
        }
      } else if (nextChar == '<') {
        if (peek() == '=') {
          str += advance();
        }
      } else if (single_token_puncts.find(str) != single_token_puncts.end()) {
        // Is valid single punct character
        ;
      } else {
        throw SimpleLexException(lineno, colno, nextChar);
      }
      tokens.push_back(new PunctToken(str));
    }
    str = "";
  }

  tokens.push_back(new EndOfFileToken());
};
