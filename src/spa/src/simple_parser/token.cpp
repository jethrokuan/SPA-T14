#include "simple_parser/token.h"

using namespace Simple;

Token::Token(TokenType t, std::string val) : T(t), Val(val){};
bool Token::operator==(const Token& other) const {
  return this->T == other.T && this->Val.compare(other.Val) == 0;
};

SymbolToken::SymbolToken(std::string val) : Token(TokenType::SYMBOL, val){};
NumberToken::NumberToken(std::string val) : Token(TokenType::NUMBER, val){};
PunctToken::PunctToken(std::string val) : Token(TokenType::PUNCT, val){};
EndOfFileToken::EndOfFileToken() : Token(TokenType::END_OF_FILE, ""){};
