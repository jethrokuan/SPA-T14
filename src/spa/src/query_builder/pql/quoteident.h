#pragma once
#include <iostream>
#include <optional>
#include <regex>

namespace QE {
//! Represents a quoted identifier in the PQL query (e.g. "abc")
class QuoteIdent {
 private:
  // Spec: LETTER (LETTER | DIGIT)*
  static const std::regex quoteident_regex;

 public:
  QuoteIdent() {}
  QuoteIdent(std::string quote_ident) : quote_ident(quote_ident) {}

  std::string quote_ident;
  // Constructs a guaranteed valid Synonym
  static std::optional<QuoteIdent> construct(std::string&);
  static std::optional<QuoteIdent> construct(const char*);

  bool operator==(const QuoteIdent& a2) const {
    return quote_ident == a2.quote_ident;
  }
  friend std::ostream& operator<<(std::ostream& os, QuoteIdent const& qi) {
    os << qi.quote_ident;
    return os;
  }
};
}  // namespace QE
