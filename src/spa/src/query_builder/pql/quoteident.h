#pragma once
#include <iostream>
#include <optional>
#include <regex>

namespace QE {
//! Represents a quoted identifier in the PQL query (e.g. "abc")
class QuoteIdent {
 public:
  std::string quote_ident;

  QuoteIdent(std::string quote_ident_);

  bool operator==(const QuoteIdent& a2) const {
    return quote_ident == a2.quote_ident;
  }
  friend std::ostream& operator<<(std::ostream& os, QuoteIdent const& qi) {
    os << qi.quote_ident;
    return os;
  }
};
}  // namespace QE
