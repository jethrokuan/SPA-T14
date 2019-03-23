#include "query_builder/pql/synonym.h"
#include <regex>
#include "query_builder/core/exceptions.h"
#include "utils/utils.h"

using namespace QE;

bool Synonym::isValidSynonym(std::string str) {
  return std::regex_match(str, std::regex("[a-zA-Z](\\d|[a-zA-Z])*"));
}

Synonym::Synonym(std::string synonym_) : synonym(synonym_) {
  if (!Synonym::isValidSynonym(synonym_)) {
    throw PQLParseException("Invalid synonym: " + synonym_);
  }
}
