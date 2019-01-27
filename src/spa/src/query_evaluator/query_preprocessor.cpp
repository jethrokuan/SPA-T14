#include "query_evaluator/query_preprocessor.h"
#include <algorithm>
#include <iostream>
#include "pql.h"
#include "query_evaluator/query_tokenizer.h"

using namespace QE;

Query *QueryPreprocessor::getQuery(std::string &pql_query_string) {
  // Tokenize the query string into a struct containing vector of strings
  auto query_tokenizer = QueryTokenizer();
  QueryTokenizerTokens query_tokens =
      query_tokenizer.getTokens(pql_query_string);

  // Begin to fill up query object with components of query
  auto query = new Query();

  QueryPreprocessor::parseDeclarations(query, query_tokens.declaration_tokens);
  QueryPreprocessor::parseSelect(query, query_tokens.select_tokens);

  return query;
}

void QueryPreprocessor::parseDeclarations(
    Query *query, std::vector<std::string> *declaration_tokens) {
  // Default value for de
  auto de = DesignEntity::ASSIGN;
  auto decls = new std::vector<Declaration>();
  for (int i = 0; i < declaration_tokens->size(); i++) {
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

void QueryPreprocessor::parseSelect(Query *query,
                                    std::vector<std::string> *select_tokens) {
  if (select_tokens->size() != 2) {
    // Error condition: depends on error handling strategy for project
    // TODO
    std::cout << "Select tokens invalid";
    return;
  }

  // The synonym in the Select statement (e.g. {"Select", "p"} --> "p")
  std::string synonym_to_match = select_tokens->at(1);

  // Find the first declaration that matches this synonym
  auto found_declaration =
      std::find_if(query->declarations->begin(), query->declarations->end(),
                   [&](auto decl) { return decl.synonym == synonym_to_match; });

  query->selected_declaration = &query->declarations->at(
      std::distance(query->declarations->begin(), found_declaration));
}