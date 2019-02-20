#include "simple_parser/exceptions.h"
#include "simple_parser/pratt.h"
#include "simple_parser/token.h"
#include "structs/node.h"

#include <unordered_set>

using namespace Simple;

Token* ExprParser::advance() {
  if (!isAtEnd()) current++;
  return previous();
};

bool ExprParser::isAtEnd() { return peek()->T == TokenType::END_OF_FILE; };

Token* ExprParser::peek() { return tokens[current]; };

Token* ExprParser::previous() { return tokens[current - 1]; };

bool ExprParser::check(std::string s) {
  if (isAtEnd()) return false;
  return peek()->Val.compare(s) == 0;
};

bool ExprParser::match(std::string s) {
  if (check(s)) {
    advance();
    return true;
  } else {
    return false;
  }
}

bool ExprParser::expect(std::string s) {
  if (match(s)) {
    return true;
  } else {
    throw SimpleParseException("Expected '" + s + "', got '" + peek()->Val +
                               "'.");
  }
};

Expr ExprParser::nud(Token* t) {
  if (t->T == TokenType::NUMBER) {
    return std::make_shared<NumberNode>(t->Val);
  } else if (t->T == TokenType::SYMBOL) {
    return std::make_shared<VariableNode>(t->Val);
  } else if (t->Val == "(") {
    Expr expr = parse(0);
    expect(")");
    return expr;
  } else {
    throw SimpleParseException("Expected an expression, got '" + t->Val + "'.");
  }
}

int ExprParser::lbp(Token* t) {
  if (delimiters.find(t->Val) != delimiters.end() ||
      t->T == TokenType::END_OF_FILE) {
    return 0;
  } else if (t->Val == "+" || t->Val == "-") {
    return 10;
  } else if (t->Val == "*" || t->Val == "/" || t->Val == "%") {
    return 20;
  } else {
    throw SimpleParseException("Unexpected token '" + t->Val + "'.");
  }
}

Expr ExprParser::led(Token* t, Expr left) {
  if (t->Val.compare("+") == 0) {
    Expr right = parse(10);
    return std::make_shared<BinOpNode>(left, right, "+");
  } else if (t->Val.compare("-") == 0) {
    Expr right = parse(10);
    return std::make_shared<BinOpNode>(left, right, "-");
  } else if (t->Val.compare("*") == 0) {
    Expr right = parse(20);
    return std::make_shared<BinOpNode>(left, right, "*");
  } else if (t->Val.compare("/") == 0) {
    Expr right = parse(20);
    return std::make_shared<BinOpNode>(left, right, "/");
  } else if (t->Val.compare("%") == 0) {
    Expr right = parse(20);
    return std::make_shared<BinOpNode>(left, right, "%");
  } else {
    throw SimpleParseException("led called on invalid token " + t->Val);
  }
}

ExprParser::ExprParser(std::vector<Token*>& _tokens, int& _current,
                       std::unordered_set<std::string> _delimiters)
    : current(_current), tokens(_tokens), delimiters(_delimiters){};

Expr ExprParser::parse(int rbp) {
  Token* t = advance();
  Expr left = nud(t);

  while (rbp < lbp(peek())) {
    t = advance();
    left = led(t, left);
  }

  return left;
}
