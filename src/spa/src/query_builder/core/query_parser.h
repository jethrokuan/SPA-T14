#pragma once

#include <string>
#include <vector>

#include <optional>
#include <unordered_set>

#include "query_builder/pql/attrref.h"
#include "query_builder/pql/declaration.h"
#include "query_builder/pql/query.h"
#include "query_builder/pql/ref.h"
#include "query_builder/pql/result.h"

using QE::Declaration;
using QE::Ref;
using QE::Result;

namespace QE {
class QueryParser {
 private:
  Query* query_;
  unsigned int current_;
  std::vector<std::string> tokens_;

  //! Checks if token at the current index has value equal to string
  //! s. If successful, consumes the token, advancing the index.
  bool match(std::string s);

  //! Returns true if token at the current index has value string s.
  bool check(std::string s);

  //! Asserts the token at current index to be of stings s.
  //! If true, consumes the token. Else, throws an exception.
  bool expect(std::string s);

  //! Returns true if the parser is at the end of file.
  bool isAtEnd();

  //! Returns the current token
  std::string peek();

  //! Returns the previous token
  std::string previous();

  //! Returns the current token, and consumes it.
  std::string advance();

  // Parses the declarations in a clause, and adds it into the Query state.
  // Returns false if not a declaration clause.
  Declaration* findDeclaration(const Synonym synonym);
  Expression parseExpression();
  ResultItem parseResultItem();
  void parseResult();
  AttrRef parseAttrRef();
  void parseAttrCompare();
  void parseAttrCond();
  bool parseWith();
  bool parseSuchThat();
  void parsePatternClause();
  bool parsePattern();
  void parseRelRef();
  void parseRelCond();
  bool parseModifies();
  Ref parseRef();
  bool parseDeclarationClause();

 public:
  explicit QueryParser(std::vector<std::string> tokens);

  Query parse();
};
}  // namespace QE
