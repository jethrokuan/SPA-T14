#include "query_evaluator/query_preprocessor.h"
#include <algorithm>
#include <iostream>
#include <variant>
#include "pql.h"
#include "query_evaluator/query_tokenizer.h"

using namespace QE;

Query* QueryPreprocessor::getQuery(std::string& pql_query_string) {
  // Tokenize the query string into a struct containing vector of strings
  auto query_tokenizer = QueryTokenizer();
  QueryTokenizerTokens query_tokens =
      query_tokenizer.getTokens(pql_query_string);

  // Begin to fill up query object with components of query
  auto query = new Query();

  QueryPreprocessor::parseDeclarations(query, query_tokens.declaration_tokens);
  QueryPreprocessor::parseSelect(query, query_tokens.select_tokens);
  QueryPreprocessor::parseSuchThat(query, query_tokens.such_that_tokens);

  return query;
}

void QueryPreprocessor::parseDeclarations(
    Query* query, std::vector<std::string>* declaration_tokens) {
  // Default value for de
  auto de = DesignEntity::ASSIGN;
  auto decls = new std::vector<Declaration>();
  for (size_t i = 0; i < declaration_tokens->size(); i++) {
    if (i % 2 == 0) {
      // Design entity
      // TODO: This can throw an error if the DE isn't recognized (incorrect
      // DE)!
      de = getDesignEntity((*declaration_tokens)[i]);
    } else {
      // This is the synonym half of the decl: store the DE we have into our
      // list
      auto synonym = (*declaration_tokens)[i];
      decls->push_back(Declaration(de, synonym));
    }
  }
  query->declarations = decls;
}

void QueryPreprocessor::parseSelect(Query* query,
                                    std::vector<std::string>* select_tokens) {
  if (select_tokens->size() != 2) {
    // Error condition: depends on error handling strategy for project
    // TODO
    std::cout << "Select tokens invalid";
    return;
  }

  // The synonym in the Select statement (e.g. {"Select", "p"} --> "p")
  std::string synonym_to_match = select_tokens->at(1);

  // Search declarations to find one that matches this synyonm
  // TODO: CHECK FOR NULLPTR
  query->selected_declaration =
      findDeclaration(query->declarations, synonym_to_match);
}

void QueryPreprocessor::parseSuchThat(
    Query* query, std::vector<std::string>* such_that_tokens) {
  if (such_that_tokens == nullptr) {
    return;
  } else if (such_that_tokens->size() <= 2) {
    // Error condition: depends on error handling strategy for project
    // TODO
    std::cout << "Such that tokens invalid";
    return;
  }

  // Join and all such-that tokens excluding "such" and "that"
  std::string joined_such_that;
  for (auto it = such_that_tokens->begin() + 2; it != such_that_tokens->end();
       ++it) {
    joined_such_that += *it;
  }

  // Erase all whitespace
  std::string::iterator end_pos =
      std::remove(joined_such_that.begin(), joined_such_that.end(), ' ');
  joined_such_that.erase(end_pos, joined_such_that.end());

  auto suchThat = SuchThat();

  std::cout << "to match: " << joined_such_that << std::endl;

  // For a Relation to match: it must match 'RelationName'+'(' since some
  // relations are substrings of other relations
  const auto& stringToRelationMap = getRelationToStringMap();
  size_t found_end_idx = 0;
  bool found = false;
  for (const auto& r : stringToRelationMap) {
    auto string_to_match = r.second + '(';
    auto found_start_idx = joined_such_that.find(string_to_match);
    if (found_start_idx != std::string::npos) {
      // Set the relation we found - args set later
      suchThat.relation = r.first;
      found_end_idx = found_start_idx + r.second.length();
      std::cout << "Found: " << string_to_match
                << " ending at: " << found_end_idx << std::endl;
      found = true;
      break;
    }
  }

  // TODO: proper error handling
  if (!found) {
    std::cout << "Cannot find Relation\n";
    return;
  }

  auto arg1_start_idx = found_end_idx + 1;
  auto arg1_len = joined_such_that.find(',') - arg1_start_idx;
  auto arg2_start_idx = joined_such_that.find(',') + 1;
  auto arg2_len = joined_such_that.find(')') - arg2_start_idx;

  // These are the two strings representing each argument to SuchThat
  auto arg1 = joined_such_that.substr(arg1_start_idx, arg1_len);
  auto arg2 = joined_such_that.substr(arg2_start_idx, arg2_len);

  std::cout << "Arg1: " << arg1 << std::endl;
  std::cout << "Arg2: " << arg2 << std::endl;

  // Parse arguments to get either entRef or stmtRef
}

// Find the first declaration that matches this synonym
Declaration* QueryPreprocessor::findDeclaration(
    std::vector<Declaration>* declarations, std::string synonym_to_match) {
  auto found_declaration =
      std::find_if(declarations->begin(), declarations->end(),
                   [&](auto decl) { return decl.synonym == synonym_to_match; });

  return &declarations->at(
      std::distance(declarations->begin(), found_declaration));
}

// Tries to produce either a statement or entity reference from the given string
static void argToStmtEntityRef(std::vector<Declaration>* declarations,
                               std::string arg) {
  return;
}
