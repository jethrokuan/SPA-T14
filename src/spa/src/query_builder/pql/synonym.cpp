#include "query_builder/core/exceptions.h"
#include "query_builder/pql/synonym.h"
#include "utils/utils.h"

using namespace QE;

Synonym::Synonym(std::string synonym_) {
  if (!Utils::is_valid_synonym(synonym_)) {
    throw PQLParseException("Invalid synonym: " + synonym_);
  }

  synonym = synonym_;
}
