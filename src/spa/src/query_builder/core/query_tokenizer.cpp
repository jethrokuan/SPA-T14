#include "query_builder/core/query_tokenizer.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "query_builder/core/exceptions.h"
#include "utils/utils.h"

using namespace QE;
using namespace Utils;

// Define static members that need it
const std::string QueryTokenizerTokens::QUERY_TOKENIZER_TOKEN_DELIMITER = "|";
const char QueryTokenizer::QUERY_DELIMITER = ';';
const char QueryTokenizer::SPACE_DELIMITER = ' ';
const char QueryTokenizer::COMMA_DELIMITER = ',';
const std::string QueryTokenizer::DECLARATION_DUMMY_SEPARATOR = ";";
const std::string QueryTokenizer::QUERY_SELECT = "Select";

QueryTokenizerTokens QueryTokenizer::getTokens(std::string pql_query_string) {
  // Initialize the final list of tokens
  auto final_tokens = QueryTokenizerTokens();

  // No tokens to process
  if (pql_query_string.empty()) {
    return final_tokens;
  }

  // Remove newlines
  pql_query_string.erase(
      std::remove(pql_query_string.begin(), pql_query_string.end(), '\n'),
      pql_query_string.end());

  // Split input query string by semicolon
  auto& tokens = *(QueryTokenizer::splitString(
      pql_query_string, QueryTokenizer::QUERY_DELIMITER));

  // Tracking where we are in tokens
  size_t tokens_counter = 0;

  // Find the optional such_that clause and pattern clauses in the Select
  // statement
  for (std::string& tok : tokens) {
    // Remove leading and trailing whitespace from all tokens
    trim(tok);
    // Make sure this is the token with the select inside
    if (tok.compare(0, QueryTokenizer::QUERY_SELECT.length(),
                    QueryTokenizer::QUERY_SELECT) == 0) {
      // This gets all the declaration strings (all semicolon delim) until the
      // select clause
      auto declaration_token_strings = std::vector<std::string>(
          tokens.begin(), tokens.begin() + tokens_counter);
      // Start parsing each of the declarations (by each design entity)
      final_tokens.declaration_tokens = new std::vector<std::string>();
      for (std::string s : declaration_token_strings) {
        // Get the design entity (string until first space)
        auto first_space_pos = s.find(' ');
        // If no space found: tokenize error
        if (first_space_pos == std::string::npos) {
          throw PQLTokenizeException(
              "Could not find any synonyms when parsing declaration string " +
              s);
        }
        // If we found space, separate declaration into DE and synonyms
        auto de_token = s.substr(0, first_space_pos);
        auto synonym_string = s.substr(first_space_pos);

        // Remove all whitespace from synonyms
        std::string::iterator end_pos =
            std::remove(synonym_string.begin(), synonym_string.end(), ' ');
        synonym_string.erase(end_pos, synonym_string.end());
        // Get all synonyms that are currently stuck together - comma delimited
        auto synonym_tokens = QueryTokenizer::splitString(
            synonym_string, QueryTokenizer::COMMA_DELIMITER);
        // Add the design entity, and then every synonym associated with it
        final_tokens.declaration_tokens->push_back(de_token);
        final_tokens.declaration_tokens->insert(
            final_tokens.declaration_tokens->end(), synonym_tokens->begin(),
            synonym_tokens->end());
        // Indicate to pre-processor that this is the end of a single
        // declaration entity type
        final_tokens.declaration_tokens->push_back(
            QueryTokenizer::DECLARATION_DUMMY_SEPARATOR);
      }
      // Add the other token lists (select, such_that, pattern)
      setClauses(tok, final_tokens);
      break;
    }
    tokens_counter++;
  }
  if (final_tokens.declaration_tokens == nullptr ||
      final_tokens.select_tokens == nullptr) {
    throw PQLTokenizeException(
        "Either no declarations or no Select statement present");
  }
  return final_tokens;
}

