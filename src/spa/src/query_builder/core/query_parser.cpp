#include "query_builder/core/exceptions.h"
#include "query_builder/core/query_parser.h"
#include "query_builder/pql/design_entity.h"
#include "query_builder/pql/query.h"
#include "query_builder/pql/result.h"

#include <optional>

using QE::Declaration;
using QE::DesignEntity;
using QE::Query;
using QE::QueryParser;
using QE::Result;
using QE::ResultType;
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

bool QueryParser::check(std::string s) {
  if (isAtEnd()) return false;
  return peek().compare(s) == 0;
}

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

Declaration* QueryParser::findDeclaration(const Synonym synonym) {
  auto found_declaration =
      std::find_if(query_->declarations->begin(), query_->declarations->end(),
                   [&](auto decl) { return decl.getSynonym() == synonym; });

  if (found_declaration == query_->declarations->end()) {
    throw PQLParseException("Semantic Error: cannot match synonym " +
                            synonym.synonym + " to list of declarations given");
  }

  return &query_->declarations->at(
      std::distance(query_->declarations->begin(), found_declaration));
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

void QueryParser::parseResult() {
  std::vector<Synonym> synonyms;
  query_->result->T = ResultType::TUPLE;
  query_->result->selected_declarations = new std::vector<Declaration*>();

  if (match("BOOLEAN")) {
    query_->result->T = ResultType::BOOLEAN;
    return;
  }

  if (!match("<")) {
    auto synonym_str = advance();
    auto synonym = Synonym::construct(synonym_str);

    if (!synonym) {
      throw PQLParseException("Expected a synonym, got " + previous());
    }
    synonyms.push_back(*synonym);
  } else {
    while (!match(">")) {
      auto synonym_str = advance();
      auto synonym = Synonym::construct(synonym_str);

      if (!synonym) {
        throw PQLParseException("Expected a synonym, got " + previous());
      }

      synonyms.push_back(*synonym);

      if (match(">")) {
        break;
      } else {
        expect(",");
      }
    }
  }

  for (const auto synonym : synonyms) {
    query_->result->selected_declarations->push_back(findDeclaration(synonym));
  }
};

Query QueryParser::parse() {
  // Parsing declarations
  while (!isAtEnd()) {
    while (true) {
      bool isDeclarationClause = parseDeclarationClause();
      if (!isDeclarationClause) {
        break;
      }
    }

    expect("Select");

    parseResult();
  }

  return *query_;
  // Parsing selects and clauses
}
