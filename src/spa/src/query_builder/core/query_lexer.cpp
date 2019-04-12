#include "query_builder/core/query_lexer.h"
#include <cctype>
#include <iostream>
#include <unordered_set>
#include "query_builder/core/exceptions.h"

using QE::QueryLexer;

namespace QE {
std::unordered_set<std::string> single_token_puncts({"(", ")", ",", "_", "<",
                                                     "*", ">", ";", "=", "."});
std::unordered_set<std::string> ops_with_t({"Follows", "Parent", "Calls",
                                            "Next", "Affects", "NextP",
                                            "AffectsP"});
}  // namespace QE

QueryLexer::QueryLexer(std::string query) : Curr(0), Query(query){};

char QueryLexer::peek() {
  char c;
  if (Curr >= Query.length()) {
    c = EOF;
  } else {
    c = Query[Curr];
  }
  return c;
}

char QueryLexer::advance() {
  char v = peek();
  Curr++;
  return v;
}

void QueryLexer::readSymbol() {
  while (isalnum(peek())) {
    Str += advance();
  }

  if (Str == "stmt" && peek() == '#') {
    Str += advance();
  }

  if (ops_with_t.find(Str) != ops_with_t.end() && peek() == '*') {
    Str += advance();
  }
}

void QueryLexer::readNumber() {
  while (isdigit(peek()) != 0) {
    Str += advance();
  }
}

bool QueryLexer::isAtEnd() { return Curr == Query.length(); }

std::vector<std::string> QueryLexer::lex() {
  char nextChar;

  while (!isAtEnd()) {
    nextChar = advance();

    if (nextChar == EOF) {
      break;
    }

    Str += nextChar;
    if (isspace(nextChar)) {
    } else if (isalpha(nextChar)) {  // Symbols
      readSymbol();
      Tokens.push_back(Str);
    } else if (isdigit(nextChar)) {
      readNumber();
      Tokens.push_back(Str);
    } else if (nextChar == '"') {
      Tokens.push_back("\"");
      Str = "";
      while (peek() != '"') {
        Str += advance();
      }
      Tokens.push_back(Str);
      Tokens.push_back("\"");
      Curr++;
    } else if (QE::single_token_puncts.find(Str) !=
               QE::single_token_puncts.end()) {
      Tokens.push_back(Str);
    } else {
      throw PQLTokenizeException("Unexpected token " +
                                 std::string(1, nextChar) + "\n");
    }

    Str = "";
  }

  Tokens.push_back("");  // Use empty string to denote end
  return Tokens;
}
