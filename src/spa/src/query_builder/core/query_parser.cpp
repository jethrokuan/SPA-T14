#include "query_builder/core/exceptions.h"
#include "query_builder/core/query_parser.h"
#include "query_builder/pql/design_entity.h"
#include "query_builder/pql/query.h"

#include <optional>

using QE::Declaration;
using QE::DesignEntity;
using QE::Query;
using QE::QueryParser;
using QE::Synonym;

QueryParser::QueryParser(std::vector<std::string> tokens)
    : query_(new Query()), current_(0), tokens_(tokens) {}

bool QueryParser::match(std::string s) {
  if (check(s)) {
    advance();
    return true;
  }
  return false;
}

bool QueryParser::check(std::string s) { return peek().compare(s) == 0; }

bool QueryParser::expect(std::string s) {
  if (match(s)) {
    return true;
  }
  throw PQLParseException("Expected '" + s + "', got '" + peek() + "'.");
}

bool QueryParser::isAtEnd() { return peek().compare("") == 0; }
std::string QueryParser::peek() { return tokens_[current_]; }
std::string QueryParser::previous() { return tokens_[current_ - 1]; }
std::string QueryParser::advance() {
  if (!isAtEnd()) current_++;
  return previous();
}

bool QueryParser::parseDeclarationClause() {
  unsigned int save_loc = current_;
  std::vector<Synonym> synonyms;
  std::string de_str = advance();
  std::string synonym_str = advance();
  // TODO: Inheriting clunky code here, should fix someday
  DesignEntity de;
  try {
    de = getDesignEntity(de_str);
  } catch (const PQLParseException& e) {
    current_ = save_loc;
    return false;
  }
  std::optional<Synonym> synonym = Synonym::construct(synonym_str);
  if (!synonym) {
    throw PQLParseException("Failed to parse synonym");
  }
  synonyms.push_back(*synonym);

  while (!match(";")) {
    expect(",");
    synonym_str = advance();
    synonym = Synonym::construct(synonym_str);
    if (!synonym) {
      throw PQLParseException("Failed to parse synonym");
    }
    synonyms.push_back(*synonym);
  }

  for (const Synonym syn : synonyms) {
    Declaration decl = Declaration(de, syn);
    query_->declarations->push_back(decl);
  }

  return true;
}

Query QueryParser::parse() {
  // Parsing declarations
  while (!isAtEnd()) {
    while (true) {
      bool isDeclarationClause = parseDeclarationClause();
      if (!isDeclarationClause) {
        break;
      }
    }
  }

  return *query_;
  // Parsing selects and clauses
}
