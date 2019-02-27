#include "query_builder/pql/quoteident.h"
using namespace QE;

// Definitions for QuoteIdent
const std::regex QuoteIdent::quoteident_regex =
    std::regex("\"[a-zA-Z](\\d|[a-zA-Z])*\"");
std::optional<QuoteIdent> QuoteIdent::construct(const char* quote_ident) {
  std::string qi(quote_ident);
  return QuoteIdent::construct(qi);
}
std::optional<QuoteIdent> QuoteIdent::construct(std::string& quote_ident) {
  if (std::regex_match(quote_ident, quoteident_regex)) {
    return QuoteIdent(quote_ident);
  } else {
    return std::nullopt;
  }
}