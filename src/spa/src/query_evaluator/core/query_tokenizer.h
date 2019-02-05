#pragma once
#include <memory>  // future use of smart pointers
#include <string>
#include <vector>

namespace QE {

//! \brief Aggregate class to return token data from QueryTokenizer to
//! QueryPreprocessor
class QueryTokenizerTokens {
 private:
  static const std::string QUERY_TOKENIZER_TOKEN_DELIMITER;

 public:
  std::vector<std::string>* declaration_tokens;
  std::vector<std::string>* select_tokens;
  std::vector<std::string>* such_that_tokens;
  std::vector<std::string>* pattern_tokens;
  friend std::ostream& operator<<(std::ostream& outputStream,
                                  const QueryTokenizerTokens& qtt);
  QueryTokenizerTokens()
      : declaration_tokens(nullptr),
        select_tokens(nullptr),
        such_that_tokens(nullptr),
        pattern_tokens(nullptr){};
};

/*! \brief Utility class that when given a PQL query as a string, splits the
 * string into tokens which are stored in a QueryTokenizerTokens struct based on
 * their role in the PQL query.
 */
class QueryTokenizer {
 private:
  const static char QUERY_DELIMITER;
  const static char SPACE_DELIMITER;
  const static std::string QUERY_SELECT;
  void setClauses(std::string&, QueryTokenizerTokens&);
  std::vector<std::string>* splitString(std::string, const char);

 public:
  // Given a PQL query as a string, splits the string into tokens
  // Returns the list of tokens as in a combined result struct
  QueryTokenizerTokens getTokens(std::string);
};
}  // namespace QE