// TODO: Handle left and right whitespace
void QueryTokenizer::setClauses(std::string& select_clause,
                                QueryTokenizerTokens& qtt) {
  // Space separate all tokens to begin analysis
  auto initial_clause_tokens = QueryTokenizer::splitString(
      select_clause, QueryTokenizer::SPACE_DELIMITER);

  // First two tokens should be "Select" and "<synonym>" always
  auto first = initial_clause_tokens->begin();

  if (initial_clause_tokens->size() < 2) {
    throw PQLTokenizeException(
        "Invalid number of tokens found for Select statement. Expected > 2, "
        "found " +
        std::to_string(initial_clause_tokens->size()));
  }

  auto select_tokens = new std::vector<std::string>(first, first + 2);
  qtt.select_tokens = select_tokens;

  // Potentially no such_that or pattern clause - return early if so
  if (initial_clause_tokens->size() == 2) {
    return;
  }

  int such_count = std::count(initial_clause_tokens->begin(),
                              initial_clause_tokens->end(), "such");
  int that_count = std::count(initial_clause_tokens->begin(),
                              initial_clause_tokens->end(), "that");
  int pattern_count = std::count(initial_clause_tokens->begin(),
                                 initial_clause_tokens->end(), "pattern");

  if (such_count > 1 || that_count > 1 || pattern_count > 1) {
    throw PQLTokenizeException(
        "More such / that / pattern tokens than expected!");
  }

  // Find such_that position if exists
  size_t such_that_pos = 0;
  for (size_t i = 2; i < initial_clause_tokens->size(); i++) {
    if ((*initial_clause_tokens)[i] == "such" &&
        (*initial_clause_tokens)[i + 1] == "that") {
      such_that_pos = i;
    }
  }

  // Find pattern position if exists
  size_t pattern_pos = 0;
  for (size_t i = 2; i < initial_clause_tokens->size(); i++) {
    if ((*initial_clause_tokens)[i] == "pattern") {
      pattern_pos = i;
    }
  }

  // If "such that" found, store tokens
  if (such_that_pos != 0) {
    // We have a such that clause - can take from here until the end of string
    // OR pattern
    first = initial_clause_tokens->begin() + such_that_pos;
    auto last = pattern_pos == 0 ? initial_clause_tokens->end()
                                 : initial_clause_tokens->begin() + pattern_pos;
    auto such_that_tokens = new std::vector<std::string>(first, last);
    qtt.such_that_tokens = such_that_tokens;
  }

  // If "pattern" found, store tokens
  if (pattern_pos != 0) {
    // We have a pattern clause - can take from here until the end of string
    first = initial_clause_tokens->begin() + pattern_pos;
    auto last = initial_clause_tokens->end();
    auto pattern_tokens = new std::vector<std::string>(first, last);
    qtt.pattern_tokens = pattern_tokens;
  }
  // Dynamically allocated in splitString
  delete initial_clause_tokens;
}

// Destroys str to produce a new vector of tokens
std::vector<std::string>* QueryTokenizer::splitString(std::string str,
                                                      const char delimiter) {
  auto tokens = new std::vector<std::string>();
  size_t pos = 0;
  while ((pos = str.find(delimiter)) != std::string::npos) {
    tokens->push_back(str.substr(0, pos));
    str.erase(0, pos + 1);
  }
  tokens->push_back(str);
  return tokens;
}

// This namespace definition is needed since this function needs to be part of
// the QE namespace
namespace QE {
std::ostream& operator<<(std::ostream& outputStream,
                         const QueryTokenizerTokens& qtt) {
  outputStream << "\nDeclaration Tokens:\n\t";
  for (auto t : *(qtt.declaration_tokens)) {
    outputStream << t << QueryTokenizerTokens::QUERY_TOKENIZER_TOKEN_DELIMITER;
  }
  outputStream << "\nSelect Tokens\n\t";
  for (auto t : *(qtt.select_tokens)) {
    outputStream << t << QueryTokenizerTokens::QUERY_TOKENIZER_TOKEN_DELIMITER;
  }
  if (qtt.such_that_tokens != nullptr) {
    outputStream << "\nSuch That Tokens\n\t";
    for (auto t : *(qtt.such_that_tokens)) {
      outputStream << t
                   << QueryTokenizerTokens::QUERY_TOKENIZER_TOKEN_DELIMITER;
    }
  }
  if (qtt.pattern_tokens != nullptr) {
    outputStream << "\nPattern Tokens\n\t";
    for (auto t : *(qtt.pattern_tokens)) {
      outputStream << t
                   << QueryTokenizerTokens::QUERY_TOKENIZER_TOKEN_DELIMITER;
    }
  }
  return outputStream;
}
}  // namespace QE
