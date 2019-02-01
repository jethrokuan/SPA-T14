#include "query_evaluator/core/query_tokenizer.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace QE;

// Define static members that need it
const std::string QueryTokenizerTokens::QUERY_TOKENIZER_TOKEN_DELIMITER = "|";
const char QueryTokenizer::QUERY_DELIMITER = ';';
const char QueryTokenizer::SPACE_DELIMITER = ' ';
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

  // Trail a space and semicolon to make parsing work later when delim splitting
  pql_query_string += " ;";

  // Split input query string by semicolon
  auto& tokens = *(QueryTokenizer::splitString(
      pql_query_string, QueryTokenizer::QUERY_DELIMITER));

  // Tracking where we are in tokens
  size_t tokens_counter = 0;

  // Find the optional such_that clause and pattern clauses in the Select
  // statement
  for (std::string t : tokens) {
    // Make sure this is the token with the select inside
    if (t.compare(0, QueryTokenizer::QUERY_SELECT.length(),
                  QueryTokenizer::QUERY_SELECT) == 0) {
      // For each assignment token, split the tokens by space
      auto declaration_token_strings = std::vector<std::string>(
          tokens.begin(), tokens.begin() + tokens_counter);
      final_tokens.declaration_tokens = new std::vector<std::string>();
      for (std::string s : declaration_token_strings) {
        // Required for the string split to work correctly (trailing space)
        s += " ";
        auto split_declaration_tokens =
            QueryTokenizer::splitString(s, QueryTokenizer::SPACE_DELIMITER);
        final_tokens.declaration_tokens->insert(
            final_tokens.declaration_tokens->end(),
            split_declaration_tokens->begin(), split_declaration_tokens->end());
      }
      // Add the other token lists (select, such_that, pattern)
      setClauses(t, final_tokens);
      break;
    }
    tokens_counter++;
  }
  return final_tokens;
}

// TODO: There is no error checking here
// TODO: Handle left and right whitespace
void QueryTokenizer::setClauses(std::string& select_clause,
                                QueryTokenizerTokens& qtt) {
  // Space separate all tokens to begin analysis
  auto initial_clause_tokens = QueryTokenizer::splitString(
      select_clause, QueryTokenizer::SPACE_DELIMITER);

  // First two tokens should be "Select" and "<synonym>" always
  auto first = initial_clause_tokens->begin();
  auto select_tokens = new std::vector<std::string>(first, first + 2);
  qtt.select_tokens = select_tokens;

  // Potentially no such_that or pattern clause - return early if so
  if (initial_clause_tokens->size() == 2) {
    return;
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
