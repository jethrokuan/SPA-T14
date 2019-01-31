#include "query_evaluator/pql/synonym.h"
using namespace QE;

const std::regex Synonym::synonym_regex = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");
std::optional<Synonym> Synonym::construct(const char* synonym) {
  std::string s(synonym);
  return Synonym::construct(s);
}
std::optional<Synonym> Synonym::construct(std::string& synonym) {
  if (std::regex_match(synonym, synonym_regex)) {
    return Synonym(synonym);
  } else {
    return std::nullopt;
  }
}